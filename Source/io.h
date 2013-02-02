/*
	This file is part of xbooxp. It makes use of io.dll, which is free, and
	available at http://www.geekhideout.com/iodll.shtml

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
*/
#include "windows.h"

typedef void (WINAPI *PORTOUT) (short int Port, char Data);
typedef void (WINAPI *PORTWORDOUT)(short int Port, short int Data);
typedef void (WINAPI *PORTDWORDOUT)(short int Port, int Data);
typedef char (WINAPI *PORTIN) (short int Port);
typedef short int (WINAPI *PORTWORDIN)(short int Port);
typedef int (WINAPI *PORTDWORDIN)(short int Port);
typedef void (WINAPI *SETPORTBIT)(short int Port, char Bit);
typedef void (WINAPI *CLRPORTBIT)(short int Port, char Bit);
typedef void (WINAPI *NOTPORTBIT)(short int Port, char Bit);
typedef short int (WINAPI *GETPORTBIT)(short int Port, char Bit);
typedef short int (WINAPI *RIGHTPORTSHIFT)(short int Port, short int Val);
typedef short int (WINAPI *LEFTPORTSHIFT)(short int Port, short int Val);
typedef short int (WINAPI *ISDRIVERINSTALLED)();

extern PORTOUT PortOut;
extern PORTWORDOUT PortWordOut;
extern PORTDWORDOUT PortDWordOut;
extern PORTIN PortIn;
extern PORTWORDIN PortWordIn;
extern PORTDWORDIN PortDWordIn;
extern SETPORTBIT SetPortBit;
extern CLRPORTBIT ClrPortBit;
extern NOTPORTBIT NotPortBit;
extern GETPORTBIT GetPortBit;
extern RIGHTPORTSHIFT RightPortShift;
extern LEFTPORTSHIFT LeftPortShift;
extern ISDRIVERINSTALLED IsDriverInstalled;

extern int LoadIODLL();

PORTOUT PortOut;
PORTWORDOUT PortWordOut;
PORTDWORDOUT PortDWordOut;
PORTIN PortIn;
PORTWORDIN PortWordIn;
PORTDWORDIN PortDWordIn;
SETPORTBIT SetPortBit;
CLRPORTBIT ClrPortBit;
NOTPORTBIT NotPortBit;
GETPORTBIT GetPortBit;
RIGHTPORTSHIFT RightPortShift;
LEFTPORTSHIFT LeftPortShift;
ISDRIVERINSTALLED IsDriverInstalled;

HMODULE hio;

void UnloadIODLL() {
	FreeLibrary(hio);
}

int LoadIODLL() {
	hio = LoadLibrary("io");
	if (hio == NULL) return 1;

	PortOut = (PORTOUT)GetProcAddress(hio, "PortOut");
	PortWordOut = (PORTWORDOUT)GetProcAddress(hio, "PortWordOut");
	PortDWordOut = (PORTDWORDOUT)GetProcAddress(hio, "PortDWordOut");
	PortIn = (PORTIN)GetProcAddress(hio, "PortIn");
	PortWordIn = (PORTWORDIN)GetProcAddress(hio, "PortWordIn");
	PortDWordIn = (PORTDWORDIN)GetProcAddress(hio, "PortDWordIn");
	SetPortBit = (SETPORTBIT)GetProcAddress(hio, "SetPortBit");
	ClrPortBit = (CLRPORTBIT)GetProcAddress(hio, "ClrPortBit");
	NotPortBit = (NOTPORTBIT)GetProcAddress(hio, "NotPortBit");
	GetPortBit = (GETPORTBIT)GetProcAddress(hio, "GetPortBit");
	RightPortShift = (RIGHTPORTSHIFT)GetProcAddress(hio, "RightPortShift");
	LeftPortShift = (LEFTPORTSHIFT)GetProcAddress(hio, "LeftPortShift");
	IsDriverInstalled = (ISDRIVERINSTALLED)GetProcAddress(hio, "IsDriverInstalled");

	atexit(UnloadIODLL);

	return 0;
}
