#include "stdio.h"
#include "conio.h"
#include "io.h"
#include "fstream"
#include "data.h"

using namespace std;

unsigned char filesize[2],no_header=0,test_flag=0;			//filesize transfer(1st and 2nd byte), no header flag(to know whether to send dummy header),beta_test flag
unsigned char rec[4]={0xFF,0xFF,0xFF,0xFF},dont_care=0;		//Received Data from GBA (used by Sendata32 func.),dont care input flag(for sendata func)
unsigned long filesize_32;									//file transfer function length
double bit_delay=0,unit_delay=11;							//delay bit transfer value(set here), delay unit transfer value (set here)
signed short port_address=0x378,filename=0;					//parellel port address,file name flag
unsigned long enc,CRC,rr,hh,cc,var_30,var_C = 0x0C387;		//Encrypyion and crc variables(all of them important)
ifstream game;												//gba file pointer, using ifstream function

void Usage(char *name);
inline void BeginTransfer();
void DelayLoop(double delay_count,char delay_type);
void QuitProgram();
void FileCalc();
char SendData(unsigned char x);
void SendData32(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Initialize();
void SendMainData();

void main(unsigned int count,char **arg)
{
	unsigned long x=0,y=0,f;
	short temp[4];

	printf ("\nxbooxp v1.2, GBA multiboot transfer software designed for Windows XP/2000.\nCopyright 2005 by Vishnu.\n\n");
	BeginTransfer();

	if (count<2)
		Usage(arg[0]);

	if (count>6)
	{
		printf ("Too many arguments.....Read intructions more carefully.\n\n");
		QuitProgram();
	}

	else
	{
		for (x=1; x<=(count-1) ;x++)							//Argument Check loop
		{
			if (arg[x][0]=='-')									//first check for -
			{
				if (arg[x][1]=='h')								//checking for -h
					no_header=1;

				else if (arg[x][1]=='d')						//checking for -d
				{
					if ((arg[x][2]>='1')&&(arg[x][2]<='9'))
					{
						bit_delay  = (arg[x][2]-48);			//offset bit delay
						unit_delay = unit_delay+(3*(arg[x][2]-48));
					}

					else
					{
						printf ("Invalid argument %s, if delay=1 then type -d1.\n\n",arg[x]);
						QuitProgram();
					}
				}

				else if (arg[x][1]=='p')						//checking for -p
				{
					for (y=0; y<3; y++)
					{
						if ((arg[x][y+2]>='0')&&(arg[x][y+2]<='9'))
							temp[y]=arg[x][y+2]-48;
						else if ((arg[x][y+2]>='A')&&(arg[x][y+2]<='F'))
							temp[y]=arg[x][y+2]-55;
						else
						{
							printf ("Invalid port address %s\nIf your port address is 0x278, then type -p278.\n\n",arg[x]);
							QuitProgram();
						}
					}
					port_address=(temp[0]<<8)+(temp[1]<<4)+temp[2];
				}

				else if (arg[x][1]=='t')						//checking for -t
					test_flag=1;

				else
				{
					printf ("Invalid argument %s\n\n",arg[x]);
					QuitProgram();
				}
			}

			else if (filename==0)								//Second Check for file.name
			{
				f=x;
				filename=1;
			}
			
			else												//Else is garbage
			{
				printf ("Invalid command %s\n\n",arg[x]);
				QuitProgram();
			}
		}

		if (filename==0)										//If no file name
		{
			printf ("No file specified.....\n\n");
			QuitProgram();
		}

		else if (filename==1)									//If file name exists, then...
		{
			game.open(arg[f], ios::in | ios::binary);
			if (game.fail())
			{
				printf ("Error opening file %s\n\n",arg[f]);
				QuitProgram();
			}
		}
	}

	//printf ("Header=%d, bit delay=%.3f, unit delay=%.3f, Port=%X, File=%s\n\n",no_header,bit_delay,unit_delay,port_address,arg[f]);
	FileCalc();
	Initialize();
	SendMainData();
	UnloadIODLL();
}

void Usage(char *name)
{
	printf ("Usage : %s yourfile.mb -argument1 -argument2 -argument3\n\n",name);
	printf ("Valid arguments are as follows :\n\n");
	printf ("   -h           Use this to upload the file without a valid header.\n");
	printf ("                NOTE : This will not modify the file, it will just upload a\n");
	printf ("                       standard header file.\n\n");
	printf ("   -d[value]    Put value here to increase delay, any value from 1 to 9,\n");
	printf ("                eg. -d3. Use this only if you are having transfer problems.\n\n");
	printf ("   -p[port]     Input your parellel port address, eg -p278. If no value is\n");
	printf ("                specified then the default port will be 0x378. Use CAPITAL\n");
	printf ("                letters for port addresses with alphabetical values,eg. -p3BC.\n\n");
	printf ("   -t           For testing whether a program can be sent without actually\n");
	printf ("                starting the program. This allows you to send another program\n");
	printf ("                without reseting the GBA. For testing purposes only.\n\n");
	QuitProgram();
}
inline void BeginTransfer()
{
	int x;

	x=LoadIODLL();
	if (x==1)
	{
		printf ("Error : io.dll is missing.\n\n");
		QuitProgram();
	}

	
	x=IsDriverInstalled();
	if (x==0)
	{
		printf ("io.dll has just been initialized. Please restart the program. This happens the\nfirst time the program is used.\n\n");
		QuitProgram();
	}

	PortOut(port_address+2,0x04);						//Idle begin, sets S0 high and SC high
}

void DelayLoop(double delay_count,char delay_type)		//delay function(timing purposes)
{
	unsigned char byte_low, byte_high;
	unsigned short temp, required_count, loop;

	if (delay_type==0)									//delay function using port io, stable but not accurate, better for minor small waitstates.
	{
		byte_low=delay_count;
		for (loop=0; loop<delay_count; loop++)
			byte_low=PortIn(port_address);
	}

	else if ((delay_type==1)&&(delay_count!=0))			//delay function using pc speaker timer, very accurate and stable, from 0.001ms to 54ms.
	{
		required_count = delay_count/(0.0008380958);
		byte_low=0x0B4;
		PortOut(0x043,byte_low);
		byte_low=0x0ff;
		PortOut(0x042,byte_low);
		byte_low=0x0ff;
		PortOut(0x042,byte_low);
		byte_low=0x01;
		PortOut(0x061,byte_low);

		for (loop=0; loop < required_count;)
		{
			byte_low = PortIn(0x042);
			byte_high = PortIn(0x042);
			temp=(byte_low+(byte_high<<8));
			loop = 0x0ffff-temp;
		}
		byte_low=0;
		PortOut(0x061,byte_low);
	}
}

void QuitProgram()
{
	printf ("Press any key to cotinue...");
	getch();
	UnloadIODLL();
	exit(0);
}

void FileCalc()
{
	unsigned int filesize_long,loop;
	char fchar[0x4000F];

	game.seekg(0,ios::end);
	filesize_long=game.tellg();							//Get file length
	game.seekg(0,ios::beg);
	game.read(fchar,filesize_long);
	filesize_32=(filesize_long + 0x0F) & 0xFFFFFFF0;	//File size realignment to xxxxx0h

	if (filesize_32<0x01C0)								//Adjust if too small
		filesize_32 = 0x01C0;

	if (filesize_32>0x40000)							//Quit if too big
	{
		printf ("File to be transfered is too big....\n\n");
		QuitProgram();
	}

	for (loop=0; loop<filesize_32; loop++)
	{
		if (loop<filesize_long)
			program[loop]=fchar[loop];
		else
			program[loop]=0;
	}
	game.close();

	filesize_long=((filesize_32-0xC0)>>2)-0x34;			//transfer length in 32 bits encoded (for length transfer protocol)
	filesize_32=(filesize_32-0xC0)/4;					//transfer length in 32 bits, after realignment (see SendMainData Function)
	filesize[0]=(filesize_long>>8)&(0x000000FF);		//put upper byte into universal variable filesize[0]
	filesize[1]=(filesize_long)&(0x000000FF);			//put lower byte into universal variable filesize[1]
}

char SendData(unsigned char x)							//Send char to GBA serial style 1/16/1
{
	int loop;
	unsigned char temp1, sendbit, temp2=0;
	const unsigned char	send=0x05;

	for(loop=7; loop>=0; loop--)
	{
		temp1=(~(x>>loop))&(0x01);					//1 or 0,then invert(damn inverted parellel port ctr reg)
		sendbit=send|(temp1<<1);

		PortOut((port_address+2),sendbit);			//send bit control port
		DelayLoop(bit_delay,0);						//wait

		temp1=sendbit&(0xFA);						//set clock high to read
		PortOut((port_address+2),temp1);			//then send
		DelayLoop(bit_delay,0);						//wait again
		
		if (dont_care==1)							//if dont care=1, then no need to read (faster transfer this way)
			temp2=0;

		else										//else, read read status port
		{
			temp1=PortIn((port_address+1));
			temp2=temp2|(((temp1>>6)&(0x01))<<(loop));
		}
	}
	
	return (temp2);
}

void SendData32(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	rec[0]=SendData(a);
	rec[1]=SendData(b);
	rec[2]=SendData(c);
	rec[3]=SendData(d);
	DelayLoop(unit_delay,0);
}

void Initialize()
{
	int loop,timeout=0;

	printf("Requesting Normal Mode...");

START:
	for (loop=0; !(((rec[0]==0x00)&&(rec[1]==0x00))|(rec[0]==0x72)); loop++)		//Wait for slave to become ready
	{
		if (loop>7)
		{
			DelayLoop(10,1);														//If failed wait 65ms
			loop=0;
			timeout++;
		}
		
		if (timeout>240)
		{
			printf("Timeout : Failed\n\nCheck if your GBA is on. Hold START + SELECT during the boot up process.\nElse, there is a problem with your cable.\n\n");
			QuitProgram();
		}
		
		SendData32(0,0,0x62,0x02);
	}

	for (loop=0; loop<47; loop++ )						//Send repeating 6200
	{
		SendData32(0,0,0x62,0x02);
		if (rec[0]!=0x72)								//Try again if failed, wait 62.5ms
		{
			DelayLoop(50,1);
			DelayLoop(12.5,1);
			goto START;
		}
	}

	SendData32(0,0,0x61,0x02);							//about to send header

	printf ("Done\nSending Header Data......");
	dont_care=1;
	for (loop=0; loop<0x60; loop++)						//Send header 0xC0 bytes, 0x60 times for 2bytes
	{
		if (no_header==0)
			SendData32(0,0,program[(loop*2)+1],program[(loop*2)]);

		else if (no_header==1)
		{
			if (loop<2)
				SendData32(0,0,program[(loop*2)+1],program[(loop*2)]);
			else
				SendData32(0,0,header[(loop*2)+1],header[(loop*2)]);
		}
	}
	dont_care=0;
	printf ("Done\n");

	SendData32(0,0,0x62,0x02);
	SendData32(0,0,0x62,0x02);							//xchange master slave info

	while (!(rec[0]==0x73))								//Wait until slave reply 73cc
		SendData32(0,0,0x63,0xF7);

	cc=rec[1];											//get the encyrption
	hh = ((cc&0x0FF)+0x20F)&0x0FF;						//
	enc = ((cc&0x0FF)<<8)|(0x0FFFF00F7);				//

	SendData32(0,0,0x64,hh);							//Handshake

	DelayLoop(50,1);									//Pause 1/16 sec
	DelayLoop(12.5,1);
}

void SendMainData()
{
	unsigned long data32, pos=0xC0, loop, bit, timeout=0, percent=0;

	SendData32(0,0,filesize[0],filesize[1]);										//transfer length
	rr = rec[1];
	rr = rr&0x0FF;

	printf ("Sending Main Data........  0%%");
	dont_care=1;
	for (loop=0; loop<filesize_32; loop++)											//Send Program
	{
		data32 = program[pos] + (program[pos+1] << 8) 
				+ (program[pos+2]<<0x10) + (program[pos+3] <<0x18);

		CRC = data32;																//CRC calculation
		for (bit = 0; bit<=31; bit++)
		{
			var_30 = var_C ^ CRC;
			var_C = var_C >> 1;
			CRC = CRC >> 1;
			if (var_30&0x01) 
				var_C = var_C^0x0C37B;
		}

		enc = (enc * 0x6F646573)+1;																		//Data encoding
		data32 = (enc ^ data32)^( ((~(pos+0x2000000))+1) ^ 0x43202F2F);

		SendData32(((data32>>0x18)&0x0FF),((data32>>0x10)&0x0FF),((data32>>8)&0x0FF),(data32&0x0FF));	//Send data 32bit at once
		pos = pos + 4;																					//realingn pos pointer, only for encoding calculation (see above)

		if (percent!=(((loop+1)*100)/filesize_32))
		{
			percent=((loop+1)*100)/filesize_32;
			printf ("\b\b\b\b%3d%%",percent);
		}
	}
	dont_care=0;
	printf("\b\b\b\bDone\nCRC Check................");

	SendData32(0,0,0x00,0x65);
	while ((!((rec[0]==0x00)&&(rec[1]==0x75)))&&(timeout<1000))
	{
		SendData32(0,0,0x00,0x65);												//while no response keep sending
		timeout++;
		if (timeout==1000)
			printf ("Timeout : ");
	}

	SendData32(0,0,0x0,0x66);													//transfer CRC follows

	CRC = ((rr<<8)|0x0FFFF0000)+hh;												//additional CRC calculation
	for (bit = 0 ; bit<=31 ; bit++)
	{
		var_30 = var_C ^ CRC;
		var_C  = var_C>>1;
		CRC = CRC>>1;
		if (var_30 & 0x01) 
			var_C = var_C ^ 0x0C37B;
	}

	SendData32(0,0,(((var_C>>8)&0x0FF)+test_flag),var_C&0x0FF);				//CRC transfer,modified with test flag

	CRC=rec[0];
	CRC=(CRC<<8)+rec[1];

	if (CRC==var_C)
		printf ("Passed\n\n");

	if (CRC!=var_C)
	{
		printf ("Failed\n\n");
		bit=bit_delay+1;
		if (bit_delay==9)
			bit=9;
		printf ("CRC check has failed. Reasons are probably:\n1.If the Nintendo image appears garbled, thats because your file has\n  an invalid header. Try using the -h argument.\n2.Try adjusting the delay by using this argument -d%d. Keep trying till\n  you get the right value.\n3.Make sure you cable is not too long and is connected properly.\n4.The best solution is to just reset the GBA and try again.\n\n",bit);
		QuitProgram();
	}
}
