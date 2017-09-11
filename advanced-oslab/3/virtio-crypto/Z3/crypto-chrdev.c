/*
 * crypto-chrdev.c
 *
 * Implementation of character devices
 * for virtio-crypto device 
 *
 * Vangelis Koukis <vkoukis@cslab.ece.ntua.gr>
 * Dimitris Siakavaras <jimsiak@cslab.ece.ntua.gr>
 * Stefanos Gerangelos <sgerag@cslab.ece.ntua.gr>
 *
 */
#include <linux/cdev.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/wait.h>
#include <linux/virtio.h>
#include <linux/virtio_config.h>

#include "crypto.h"
#include "crypto-chrdev.h"
#include "debug.h"

#include "cryptodev.h"

#define IV_SIZE 16
/*
 * Global data
 */
struct cdev crypto_chrdev_cdev;

/**
 * Given the minor number of the inode return the crypto device 
 * that owns that number.
 **/

static struct crypto_device *get_crypto_dev_by_minor(unsigned int minor)
{
	struct crypto_device *crdev;
	unsigned long flags;

	debug("Entering");

	spin_lock_irqsave(&crdrvdata.lock, flags);
	list_for_each_entry(crdev, &crdrvdata.devs, list) {
		if (crdev->minor == minor)
			goto out;
	}
	crdev = NULL;

out:
	spin_unlock_irqrestore(&crdrvdata.lock, flags);

	debug("Leaving");
	return crdev;
}

/*************************************
 * Implementation of file operations
 * for the Crypto character device
 *************************************/

static int crypto_chrdev_open(struct inode *inode, struct file *filp)
{
	int ret = 0;
	int err;
	unsigned int len;
	struct crypto_open_file *crof;
	struct crypto_device *crdev;
	unsigned int syscall_type = VIRTIO_CRYPTO_SYSCALL_OPEN;
	int host_fd = -1;
	unsigned long flag;
        struct scatterlist *sglist[2], syscall_type_sg, host_fd_sg;
        
        debug("Entering");
     
	ret = -ENODEV;
        if ((ret = nonseekable_open(inode, filp)) < 0)
                goto fail;

        /* Associate this open file with the relevant crypto device. */
        crdev = get_crypto_dev_by_minor(iminor(inode));
        if (!crdev) {
                debug("Could not find crypto device with %u minor",
                      iminor(inode));
                ret = -ENODEV;
                goto fail;
        }

        crof = kzalloc(sizeof(*crof), GFP_KERNEL);
        if (!crof) {
                ret = -ENOMEM;
                goto fail;
        }
        crof->crdev = crdev;
        crof->host_fd = -1;
	debug("SYSCALL_TYPE = %d",syscall_type);
        /**
         * We need two sg lists, one for syscall_type and one to get the 
         * file descriptor from the host.
         **/
	

	
        sg_init_one(&syscall_type_sg, &syscall_type, sizeof(syscall_type));
        sglist[0]=&syscall_type_sg;
        sg_init_one(&host_fd_sg, &host_fd, sizeof(host_fd));
        sglist[1]=&host_fd_sg;
        debug("Created sg_lists: launching vitqueue_add");
        spin_lock_irqsave(&crdev->vq_lock,flag);
	err = virtqueue_add_sgs(crdev->vq,sglist,1,1,&syscall_type_sg,GFP_ATOMIC);
        debug("Kicking");
        virtqueue_kick(crdev->vq);
        /**
         * Wait for the host to process our data.
        **/
        debug("virtqueue_get_buf");
	while(virtqueue_get_buf(crdev->vq,&len)==NULL);
	spin_unlock_irqrestore(&crdev->vq_lock,flag);
	crof->host_fd = host_fd;
	debug("len = %d",len);
	debug("SYSCALL_TYPE = %d",syscall_type);
	debug("host_fd = %d",host_fd);        
	debug("ret = %d",ret);
	if(host_fd == -1){
                ret = -ENODEV;
        	goto fail;
	}	   	
        filp->private_data = crof;
     
        /*
         If host failed to open() return -ENODEV. */ 
                

fail:
        debug("Leaving");
        return ret;
}

