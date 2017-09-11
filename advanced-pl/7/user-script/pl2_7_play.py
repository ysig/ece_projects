#!/usr/bin/env python
import requests
import re
import sys
from math import sqrt

#def play(number):
#	moves = (
#			"!.,",#0
#			":.", #1
#			"::+.",#2
#			":::++.",#3
#			"::+:+.",#4
#			":::+:++.",#5
#			":::++:+.",#6
#			"::::++:++.",#7
#			"::+:+:+.",#8
#			":::++::++."#9
#			)
#	befunge = "!"
#	digits = map(int,list(str(number)))
#	print digits
#	for d in digits:
#		befunge += moves[d]
#	befunge += "@"
#	return befunge

def build_pow2(number):
	#suppose your stack looks like this: [bottom] 1 *** 2 [top]
	res = ""
	if (number>1):
		res = ((number)*":")+((number-1)*"*")+"\\";
	return res;

def fold(program):
	plen = len(program)
	#for square use this
	n = int(sqrt(plen))
	'''
	#based on the torus 80x25 size
	n = 23
	'''
	folded = list()
	j=0
	for i in range(0, plen, n):
		if(j%2==0):
			if(i+n>plen):
				folded.append(">"+program[i:i+n]+((n-len(program[i:i+n])+1)*" "))
			else:
				folded.append(">"+program[i:i+n]+"v")
		elif(j%2==1):
			t = program[i:i+n]
			if(i+n>plen):	
				folded.append(((n-len(t)+1)*" ")+t[::-1]+"<")
			else:
				folded.append("v"+t[::-1]+"<")				
		j+=1
	return ('\n'.join(folded))



def play(number,print_msg):
	befunge = "!"
	b = "{0:b}".format(int(number))
	bl = map(int,list(b))
	nb = len(bl)
	res = "!::+" #suppose your stack now like this: [bottom] 1 2 [top]
	if(print_msg):
		print "Number's "+str(number)+" Binary is "+str(b);
	exp = nb-1;
	muls = 0;
	for bit in bl:
	#	print "Bit = " + str(bit) + " on exp = " + str(exp)
		if(bit):
			res+=build_pow2(exp)
			muls+=1
		exp-=1
	#print "Muls = " + str(muls)
	no2 = (not bl[-2]) if (nb>=2) else true
	res+=((no2)*"$")+((muls-1)*"+")+".@"
	return res

in_size = len(sys.argv)
fold_f = True
#site = "http://localhost/pl2/7/funge.php"
site = "http://courses.softlab.ntua.gr/pl2/2016b/exercises/funge.php"	
if(in_size>=2):
	site = sys.argv[1]
	if(in_size>=3):
		fold_f = str(sys.argv[2])[:len("True")] == "True"
	if(in_size>=4):
		print "Arguments have been omitted"

dev = False
if(not(dev)):
	rounds = 10
	s = requests.Session()
	r = s.get(site)
	for i in range(1,rounds+1):
		#ci = raw_input("Do you want to continue? (Y/n)\n>")	
		#c = str(ci)
		#if(c == "Y" or c == "y"):
		#	print "Next:\n"
		#elif(c == "n"):
		#	break
		#else:
		#	continue
		txt = r.text
		m = re.search('<span class="question">[-+]?\d+</span>',txt)
		number = m.group().strip('<span class="question">').strip('</span>')
		print "Round "+str(i)+", number "+str(number)
		game_res = play(number,False)
		program = game_res
		if (fold_f):
			game_res = fold(game_res)
			program = game_res+'\n'+((game_res.index('\n')-1)*" ")
			# due to a strange bug (not caused from my code)
			# if the last line of our input has as much characters as the others
			# it is not considered a rectangle on posting
			# when on the contrary it is when it consists of one less
			# Thus i added one more line of n-1 whitespace size,
			# where n is the size of each line
		inp = {'program' : program, 'submit' : 'Submit!'}
		r = s.post(site, data=inp)
		print "Submitted solution:\n"+game_res
		if (re.search('"right">Right!',r.text)):
			print "Right!  :-)"
			if(i==rounds):
				break
			s = requests.Session()
			r = s.post(site,data={'again':'Play again!'})
		elif (re.search('"wrong">Wrong!',r.text)):
			print "Wrong! :-("
			#print r.text
			if(i==rounds):
				break
			s = requests.Session()
			r = s.post(site,data={'again':'Try again!'})
else:
	a = play(input("Insert the desired number to generate a Befunge-93 program..:\n>"),True)
	print "Unfolded:\n"+a
	print "Folded:\n"+fold(a)
