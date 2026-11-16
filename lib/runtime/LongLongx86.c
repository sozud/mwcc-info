/************************************************************************/
/*	Project...:	Standard C++ Library									*/
/*	Name......:	LongLong68K.c											*/
/*	Purpose...:	68K 64-bit integer arithmetic functions					*/
/*  Copyright.: ©Copyright 1996 by metrowerks inc						*/
/************************************************************************/

#include <stddef.h>

#if __cplusplus
extern "C" {
#endif

extern __stdcall long long _rt_mul64(long long left,long long right);
extern __stdcall long long _rt_divu64(long long left,long long right);
extern __stdcall long long _rt_divs64(long long left,long long right);
extern __stdcall long long _rt_modu64(long long left,long long right);
extern __stdcall long long _rt_mods64(long long left,long long right);
extern __stdcall long long _rt_shl64(long long left,long long count);
extern __stdcall long long _rt_shru64(long long left,long long count);
extern __stdcall long long _rt_shrs64(long long left,long long count);
extern __stdcall int _rt_cmpu64(long long left,long long right);
extern __stdcall int _rt_cmps64(long long left,long long right);
extern __stdcall long long _rt_rotl64(unsigned long long left,long long count);
extern __stdcall long long _rt_rotr64(unsigned long long left,long long count);

#if __cplusplus
}
#endif