static int crypto_chrdev_release(struct inode *inode, struct file *filp)
{
	int ret = 0;
	int err = 0;
	int len = 0;
	struct crypto_open_file *crof = filp->private_data;
	struct crypto_device *crdev = crof->crdev;
	unsigned int syscall_type = VIRTIO_CRYPTO_SYSCALL_CLOSE;
	struct scatterlist *sglist[3], syscall_type_sg, host_fd_sg, syscall_ret;
	int host_fd = crof->host_fd;
	unsigned long flag;	

	sg_init_one(&syscall_type_sg, &syscall_type, sizeof(syscall_type));
        sglist[0]=&syscall_type_sg;
        sg_init_one(&host_fd_sg, &host_fd, sizeof(host_fd));
        sglist[1]=&host_fd_sg;
	sg_init_one(&syscall_ret,&ret,sizeof(ret));
	sglist[2]=&syscall_ret;
	debug("Entering");

	/**
         * Send data to the host.
         **/
	spin_lock_irqsave(&crdev->vq_lock,flag);
	err = virtqueue_add_sgs(crdev->vq,sglist,2,1,&syscall_type_sg,GFP_ATOMIC);
        debug("Kicking");
        virtqueue_kick(crdev->vq);

	/**
	 * Wait for the host to process our data.
	 **/
	debug("virtqueue_get_buf");
        while(virtqueue_get_buf(crdev->vq,&len)==NULL);
	spin_unlock_irqrestore(&crdev->vq_lock,flag);
	debug("close returned ret = %d",ret);

	kfree(crof);
	debug("Leaving");
	return ret;

}

static long crypto_chrdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct session_op sess;
	struct crypt_op cryp;
	long ret = 0;
	int i=0;
	int err;
	unsigned int md = cmd;
	struct crypto_open_file *crof = filp->private_data;
	struct crypto_device *crdev = crof->crdev;
	struct virtqueue *vq = crdev->vq;
	int fd = crof->host_fd;	
	unsigned char *session_key;
	unsigned char *src;
	unsigned char *iv;
	unsigned char *dst=NULL;
	unsigned long flag;
	struct scatterlist syscall_type_sg,sess_arg, src_sg, iv_sg, dest_sg, arg_sg, fd_sg, type_sg, ret_sg, *sgs[8];
#define MSG_LEN 100
	unsigned char output_msg[MSG_LEN], input_msg[MSG_LEN];
	unsigned int num_out, num_in,syscall_type = VIRTIO_CRYPTO_SYSCALL_IOCTL,len;
	debug("Entering");

	num_out = 0;
	num_in = 0;

	/**
	 *  These are common to all ioctl commands.
	 **/
	sg_init_one(&syscall_type_sg, &syscall_type, sizeof(syscall_type));
	sgs[num_out++] = &syscall_type_sg;
	/* ?? */
	sg_init_one(&fd_sg,&fd,sizeof(fd));
	sgs[num_out++] = &fd_sg;
	sg_init_one(&type_sg, &md, sizeof(md));
	sgs[num_out++] = &type_sg;
	/**
	 *  Add all the cmd specific sg lists.
	 **/
	debug("fd = %d",fd);
	debug("CIOCGSESSION = %ld",CIOCGSESSION);
        debug("CIOCFSESSION = %ld",CIOCFSESSION);
        debug("CIOCCRYPT = %ld",CIOCCRYPT);
	debug("cmd = %d",md);
	switch (cmd) {
	case CIOCGSESSION:
		debug("CIOCGSESSION");
		err = copy_from_user(&sess,(struct session_op __user *)arg,sizeof(sess));
		debug("CRYPTO_AES_CBC = %d",CRYPTO_AES_CBC);
		debug("sess.cipher = %d",sess.cipher);
		//debug("KEY_SIZE = %d",KEY_SIZE);
		debug("sess.keylen = %d",sess.keylen);
		
		
		session_key = kmalloc(sess.keylen*sizeof(unsigned char),GFP_KERNEL);
                memset(session_key,0,sess.keylen*sizeof(unsigned char));
                err = copy_from_user(session_key,((struct session_op __user *)arg)->key,sess.keylen*sizeof(unsigned char));
                sg_init_one(&sess_arg, &session_key,sess.keylen*sizeof(unsigned char));
                sgs[num_out++]=&sess_arg;


		sg_init_one(&arg_sg, &sess,sizeof(sess));
		sgs[num_out + num_in++] = &arg_sg;
		
		break;

	case CIOCFSESSION:
		debug("CIOCFSESSION");
                err = copy_from_user(&sess,(struct session_op __user *)arg,sizeof(sess));
		sg_init_one(&arg_sg, &sess,sizeof(sess));
		sgs[num_out++] = &arg_sg;
		break;

	case CIOCCRYPT:
		debug("CIOCCRYPT");
                err = copy_from_user(&cryp,(struct crypt_op __user *)arg,sizeof(cryp));
		sg_init_one(&arg_sg, &cryp,sizeof(cryp));
		sgs[num_out++] = &arg_sg;
		

		src = kmalloc(cryp.len*sizeof(unsigned char),GFP_KERNEL);
		memset(src,0,cryp.len*sizeof(unsigned char));
		err = copy_from_user(src,((struct crypt_op __user *)arg)->src,cryp.len*sizeof(unsigned char));
		sg_init_one(&src_sg, src,cryp.len*sizeof(unsigned char));
		sgs[num_out++] = &src_sg;
		

		iv = kmalloc(IV_SIZE*sizeof(unsigned char),GFP_KERNEL);
		memset(iv,0,IV_SIZE*sizeof(unsigned char));
		err = copy_from_user(iv,((struct crypt_op __user *)arg)->iv,IV_SIZE*sizeof(unsigned char));
		sg_init_one(&iv_sg, iv,IV_SIZE*sizeof(unsigned char));
                sgs[num_out++] = &iv_sg;
		
		dst = kmalloc(cryp.len*sizeof(unsigned char),GFP_KERNEL);
                memset(dst,0,cryp.len*sizeof(unsigned char));
		sg_init_one(&dest_sg, dst,cryp.len*sizeof(unsigned char));
                sgs[num_out + num_in++] = &dest_sg;

		debug("cryp.len = %d",cryp.len);	
		for (i = 0; i < cryp.len; i++) {
         	       printk(KERN_DEBUG "%x", cryp.src[i]);
        	}
		
		break;
	default:
		debug("Unsupported ioctl command");
		break;
	}
	sg_init_one(&ret_sg, &ret,sizeof(ret));
	sgs[num_out + num_in++] = &ret_sg;
	/**
	 * Wait for the host to process our data.
	 **/
	/* ?? */
	/* ?? Lock ?? */
	debug("Virtqueue add sgs on ioctl::");
	debug("num_out = %d",num_out);
	debug("num_in = %d",num_in);
	spin_lock_irqsave(&crdev->vq_lock,flag);
	err = virtqueue_add_sgs(crdev->vq, sgs, num_out, num_in, &syscall_type_sg, GFP_ATOMIC);
	virtqueue_kick(crdev->vq);
	while (virtqueue_get_buf(crdev->vq, &len) == NULL);
	/* do nothing */
	spin_unlock_irqrestore(&crdev->vq_lock,flag);
	switch (cmd) {
        case CIOCGSESSION:
        case CIOCFSESSION:
		debug("copy_to_user:  CIOCG/FSESSION");
		err = copy_to_user((struct session_op __user *)arg,&sess,sizeof(sess));
		break;
        case CIOCCRYPT:
		debug("copy_to_user:  CIOCCRYPT");
		err = copy_to_user(((struct crypt_op __user *)arg)->dst,dst,cryp.len*sizeof(unsigned char));
                break;
        default:
		debug("Unsupported ioctl command");
                break;
        }

	debug("<troika>");

	debug("Leaving with ret = %ld",ret);

	return ret;
}

