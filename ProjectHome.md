# Introduction #

Xbooxp is a nifty software to upload homebrew apps into your Game Boy Advance (GBA) using an xboo cable. It is a multiboot transfer software designed with WinXP/2000 in mind, but should work in Win98/95. This program will not work in 16-bit DOS mode.

The xboo cable is cheap and easy to build. Costs about $5 to build. Plus you can send multiboot images of up to 256kB, and upload speeds are fast. This program can send a 256kB multiboot image in about 8.1-8.5 seconds on an AMD 2100++ CPU. With the delay setting, its a little slower.

Even though this program runs in DOS, it is made for WinXP/2000. Usage of batch files are recommended. This program does not require other software like _Userport_ to run, it's pretty much independent. It has a kernel mode driver which allows it to access the parallel port and the PC speaker(for timing), that unloads itself after exit. This is much safer because there will be no security holes in your PC.

This is an old project, I uploaded this in Google Projects to get the code out there for hackers to play with it. I no longer actively support this project, because I mainly use Linux these days for my research and development work.

# Usage #

Run.bat is a batch file included for your ease. Edit the file with notepad. To run xbooxp, double click the file. To edit the file, right click and choose edit. Make sure your compiled .mb file is in the same directory as xbooxp. Or you can move xbooxp into your program source directory.

To use xbooxp, you will need to edit this file to suit you purposes. This is a basic explanation of the usage:

```
xbooxp yourfile.mb -argument
```

To see the help screen, just type xbooxp, and you will see a list of arguments and its usage. For example, if your parallel port address is 0x278, you want delay=2, and you want to upload a multiboot file named _matrix.mb_ without a valid header (too lazy to use GBAFIX), just put this line in run.bat:

```
xbooxp matrix.mb -278 -h -d2
```

Simple as pie. Or, you can just type the command in the command prompt.


# FAQ #

  1. Why use an xboo cable for dev instead of a flash cart?
    * Simply because there is really no faster way to test your code on hardware, and having a nice debug console is pretty slick too.
  1. How to know your parallel port address?
    1. Right click on My Computer and chose properties. Then click on the Hardware tab, and choose Device Manager.
    1. Look for Ports(Com & LTP), and expand this branch. Then right click on printer port and choose properties.
    1. Go to the Resources Tab, and look at the first IO-Range value, if 378-3FF that means your port is 0x378. This is the most common value.
  1. What is a batch file?
    * A batch file is a file with .bat extension. It is a set of CLI commands written in sequence. To get a better idea on how a batch file works, check out run.bat file included. You can edit it using notepad. The **@ECHO OFF** command means that whatever typed will not be displayed in the screen. To run the batch file, just double-click it.
  1. How do i build a xboo cable?
    * It's easy. To build one, check the gbadev.org website. You only need to connect three wires, four IF you want to use high speed BURST BOOT. You can still use BURST BOOT with only three wires, only transfer is a little slower. Check out the [PERN project tutorial](http://www.drunkencoders.com/tutorials/GBA/day_1.html). It has a great coverage on the topic of building the cable. My salutations to the author. There are also several other guides available.
  1. I think your app is slowing my PC down, where's that driver? I want it out! Isn’t it dangerous to leave my ports open?
    * Relax, this is a kernel mode driver, it only runs when an app calls it. When the app exits, the driver is unloaded from RAM. Therefore your PC's ports are closed again. And only apps with the io.dll can call the driver, so even while the program is running, there will be no intrusions. So whatever is slowing your PC down, it isn't the driver. If you wanna remove the driver, just look for _io.sys_ in your windows\system32\drivers folder and remove it.
  1. What is BURST BOOT?
    * BURST BOOT is a system designed by Martin Korth that allows you to send a multiboot image REPLACING the old image that you have transferred PREVIOUSLY. To use this system:-
      * Place the burst boot code section (like a backdoor) in your program.
      * Send your compiled multiboot image.
      * Once you've finished checking out your program, edit your code and send your recompiled multiboot image again. No need to reset the GBA.
    * So, you need not RESET(turn off/on) the GBA, thus saving you time and a lot of hassle. You can modify your code, then send it again, and again, and again.
  1. Why is this program running in DOS? I want a GUI! I'm sick of the twisted-ness of GBA dev tools!
    * A CLI program has the advantage of being used with batch files easily without much hassle. Therefore a programmer who wants to test his program just after he has compiled his code just needs to double click the batch file.

# Contact me #

This program was created by me, Vishnu P. Nambiar aka ZeroX as a contribution towards
a society that has helped me a lot, with all the guides, emulators, and stuff. Remember, before contacting me, make sure you have checked the forum for your answers. I'm quite busy. Anyway, contact me at:
  * zero.vishnu {at} gmail dot com, i check this often.
  * ZeroX at the GBAdev forums.

# Credits #

Thanks to:
  * Alexander aka arundel, for all his help and support. His _gbacopy.asm_ was of great help, but took me a while to understand. His beta testing was also a great help. Check out his multiboot app _gbacopy_ for Linux.
  * Matt, for his C/C++ code for CRC routines.
  * Martin Korth's extremely informative GBAtek document.
  * Fred's great io.dll from geekhideout.com. This driver was also useful for my simple undergraduate projects.

# License #

xbooxp is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

xbooxp is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with xbooxp. If not, see <http://www.gnu.org/licenses/>.

xbooxp makes use of io.dll, which is free, and available at http://www.geekhideout.com/iodll.shtml