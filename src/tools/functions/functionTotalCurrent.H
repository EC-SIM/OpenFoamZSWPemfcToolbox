class functionTotalCurrent
{
    scalar i_, i0_, A_, B_; //changed (K.W 2020)
    
public: 
//changed (K.W 2020)
    functionTotalCurrent(scalar i, scalar i0, scalar A, scalar B)
    :
    i_(i),
    i0_(i0),
    A_(A),
    B_(B)
    {}

    scalar operator()(const scalar& eta) const
    //scalar evaluate(const scalar& eta) const
    {
        return i_ - i0_*(Foam::exp(A_*eta)-Foam::exp(B_*eta));
    
    }
};


