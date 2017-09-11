/*
 * lunix-chrdev.c
 *
 * Implementation of character devices
 * for Lunix:TNG
 *
 * < Your name here >
 *
 */

#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/cdev.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mmzone.h>
#include <linux/vmalloc.h>
#include <linux/spinlock.h>
#include <linux/string.h>

#include "lunix.h"
#include "lunix-chrdev.h"
#include "lunix-lookup.h"

/*
 * Global data
 */
struct cdev lunix_chrdev_cdev;

/*
 * Just a quick [unlocked] check to see if the cached
 * chrdev state needs to be updated from sensor measurements.
 */
static int lunix_chrdev_state_needs_refresh(struct lunix_chrdev_state_struct *state)
{
	struct lunix_sensor_struct *sensor;
	sensor = state->sensor;	
	WARN_ON ( !(sensor = state->sensor));
        printk(KERN_DEBUG "Entered refresh\n");
	/* ? */
	if(state->sensor->msr_data[state->type]==NULL){
		return 0;
	}
	return (state->buf_timestamp < sensor->msr_data[state->type]->last_update); /* ? */
}

/*
 * Updates the cached state of a character device
 * based on sensor data. Must be called with the
 * character device state lock held.
 */
static int lunix_chrdev_state_update(struct lunix_chrdev_state_struct *state)
{
	struct lunix_sensor_struct *sensor;
	uint32_t read;
	unsigned char A[20];
	long temp;
	unsigned long flag;
		
	debug("leaving\n");
        printk(KERN_DEBUG "update entered\n");
	sensor = state->sensor;

	/*
	 * Grab the raw data quickly, hold the
	 * spinlock for as little as possible.
	 */
	spin_lock_irqsave(&sensor->lock,flag);
        printk(KERN_DEBUG "Enter update and locked \n");
	/* ? */
	if(sensor->msr_data[state->type]->values == NULL || !lunix_chrdev_state_needs_refresh(state)){
		spin_unlock(&sensor->lock);
		return -EAGAIN;
	}
	else{
		read = sensor->msr_data[state->type]->values[0];
		state->buf_timestamp = sensor->msr_data[state->type]->last_update;
	}
	/* Why use spinlocks? See LDD3, p. 119 */
	
	/*
	 * Any new data available?
	 */
	spin_unlock_irqrestore(&sensor->lock,flag);
	switch(state->type){
		case BATT :
			temp = lookup_voltage[(int)read];
			break;		
		case TEMP :
			temp = lookup_temperature[(int)read];
			break;
		case LIGHT:
			temp = lookup_light[(int)read];
			break;
		default :
			return -EFAULT;
	}
	
	/* ? */
	if(temp<0){
		sprintf(A,"%ld,%ld\n",temp/1000,(-temp)%1000);
	}
	else{
		sprintf(A,"%ld,%ld\n",temp/1000,(temp)%1000);
	}
	strcpy(state->buf_data,A);
	state->buf_lim = (unsigned int)strlen(A);
	/*
	 * Now we can take our time to format them,
	 * holding only the private state semaphore
	 */

	/* ? */

//out:
	debug("leaving\n");
	return 0;
}

/*************************************
 * Implementation of file operations
 * for the Lunix character device
 *************************************/

static int lunix_chrdev_open(struct inode *inode, struct file *filp)
{
	/* Declarations */
	/* ? */
	int ret;
	struct lunix_chrdev_state_struct *s = (struct lunix_chrdev_state_struct *)kmalloc(sizeof(struct lunix_chrdev_state_struct), GFP_KERNEL);

	debug("entering\n");
        printk(KERN_DEBUG "Open started\n");	
	ret = -ENODEV;
	if ((ret = nonseekable_open(inode, filp)) < 0)
		goto out;
	
	//my code
	s->type=(enum lunix_msr_enum)(iminor(inode)%8);
	printk(KERN_DEBUG "open : s->type = %d\n",s->type);
	s->sensor = &lunix_sensors[iminor(inode)/8]; 
	s->buf_timestamp=0;
	sema_init(&s->lock,1);
	spin_lock_init(&s->sensor->lock);
	filp->private_data = s;
	//end	
	

	/*
	 * Associate this open file with the relevant sensor based on
	 * the minor number of the device node [/dev/sensor<NO>-<TYPE>]
	 */


	/* Allocate a new Lunix character device private state structure */
	/* ? */
out:
	debug("leaving, with ret = %d\n", ret);
	return ret;
}

static int lunix_chrdev_release(struct inode *inode, struct file *filp)
{
	kfree(filp->private_data);
	return 0;
}

static long lunix_chrdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	/* Why? */
	return -EINVAL;
}

