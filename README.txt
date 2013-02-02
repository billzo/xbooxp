============================| Intro |===============================

XBOO XP is a multiboot transfer software designed with winXP/2000 in mind, but 
should work in Win98/95. This program will NOT work in raw DOS mode.

XBOO cable is cheap and easy to build. Costs about $5 to build. Plus you can send 
multiboot images of up to 256kB, and upload speed is fast. This program can send a 256kB
multiboot image in about 8.1 - 8.5 seconds in my AMD 2100++ CPU. With the delay 
setting, its a little slower.

Even though this program runs in DOS, it is made for WinXP/2000. Usage of batch files
recommended. This program does not require other software like Userport to run, it's 
pretty much independent. It has a kernel mode driver which allows it to access the parallel 
port and the PC speaker(for timing), that unloads itself after exit. This is much safer 
cause there will be no security holes in your PC.

This ia an old project, I uploaded this in Google Code to get the code out there for hackers
to play with it. I am no longer supporting this project. This is because I dont use Windows
anymore. Linux is so much better, in my humble opinion.



============================| License |=============================

xbooxp is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

xbooxp is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with xbooxp.  If not, see <http://www.gnu.org/licenses/>.

xbooxp makes use of io.dll, which is free, and available at
http://www.geekhideout.com/iodll.shtml


============================| Usage |===============================

Run.bat is a batch file included for your ease. Edit the file with notepad. To run XBOOXP, double 
click the file. To edit the file, right click and choose edit.

Make sure your compiled .mb file is in the same directory as XBOOXP. Or you can move XBOOXP into
your program source directory.

To use XBOOXP, you will need to edit this file to suit you purposes. This is a basic explanation:

XBOOXP yourfile.mb -argument

the arguments :
			-h		use this to upload multiboot file without a valid
					header. This does not affect or modify the file.

			-d[value]	To increase the delay value, use this. Example,
						if i want to use delay=1, then type -d1.
						Use this only if you have a lot of problems.

			-p[port]	Set the port address here. If your port address
						is 0x278, then type -p278. If no value specified
						then 0x378 will be used as default.

			-t		Set for testing purposes. This will allow you to 
					send files without actually starting them. The
					file will be sent, but wont start. Only for
					testing purposes.


To see the help screen, just type XBOOXP, and you will see the above message(or similar).

So, if your parellel port address is 0x278, you want delay=2, and you want to upload a multiboot file 
named matrix.mb without a valid header(too lazy to use GBAFIX), just put this line in run.bat:

	XBOOXP matrix.mb -278 -h -d2

Simple as pie.



==========================| FAQ |=============================

1. How to know your parallel port address?

i.   Right click on my computer and chose properties. Then click on the Hardware tab, and 
     choose Device Manager.

ii.  Look for Ports(Com & LTP), and expand this branch. Then right click on printer port and 
     choose properties.

iii. Go to the Resources Tab, and look at the first IO-Range value, if 378-3FF that means you 
     port is 0x378. This is the most common value.


2. Whats a batch file?

A batch file is a file with .bat extension. It is a set of CLI commands written in sequence.

To get a better idea on how a batch file works, check out run.bat file included. You can 
edit it using notepad. The @ECHO OFF command means that whatever typed will not be displayed 
in the screen. To run the batch file, just double-click it.

Why use a batch file? Cause everybody's is just too lazy to type out "xbooxp program.mb -278 -h" 
EVERYTIME in CLI just to test their program, right?


3. How do i build a Xboo cable?

It's REALLY EASY.
No need to be an engineer. But making one will make others believe you are        ;)
To build one, check the gbadev.org website. You only need to connect three wires, 
four IF you want to use high speed BURST BOOT. You can still use BURST BOOT with only three 
wires, only transfer is a little slower.

Check out the PERN project tutorial. It has a great coverage on the topic of building
the cable. My salutations to the author. There are also several other guides available.

4. I think your app is slowing my PC down, where's that damn driver??!!! I want it out!!
Plus some dude told me its dangerous to leave my ports open!!

Relax, this is a kernel mode driver, it only runs when an app calls it. When the app
exits, the driver is unloaded from RAM. Therefore your PC's ports are closed again. And 
only apps with the io.dll can call the driver, so even while the program is running, there 
will be no intrusions. So whatever is slowing your PC down, it isn't the driver.

If you wanna remove the driver, just look for io.sys in your windows\system32\drivers folder
and remove it. But i wouldn't't bother.


5. What on earth is BURST BOOT? Another guy told me it's great! What IS so great about it?

Okay this is a question asked too many times.......
But the answer is simple. BURST BOOT is a system designed by Martin Korth that allows you to
send a multiboot image REPLACING the old image that you have transfered PREVIOUSLY. To use this
system:-

i.   Put a burst boot code(like a backdoor, you know) in your code.
ii.  Send your multiboot image. Once you've finished checking it out....
iii. Send your multiboot image again.....WITHOUT reseting the GBA.

So, if youre smart, you should know that you need not RESET(turn off/on) the GBA, thus 
saving you time and a lot of hassle. You can modify your code, then send it again, 
and again.....


7. Why is this program running in DOS??? I want Windows GUI?!!??! I'm sick of the
twistedness of GBA dev tools!!!!!!!

A CLI program has the advantage of being used with batch files easily without much hassle.
Therefore a programmer who wants to test his program just after he has compiled his code 
just need to double click the batch file.

However, if you wanna see a GUI version, tell me. I can developed one thats twisted-free  ;)



=======================| Contact me |================================

This program was created by me, Vishnu P. Nambiar aka ZeroX as a contribution towards
a society that has helped me a lot, with all the guides, emulators, and stuff.

Remember, before contacting me, make sure you have check the forum for your answers, or it
is likely I might not answer, I've got a life. And yeah, NO SPAM...

Contact me : ZeroX at GBAdev forum.
             zero.vishnu {at} gmail dot com, i check this often.



=======================| Need Help |======================================

Contact me if you have info on:-
1. Tips on intergrating BURST BOOT
2. Save file grabbing tips....
3. Bugs.
4. Any help on converting the project to a modern Linux distro, Im too busy.
5. Advice on program interface.



=======================| Credits |================================

Thanks to
	  - Alexander aka arundel, for all his help and support. His gbacopy.asm was of GREAT
 	    help, but took a while to understand. His beta testing was also a great help. He's
        a great guy. Check out his gbacopy for Linux.
	  - Matt, for his c code for CRC routines.
	  - Martin Korth's extremely informative GBAtek document.
	  - Fred's great io.dll from geekhideout.com. This driver was most useful for my 
	    university projects.

