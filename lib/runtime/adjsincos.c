// Adjust arguments too large for sin and cos
//
// These functions are called when the sin or cos instrinsic function is
// generated and the argument is outside the range of -2**64 .. 2**64.  In
// this case the sin or cos instruction fails and returns C2 set.  These
// functions do an explicit range reduction, then redo the operation.  Note
// that the result is essentially a random number since all accxuracy has been
// lost for arguments this big anyway!


static double twopi  = (3.141592653589793116 * 2);

// Adjust argument and do a sin

extern __declspec(naked) void _reduce()
{

    asm
    {
    	fld			twopi
    	fxch
    more:
    	fprem
    	fstsw		ax
    	sahf
    	jp			more
    	fstp		st(1)
    	ret
    }
}