static ssize_t lunix_chrdev_read(struct file *filp, char __user *usrbuf, size_t cnt, loff_t *f_pos)
{
	ssize_t ret;

	struct lunix_sensor_struct *sensor;
	struct lunix_chrdev_state_struct *state;

	state = filp->private_data;
	WARN_ON(!state);

	sensor = state->sensor;
	WARN_ON(!sensor);
	
	/* Lock? */
	if (down_interruptible(&state->lock))
		return -ERESTARTSYS;	
	printk(KERN_DEBUG "locked!!! read\n");
	/*
	 * If the cached character device state needs to be
	 * updated by actual sensor data (i.e. we need to report
	 * on a "fresh" measurement, do so
	 */
	if (*f_pos == 0) {
	        printk(KERN_DEBUG "read from scratch\n");
		while (lunix_chrdev_state_update(state) == -EAGAIN) {
			/* ? */
			up(&state->lock);
			printk(KERN_DEBUG "no update\n");
			if(wait_event_interruptible(sensor->wq,lunix_chrdev_state_needs_refresh(state)))
				return -ERESTARTSYS;
			if (down_interruptible(&state->lock))
                                return -ERESTARTSYS;
			/* The process needs to sleep */
			/* See LDD3, page 153 for a hint */
		}
		printk(KERN_DEBUG "READ SUCCESS .... \n");

        	printk(KERN_DEBUG "Lets see: \n");
        	printk(KERN_DEBUG "BUFF_DATA: %s \n", state->buf_data);      	
	}
	debug("Continue Reading\n");
	if(((int)(*f_pos) + (int)cnt) <= state->buf_lim){
		int ll;
                printk(KERN_DEBUG "SECTOR 1 with f_pos = %d and buf_lim = %d\n",(int)*f_pos,state->buf_lim);
		if ((ll=copy_to_user(usrbuf, &state->buf_data[(int)*f_pos], cnt))!=0) {
			debug("What the fuck happened: EFAULT(1) : copy_to_user = %d",ll);
			printk(KERN_DEBUG "Was about to read < %d > data beggining from character < %c > of string < %s >, while *fpos = %d, and state->buf_lim=%d \n",(int)cnt,state->buf_data[(int)*f_pos],state->buf_data,(int)*f_pos,state->buf_lim);
			ret = -EFAULT;
			goto out;
		}
		*f_pos = *f_pos + cnt;
		if(*f_pos == state->buf_lim){
			*f_pos = 0;
		}
		ret = cnt;
		printk(KERN_DEBUG "exiting ... SECTOR 1 with ret = %d,cnt = %d\n",ret,cnt);
	}		
	else{
		printk(KERN_DEBUG "SECTOR 2 with f_pos = %d and buf_lim = %d\n",*f_pos,state->buf_lim);
		int lll;
		if ((lll=copy_to_user(usrbuf, &state->buf_data[(int)*f_pos], (state->buf_lim - (int)*f_pos)))!=0){
                        debug("What the fuck happened: EFAULT(2) : copy_to_user = %d",lll);
			printk(KERN_DEBUG "Was about to read < %d > data beggining from character < %c > of string < %s >, while *fpos = %d, and state->buf_lim=%d \n",(state->buf_lim - (int)*f_pos),state->buf_data[(int)*f_pos],state->buf_data,(int)*f_pos,state->buf_lim);
			ret = -EFAULT;
                        goto out;
                }
		ret =state->buf_lim - (int)*f_pos;
		printk(KERN_DEBUG "exiting ... SECTOR 2 with ret = %d,buf_lim = %d, *f_pos = %d \n",ret,state->buf_lim,(int)*f_pos);
		*f_pos = 0;
	}
	/* End of file */
	/* ? */
	
	/* Determine the number of cached bytes to copy to userspace */
	/* ? */

	/* Auto-rewind on EOF mode? */
	/* ? */
out:
	/* Unlock? */
	up(&state->lock);
	return ret;
}

static int lunix_chrdev_mmap(struct file *filp, struct vm_area_struct *vma)
{
	return -EINVAL;
}

static struct file_operations lunix_chrdev_fops = 
{
        .owner          = THIS_MODULE,
	.open           = lunix_chrdev_open,
	.release        = lunix_chrdev_release,
	.read           = lunix_chrdev_read,
	.unlocked_ioctl = lunix_chrdev_ioctl,
	.mmap           = lunix_chrdev_mmap
};

int lunix_chrdev_init(void)
{
	/*
	 * Register the character device with the kernel, asking for
	 * a range of minor numbers (number of sensors * 8 measurements / sensor)
	 * beginning with LINUX_CHRDEV_MAJOR:0
	 */
	int ret;
	dev_t dev_no;
	unsigned int lunix_minor_cnt = lunix_sensor_cnt << 3;
	
	debug("initializing character device\n");
	cdev_init(&lunix_chrdev_cdev, &lunix_chrdev_fops);
	lunix_chrdev_cdev.owner = THIS_MODULE;
	
	dev_no = MKDEV(LUNIX_CHRDEV_MAJOR, 0);
	/* ? */
	ret = register_chrdev_region(dev_no,lunix_minor_cnt,"lunix-tng");
	/* register_chrdev_region(LOOK OK) */
	if (ret < 0) {
		debug("failed to register region, ret = %d\n", ret);
		goto out;
	}	
	/* ? */
	ret = cdev_add(&lunix_chrdev_cdev,dev_no,lunix_minor_cnt);
	/* cdev_add? */
	if (ret < 0) {
		debug("failed to add character device\n");
		goto out_with_chrdev_region;
	}
	debug("completed successfully\n");
	return 0;

out_with_chrdev_region:
	unregister_chrdev_region(dev_no, lunix_minor_cnt);
out:
	return ret;
}

void lunix_chrdev_destroy(void)
{
	dev_t dev_no;
	unsigned int lunix_minor_cnt = lunix_sensor_cnt << 3;
		
	debug("entering the destruction protzes\n");
	dev_no = MKDEV(LUNIX_CHRDEV_MAJOR, 0);
	cdev_del(&lunix_chrdev_cdev);
	unregister_chrdev_region(dev_no, lunix_minor_cnt);
	debug("leaving\n");
}