/************************************************************************/
/* Purpose..: Compute result = left * right								*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/

extern long long __stdcall __declspec(naked)
_rt_mul64(long long left, long long right)
{
	asm
	{
		mov		edx, [esp+8]		// multiplicand-hi
		mov		ecx, [esp+16]		// multiplier-hi
		or		edx, ecx			// one operand >= 2^32 ?
		mov		edx, [esp+12]		// multiplier-lo
		mov		eax, [esp+4]		// multiplicand-lo
		jnz		twomul				// yup, need two multiplies
		mul		edx					// multiplicand-lo * multiplier-lo
		ret		16					// done, return to caller
	twomul:
		imul	edx, [esp+8]		// p3_lo = multiplicand-hi*multiplier-lo
		imul	ecx, eax			// p2_lo = multiplier-hi*multiplicand-lo
		add		ecx, edx			// p2_lo + p3_lo
		mul		dword ptr [esp+12]	// p1=multiplicand_lo*multiplier-lo
		add		edx, ecx			// p1+p2lo+p3_lo = result in EDX:EAX
		ret		16					// done, return to caller
	}
}

/************************************************************************/
/* Purpose..: Compute result = left / right (unsigned)					*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/

extern long long __stdcall __declspec(naked)
_rt_divu64(long long dividend,long long divisor)
{
	asm
	{
		push	ebx					// save ebx as per calling convention
		mov		ecx, [esp+20]		// divisor_hi
		mov		ebx, [esp+16]		// divisor_lo
		mov		edx, [esp+12]		// dividend_hi
		mov		eax, [esp+8]		// dividend_lo
		test	ecx, ecx			// divisor > 2^32-1?
		jnz		big_divisor			// yes, divisor > 32^32-1
		cmp		edx, ebx			// only one division needed? (ecx = 0)
		jae		two_divs			// need two divisions
		div		ebx					// eax = quotient_lo
		mov		edx, ecx			// edx = quotient_hi = 0 (quotient in edx:eax)
		pop		ebx					// restore ebx as per calling convention
		ret		16					// done, return to caller
	two_divs:
		mov		ecx, eax			// save dividend_lo in ecx
		mov		eax, edx			// get dividend_hi
		xor		edx, edx			// zero extend it into edx:eax
		div		ebx					// quotient_hi in eax
		xchg	eax, ecx			// ecx = quotient_hi, eax = dividend_lo
		div		ebx					// eax = quotient_lo
		mov		edx, ecx			// edx = quotient_hi (quotient in edx:eax)
		pop		ebx					// restore ebx as per calling convention
		ret		16					// done, return to caller
	big_divisor:
		push	edi					// save edi as per calling convention
		mov		edi, ecx			// save divisor_hi
		shr		edx, 1				// shift both divisor and dividend right
		rcr		eax, 1				// by 1 bit
		ror		edi, 1
		rcr		ebx, 1
		bsr		ecx, ecx			// ecx = number of remaining shifts
		shrd	ebx, edi, cl		// scale down divisor and dividend
		shrd	eax, edx, cl		// such that divisor is less than
		shr		edx, cl				// less than 2^32 (i.e. fits in ebx)
		rol		edi, 1				// restore original divisor_hi
		div		ebx					// compute quotient
		mov		ebx, [esp+12]		// dividend_lo
		mov		ecx, eax			// save quotient
		imul	edi, eax			// quotient * divisor hi-word (low only)
		mul		dword ptr [esp+20]	// quotient * divisor lo-word
		add		edx, edi			// edx:eax = quotient * divisor
		sub		ebx, eax			// dividend_lo - (quot.*divisor)_lo
		mov		eax, ecx			// get quotient
		mov		ecx, [esp+16]		// dividend_hi
		sbb		ecx, edx			// subtract divisor * quot. from dividend
		sbb		eax, 0				// adjust quotient if remainder negative
		xor		edx, edx			// clear hi-word of quot(eax<=ffffffffh)
		pop		edi					// restore edi as per calling convention
		pop		ebx					// restore ebx as per calling convention
		ret		16					// done, return to caller
	}
}

/************************************************************************/
/* Purpose..: Compute result = left / right (signed)					*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern long long __stdcall _rt_divs64(long long left,long long right)
{
    long long result;

	// only check top 4 bytes -- we don't need full quad to check sign
	if (*((long *)&left + 1) < 0)
	{
		left = -left;
		if (*((long *)&right + 1) < 0)
		{	//	left<0 / right<0
			right = -right;
			result = _rt_divu64(left,right);
		}
		else
		{	//	left>=0 / right<0
			result = - _rt_divu64(left,right);
		}
	}
	else
	{
		if (*((long *)&right + 1) < 0)
		{	//	left<0 / right>=0
			right = -right;
			result = - _rt_divu64(left,right);
		}
		else
		{	//	left>=0 / right>=0
			result = _rt_divu64(left,right);
		}
	}
	return result;
}

/************************************************************************/
/* Purpose..: Compute result = left % right (unsigned)					*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern long long __stdcall __declspec(naked)
_rt_modu64(long long left,long long right)
{
	asm
	{
		push	ebx				//save ebx as per calling convention
		mov		ecx, [esp+20]	// divisor_hi
		mov		ebx, [esp+16]	// divisor_lo
		mov		edx, [esp+12]	// dividend_hi
		mov		eax, [esp+8]	// dividend_lo
		test	ecx, ecx		// divisor > 2^32-1?
		jnz		r_big_divisor	// yes, divisor > 32^32-1
		cmp		edx, ebx		// only one division needed? (ecx = 0)
		jae		r_two_divs		// need two divisions
		div		ebx				// eax = quotient_lo
		mov		eax, edx		// eax = remainder_lo
		mov		edx, ecx		// edx = remainder_hi = 0
		pop		ebx				// restore ebx as per calling convention
		ret		16
	r_two_divs:
		mov		ecx, eax		// save dividend_lo in ecx
		mov		eax, edx		// get dividend_hi
		xor		edx, edx		// zero extend it into edx:eax
		div		ebx				// eax = quotient_hi, edx = intermediate remainder
		mov		eax, ecx		// eax = dividend_lo
		div		ebx				// eax = quotient_lo
		mov		eax, edx		// eax = remainder_lo
		xor		edx, edx		// edx = remainder_hi = 0
		pop		ebx				// restore ebx as per calling convention
		ret		16				// done, return to caller
	r_big_divisor:
		push	edi				// save edi as per calling convention
		mov		edi, ecx		// save divisor_hi
		shr		edx, 1			// shift both divisor and dividend right
		rcr		eax, 1			//  by 1 bit
		ror		edi, 1
		rcr		ebx, 1
		bsr		ecx, ecx		// ecx = number of remaining shifts
		shrd	ebx, edi, cl	// scale down divisor and dividend such
		shrd	eax, edx, cl	//  that divisor is less than 2^32
		shr		edx, cl			//  (i.e. fits in ebx)
		rol		edi, 1			// restore original divisor (edi:esi)
		div		ebx				// compute quotient
		mov		ebx, [esp+12]	// dividend lo-word
		mov		ecx, eax		// save quotient
		imul	edi, eax		// quotient * divisor hi-word (low only)
		mul		dword ptr [esp+20] // quotient * divisor lo-word
		add		edx, edi		// edx:eax = quotient * divisor
		sub		ebx, eax		// dividend_lo - (quot.*divisor)Ðlo
		mov		ecx, [esp+16]	// dividend_hi
		mov		eax, [esp+20]	// divisor_lo
		sbb		ecx, edx		// subtract divisor * quot. from dividend
		sbb		edx, edx		// (remainder < 0)? 0xffffffff : 0
		and		eax, edx		// (remainder < 0)? divisor_lo : 0
		and		edx, [esp+24]	// (remainder < 0)? divisor_hi : 0
		add		eax, ebx		// remainder += (remainder < 0)?
		adc		edx, ecx		// divisor : 0
		pop		edi				// restore edi as per calling convention
		pop		ebx				// restore ebx as per calling convention
		ret		16				// done, return to caller
	}
}

/************************************************************************/
/* Purpose..: Compute result = left % right (signed)					*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern long long __stdcall _rt_mods64(long long left,long long right)
{
    long long result;

	if (*((long *)&left + 1) < 0)
	{
		left = -left;
		if (*((long *)&right + 1) < 0)
		{	//	left<0 / right<0
			right = -right;
			result = _rt_modu64(left,right);
		}
		else
		{	//	left<0 / right>=0
			result = _rt_modu64(left,right);
		}
		result = -result;
	}
	else // left >= 0
	{
		if (*((long *)&right + 1) < 0)
		{	//	left>=0 / right<0
			right = -right;
			result = _rt_modu64(left,right);
		}
		else
		{	//	left>=0 / right>=0
			result = _rt_modu64(left,right);
		}
	}
	return result;
}

/************************************************************************/
/* Purpose..: Compute result = left << (right % 64)						*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: shift count												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern long long __stdcall _rt_shl64(long long left,long long count)
{
	asm
	{
		mov		eax, left
		mov		edx, left+4
		mov		cl, byte ptr count
		and		cl, 0x3f

		// check for too big a shift

		cmp		cl, 32
		jb		do_shift

		mov		edx, eax
		xor		eax, eax

	do_shift:
		shld	edx, eax, cl
		shl		eax, cl

	done:
	}
}

/************************************************************************/
/* Purpose..: Compute result = left >> (right % 64) (unsigned)			*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: shift count												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern long long __stdcall _rt_shru64(long long left,long long count)
{
	asm
	{
		// get the result address and the 64 bit operand

		mov		eax, left
		mov		edx, left+4
		mov		cl, byte ptr count
		and		cl, 0x3f

		// check for too big a shift

		cmp		cl, 32
		jb		do_shift

		mov		eax, edx
		xor		edx, edx

	do_shift:
		shrd	eax, edx, cl
		shr		edx, cl

	done:
	}
}

/************************************************************************/
/* Purpose..: Compute result = left >> (right % 64) (signed)			*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: shift count												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern long long __stdcall _rt_shrs64(long long left,long long count)
{
	asm
	{
		mov		eax, left
		mov		edx, left+4
		mov		cl, byte ptr count
		and		cl, 0x3f

		// check for too big a shift

		cmp		cl, 32
		jb		do_shift
		mov		eax, edx
		sar		edx, 31

	do_shift:
		shrd	eax, edx, cl
		sar		edx, cl

	done:
	}
}

/************************************************************************/
/* Purpose..: Compare left and right (unsigned)							*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: -1 : left < right; 0: left == right; 1: left > right		*/
/************************************************************************/
extern __declspec(naked) int __stdcall _rt_cmpu64(long long left,long long right)
{
	asm
	{
		mov		eax, 8[esp]
		cmp		eax, 16[esp]
		jne		done

		mov		eax, 4[esp]
		cmp		eax, 12[esp]

	done:
		ret		16
	}
}

/************************************************************************/
/* Purpose..: Compare left and right (unsigned)							*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: -1 : left < right; 0: left == right; 1: left > right		*/
/************************************************************************/
extern __declspec(naked) int __stdcall _rt_cmps64(long long left,long long right)
{
	asm
	{
		mov		eax, 8[esp]
		cmp		eax, 16[esp]
		jne		done

		mov		eax, 4[esp]
		cmp		eax, 12[esp]
		je		done
		ja		high

		mov		eax, 0
		cmp		eax, 1
		ret		16

	high:
		mov		eax, 1
		cmp		eax, 0

	done:
		ret		16
	}
}

/************************************************************************/
/* Purpose..: Compute result = left ROTL right							*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: rotate count												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern long long __stdcall _rt_rotl64(unsigned long long left, long long count)
{
	return (left << count) | (left >> (64 - count));
}

/************************************************************************/
/* Purpose..: Compute result = left ROTR right							*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: rotate count												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern long long __stdcall _rt_rotr64(unsigned long long left, long long count)
{
	return (left << (count - 64)) | (left >> count);
}
