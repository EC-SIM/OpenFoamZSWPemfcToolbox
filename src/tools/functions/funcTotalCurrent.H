class funcTotalCurrent
{
    scalar Nernst_, Vcell_, R_, A_, i0_Cat_, conC_, C_, i0_An_, conA_, iLeakValue_;

    
public: 

    funcTotalCurrent(scalar Nernst, scalar Vcell, scalar R, scalar A,
					 scalar i0_Cat, scalar conC, scalar C, scalar i0_An,
					 scalar conA, scalar iLeakValue)
    :
    Nernst_(Nernst),
    Vcell_(Vcell),
    R_(R),
    A_(A),
    i0_Cat_(i0_Cat),
    conC_(conC),
    C_(C),
    i0_An_(i0_An),
    conA_(conA),
    iLeakValue_(iLeakValue)
    {}

    scalar operator()(const scalar& i) const
    {
	   return (Nernst_-(i*R_)-(A_*Foam::asinh((i+iLeakValue_)/(2*i0_Cat_)/conC_))
			   -Vcell_-(C_*Foam::asinh(i/(2*i0_An_)/conA_)));

	
    }

};




