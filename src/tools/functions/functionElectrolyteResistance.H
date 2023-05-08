// integral calculation (J.P.Kone, 25.10.2016)

class functionElectrolyteResistance
{
    scalar sigmaElectrolyte_;
    
public: 

    functionElectrolyteResistance(scalar sigmaElectrolyte)
    :
    sigmaElectrolyte_(sigmaElectrolyte)
    {}

    scalar operator()(const scalar& t) const
    //scalar evaluate(const scalar& eta) const
    {
        return (1.0/sigmaElectrolyte_); 
    }
};