static ssize_t crypto_chrdev_read(struct file *filp, char __user *usrbuf, 
                                  size_t cnt, loff_t *f_pos)
{
	debug("Entering");
	debug("Leaving");
	return -EINVAL;
}

static struct file_operations crypto_chrdev_fops = 
{
	.owner          = THIS_MODULE,
	.open           = crypto_chrdev_open,
	.release        = crypto_chrdev_release,
	.read           = crypto_chrdev_read,
	.unlocked_ioctl = crypto_chrdev_ioctl,
};

int crypto_chrdev_init(void)
{
	int ret;
	dev_t dev_no;
	unsigned int crypto_minor_cnt = CRYPTO_NR_DEVICES;
	
	debug("Initializing character device...");
	cdev_init(&crypto_chrdev_cdev, &crypto_chrdev_fops);
	crypto_chrdev_cdev.owner = THIS_MODULE;
	
	dev_no = MKDEV(CRYPTO_CHRDEV_MAJOR, 0);
	ret = register_chrdev_region(dev_no, crypto_minor_cnt, "crypto_devs");
	if (ret < 0) {
		debug("failed to register region, ret = %d", ret);
		goto out;
	}
	ret = cdev_add(&crypto_chrdev_cdev, dev_no, crypto_minor_cnt);
	if (ret < 0) {
		debug("failed to add character device");
		goto out_with_chrdev_region;
	}

	debug("Completed successfully");
	return 0;

out_with_chrdev_region:
	unregister_chrdev_region(dev_no, crypto_minor_cnt);
out:
	return ret;
}

void crypto_chrdev_destroy(void)
{
	dev_t dev_no;
	unsigned int crypto_minor_cnt = CRYPTO_NR_DEVICES;

	debug("entering");
	dev_no = MKDEV(CRYPTO_CHRDEV_MAJOR, 0);
	cdev_del(&crypto_chrdev_cdev);
	unregister_chrdev_region(dev_no, crypto_minor_cnt);
	debug("leaving");
}
