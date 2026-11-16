#ifndef __CONIO_H
#define __CONIO_H

/************************************************************************/
/*	Project...:	Win32-x86 Runtime Library								*/
/*	Name......:	conio.h													*/
/*	Purpose...:	direct hardware access function prototypes				*/
/*  Copyright.: Copyright © 1998-1999 Metrowerks, Inc.						*/
/************************************************************************/

# ifdef __cplusplus     
	extern "C" {
# endif

void _outp (unsigned short port, unsigned char val);
void _outpw (unsigned short port, unsigned short val);
void _outpd (unsigned short port, unsigned long val);
unsigned char _inp (unsigned short port);
unsigned short _inpw (unsigned short port);
unsigned long _inpd (unsigned short port);

/* Useful console I/O functions defined in conio.c */

void __initscr(void);
int  __kbhit(void);
int  __getch(void);
int  __getche(void);
void __clrscr(void);
void __gotoxy(int x, int y);
int  __wherex(void);
int  __wherey(void);
void __textattr(int newattr);
void __textcolor(int newcolor);
void __textbackground(int newcolor);

# ifdef __cplusplus     
   }
# endif
#endif