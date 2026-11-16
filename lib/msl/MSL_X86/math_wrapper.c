/*  Metrowerks Standard Library  Version 2.2  1997 October 17  */
#ifndef _No_Floating_Point 
#include <errno.h>
#include <math.h>
static double ONE=1.0;
static double HALF=.5;
static long  QNAN=0x7fffffff;
static short TRUNC=0x0f7f ;
static short TEMP ;
#pragma k63d_calls off

#ifdef __cplusplus
 extern "C" {
#endif
__declspec(naked) double sqrt(double)
{
asm 
 {
    fld qword ptr[esp + 4]
    ftst
	fstsw ax
	test ax,0x0100
	jnz seterrno
    fsqrt
    ret
seterrno:
    fstp st
 }
errno=EDOM ;
return NAN ;
/* asm 
 {
	fld QNAN
    ret
 }
*/
}
 
__declspec(naked) double log(double )
{
 asm
 {
  fld qword ptr[esp + 4]
  ftst
  fstsw ax
  and ax,0x4300
  jnz seterrno
  fld1      	; st=1.0 st(1)=x
  fxch
  fyl2x	        ; st=1.0*log (x)
  fldln2	    ;           2
  fmul
  ret 
seterrno: 
  fstp st  
 }
errno=EDOM ;
return NAN ;
/*asm 
 {
    
	fld QNAN
    ret
 }
 */
}

__declspec(naked)  double log10(double  )
{
 
 asm
 {
  fld qword ptr[esp + 4]
  ftst
  fstsw ax
  and ax,0x4300
  jnz seterrno
  fld1    ; st=1.0 st(1)=x
  fxch
  fyl2x		           ; st=1.0*log (x)
  fldlg2               ;           2
  fmul
  ret
seterrno: 
  fstp st 
 }
errno=EDOM ;
return NAN ;
/*
asm 
 {
	fld QNAN
    ret
 }
 */
}

double pow(double x , double y)
{
  short TEMP;
  long  LONGTEMP;
  if(x > 0)
  { 
   asm
   {
 	fld y
 	fld x 
	fyl2x	        ; st=y*log2(x) overwrites st(1) and pops stack
	fld st	        ; backup I+f
	frndint	        ; st=I st(1)=y*log2(x)
	fxch
	fsub st,st(1)	; st(1)-st=st=f overwrites st(1)
	f2xm1
	fld1 
	fadd			; st=2^f st(1)=2^f-1 st(2)=I
	fscale
	fstp st(1)	
	fstp x
   }
  return x; 
 }
 
 if((x==0.0 && y <= 0) || (x < 0 && (rint(y) != y)))
 {
  errno=EDOM;
  if(x == 0.0 && y < 0.0) return INFINITY ;
  else return NAN ;
 }
 /* if we're here we know the base is not positive and exponent integral */
 
asm
{
 	fld y
 	fld x 
	mov TEMP,1		   ; used at end
	ftst
	fstsw ax
	and ax,0x4000      ; c3=0 iff base (x) is zero(nan case taken care of by above)
    			       ; zf=0 if base is zero
	jz negative_base
	mov TEMP, 0
	jmp zero_base
negative_base:
	;; test for even or odd
	fxch
	fld st
	fistp LONGTEMP
	mov al, byte ptr[LONGTEMP]
    test al,0x01
	fxch
	fchs			; base,exp combo legit so we change
				    ; sign of base to make it >0
	jz normal_pow
	mov TEMP,-1
normal_pow:
	fyl2x			; st=y*log2(x) overwrites st(1) and pops stack
	fld st			; backup I+f
	frndint			; st=I st(1)=y*log2(x)
	fxch
	fsub st,st(1)	; st(1)-st=st=f overwrites st(1)
	f2xm1
	fld1 
	fadd			; st=2^f st(1)=2^f-1 st(2)=I
	fscale	
zero_base:	
	fstp st(1)	
	fimul TEMP		; change sign of result if exponent is odd
	fstp x
	
}
return x;
}
/* rounding functions */

double nearbyint(double x)
{
 char temp[28] ; /* 28 bytes of storage for fp environment */
 asm
 {
  fld x
  frndint		 ; rounded according to current rounding mode
  fnstenv temp
  and    word ptr[temp+4],0xffcf ; clears inexact exception
  fldenv temp
  fstp x         
 }
  return x;
}

__declspec(naked) double round(double )
{
/* short TRUNC=0x0f7f ;
 short TEMP ;
 if(x < 0) x-=.5;
 else x+=.5;
 */
 asm
 {
  fld qword ptr[esp+4]
  ftst
  fstsw ax
  and ax,0x0100
  jne SUBTRACT
  fadd HALF
  jmp GO
SUBTRACT:
  fsub HALF
GO:    
  fstcw TEMP
  fldcw TRUNC
  frndint
  fldcw TEMP
  ret
 }
// return ;
}

__declspec(naked) long roundtol(double )
{

 asm
 {
  fld qword ptr[esp+4]
  ftst
  fstsw ax
  and ax,0x0100
  jne SUBTRACT  ; yes I'm aware that c0 gets set by ftst when x is a nan
  fadd HALF     ; but whether you add or substract .5 from the nan is irrelevant
  jmp GO
SUBTRACT:
  fsub HALF
GO:    
  fstcw TEMP
  fldcw TRUNC
  frndint
  fistp dword ptr[esp+4]   ; this bashes the original argument which is o.k. at this point
  mov eax,dword ptr[esp+4] ; since it was a copy
  fldcw TEMP
  ret
 }

}

__declspec(naked)  long rinttol(double )
{
 asm
 {
  fld qword ptr[esp+4]
  frndint
  fistp dword ptr[esp+4]   ; this bashes the original call stack argument
                           ; which is o.k. at this point
  mov eax,dword ptr[esp+4] 
  ret
 } 

}

double asinh(double x )
{
if(fabs(x) < 2.2204460492503131e-016) return x; // error is O(10^-48) 
else
{
 asm
  {
	fld x 
	fld st
	fmul st,st(1)		; st=x^2 ,  st(1)=x
	fadd qword ptr[ONE]	; st=x^2 +1 st(1)=x
	fsqrt
	fadd    			; st= x + sqrt(x^2 + 1)
	fld1
	fxch
	fyl2x
	fldln2
	fmul
	fstp x
  } 
	
}
return x;
}

double atanh(double x )
{

if(fabs(x) > 1.0)
{ errno=EDOM;
  return NAN;
} else if(fabs(x) < 2.2204460492503131e-016) return x; // error is O(10^-48) 

asm
{
 fld x
 fld st
 fsub qword ptr[ONE]  ; st=(x-1), st(1)=x, st2=1
 fchs
 fxch
 fadd qword ptr[ONE]
 fld  qword ptr[ONE]
 fxch
 fyl2x                ; st1*ln(st)
 fldln2
 fmul                 
 fxch
 fld  qword ptr[ONE]
 fxch
 fyl2x
 fldln2
 fmul
 fsubr st,st(1)
 fmul qword ptr[HALF]
 fstp st(1)
 leave
 ret
}

}

__declspec(naked) double acosh(double )
{

  asm
  {
   fld  qword ptr[esp + 4] ; x
   fcom ONE
   fstsw ax
   sahf
   jb seterrno
   fld st
   fmul st,st(1)
   fsub ONE
   fsqrt
   fadd
   fld1
   fxch
   fyl2x
   fldln2
   fmul
   ret
seterrno:   
   fstp st
  }
errno=EDOM ;
return NAN ;

}


__declspec(naked)  double copysign(double , double)
{
   asm
   {
     fld qword ptr[esp+4]       ; load x
     mov eax, dword ptr[esp+16]	; just want the first byte of y
     mov ecx, dword ptr[esp+8]  ; sign of x
     xor eax,ecx
     and eax,0x80000000
	 jz done
	 fchs                       ; change sign of x only when 
	                            ; sign(x) != sign(y)
done:
     ret	 
   }
    
}

__declspec(naked)  double fmax ( double , double  )
 {
	asm
	{
	 fld qword ptr[esp+4]
	 fld qword ptr[esp+12]
	 fcom 
	 fstsw ax
	 shr ax,8                 ; don't really need this instruction
	 and eax,0x00000045       ; makes it easier for me to debug
	 jz done_fmax             ; st=maximum
	 fxch
	 cmp ax, 0x0040
	 jle done_fmax
	 fxam                     ; one or more operand at this point is 
	 fstsw ax                 ; guaranteed to be a nan
	 shr ax,8
	 and eax, 0x00000045
	 cmp eax,1
	 jne done_fmax
	 fxch
done_fmax:
     fstp st(1)	 
     ret
	}

}
__declspec(naked)  double fmin ( double , double )
 {
   asm
	{
	 fld qword ptr[esp+4]
	 fld qword ptr[esp+12]
	 fcom 
	 fstsw ax
	 shr ax,8
	 and ax,0x0045
	 cmp eax,1
	 je done_fmin             
	 fxch
	 cmp ax, 0x0040      ; st==st(1) if ax=4
	 jle done_fmin       ; ax==0 -> st > st(1) originally but was 
	                     ; exchanged via fxch
	 fxam                ; one or more operand at this point is 
	 fstsw ax            ; guaranteed to be a nan
	 shr ax,8
	 and eax, 0x00000045
	 cmp eax,1
	 jne done_fmin
	 fxch
done_fmin:
     fstp st(1)	 
     ret
	}

}

__declspec(naked)  double fdim ( double , double  )
 {
 asm
	{
	 fld qword ptr[esp+4]
	 fld qword ptr[esp+12]
	 fsub                     ; st=x-y  note: if either x or y is a nan
	                          ; then the difference will also be a nan
	 fldz
	 fxch
	 fcom 
	 fstsw ax
	 shr ax,8
	 and ax,0x0045
	 jz done_fdim             ; st=maximum
	 fxam                     ; one or more operand at this point is 
	 fstsw ax                 ; guaranteed to be a nan
	 shr ax,8
	 and eax, 0x00000045
	 cmp eax,1
	 je done_fdim
	 fxch
done_fdim:
     fstp st(1)                   
     ret
	}

#ifdef __cplusplus
 }
#endif
}
#pragma k63d_calls reset

#endif /* ifndef _No_Floating_Point */ 
/* mf-- 040598-- updated fmax,fmin,fdim to C9X to return a non-nan value when 
                 exactly one argument is a nan.
   mf-- 042098-- fixed round and roundtol     
   mf-- 060898-- performance enhancements, __declspec(naked)
                 whenever possible(shrinks codesize by about 10% when combined
                 with other minor fixes(elimination of local variables etc.)  
   mf-- 061098   fixed bug in acosh(was using fmul instead of fmul st,st(1)(i.e. was popping)				 
   mf-- 071098   rewrote copysign
   mf-- 071398   bug in asinh(wasn't restoring ebp)
   mf-- 111398   log10 wasn't popping argument from fp stack when errno occured
*/                 
