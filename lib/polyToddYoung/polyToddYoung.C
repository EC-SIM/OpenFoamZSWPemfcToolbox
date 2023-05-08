/*-------------------------------------------------------------------------*\
scalarField tau = T*1e-3;
int n = 7;
aH2O = (37.373, -41.205, 146.01, -217.08, 181.54, -79.409, 14.015);
\*-------------------------------------------------------------------------*/

#include "polyToddYoung.H"

#include "scalar.H" // OpenFOAM src file
#include "scalarField.H" // OpenFOAM src file

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

    //defineTypeNameAndDebug(polyToddYoung, 0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

    // Construct from coefficients
    polyToddYoung::polyToddYoung
    (
        scalar a0, scalar a1, scalar a2,
        scalar a3, scalar a4, scalar a5, scalar a6
    )
    :
        a0_(a0),
        a1_(a1),
        a2_(a2),
        a3_(a3),
        a4_(a4),
        a5_(a5),
        a6_(a6)
    {}
 

    //- Construct from Istream
    polyToddYoung::polyToddYoung(Istream& is)
    {
        forAll(tyCoeffs_, i)
        {
            is >> tyCoeffs_[i];
        }

        // Check state of Istream
        is.check("polyToddYoung::polyToddYoung(Istream& is)");

        a0_ = tyCoeffs_[0];
        a1_ = tyCoeffs_[1];
        a2_ = tyCoeffs_[2];
        a3_ = tyCoeffs_[3];
        a4_ = tyCoeffs_[4];
        a5_ = tyCoeffs_[5];
        a6_ = tyCoeffs_[6];
    }


    //Cp(H2 )  21.157, 56.036, -150.55, 199.29, -136.15, 46.903, -6.4725
    //Cp(H2O)  37.373, -41.205, 146.01, -217.08, 181.54, -79.409, 14.015


    // Member Functions

    //- Evaluate the polynomial function and return the result
    scalar polyToddYoung::polyVal(scalar T)
    {
        scalar t = T*1.0e-3;
        return (((((a6_*t + a5_)*t + a4_)*t + a3_)*t + a2_)*t + a1_)*t + a0_;
    }

    //- Evaluate the polynomial function and return the result
    scalarField polyToddYoung::polyVal(scalarField T)
    {
        scalarField t = T*1.0e-3;
        return (((((a6_*t + a5_)*t + a4_)*t + a3_)*t + a2_)*t + a1_)*t + a0_;
    }


    //- Evaluate the integral of the polynomial function and return the result
    //  Assumes zero for constant of integration
    scalar polyToddYoung::polyInt(scalar T)
    {
        scalar t = T*1.0e-3;
        return ((((((
        (
            a6_/7.0*t
            + a5_/6.0)*t
            + a4_/5.0)*t
            + a3_/4.0)*t
            + a2_/3.0)*t
            + a1_/2.0)*t
            + a0_)*t
        )*1.0e3;
    }

    //- Evaluate the integral of the polynomial function and return the result
    //  Assumes zero for constant of integration
    scalarField polyToddYoung::polyInt(scalarField T)
    {
        scalarField t = T*1.0e-3;
        return ((((((
        (
            a6_/7.0*t
            + a5_/6.0)*t
            + a4_/5.0)*t
            + a3_/4.0)*t
            + a2_/3.0)*t
            + a1_/2.0)*t
            + a0_)*t
        )*1.0e3;
    }


    //- Evaluate the definite integral of the polynomial function 
    //  from Tlow to Thigh and return the result
    //  (used with Cp polynomial for enthalpy)
    scalar polyToddYoung::polyInt(scalar Tlow, scalar Thigh)
    {
        return(polyInt(Thigh) - polyInt(Tlow));
    }

    //- Evaluate the definite integral of the polynomial function 
    //  from Tlow to Thigh and return the result
    scalarField polyToddYoung::polyInt(scalar Tlow, scalarField Thigh)
    {
        scalarField TlowF(Thigh.size(), Tlow);
        return(polyInt(Thigh) - polyInt(TlowF));
    }

    //- Evaluate the definite integral of the polynomial function 
    //  from Tlow to Thigh and return the result
    scalarField polyToddYoung::polyInt(scalarField Tlow, scalarField Thigh)
    {
        return(polyInt(Thigh) - polyInt(Tlow));
    }


    //- Evaluate the integral of the polynomial function divided by T
    //  and return the result.  Assumes zero for constant of integration
    //  (used with Cp polynomial for entropy)
    scalar polyToddYoung::polyIntS(scalar T)
    {
        scalar t = T*1.0e-3;
        return (((((
        (
            a6_/6.0*t
            + a5_/5.0)*t
            + a4_/4.0)*t
            + a3_/3.0)*t
            + a2_/2.0)*t
            + a1_)*t
            + a0_*Foam::log(T)
        );
    }

    scalarField polyToddYoung::polyIntS(scalarField T)
    {
        scalarField t = T*1.0e-3;
        return (((((
        (
            a6_/6.0*t
            + a5_/5.0)*t
            + a4_/4.0)*t
            + a3_/3.0)*t
            + a2_/2.0)*t
            + a1_)*t
            + a0_*Foam::log(T)
        );
    }

    //- Evaluate the definite integral of the polynomial function divided by T
    //  from Tlow to Thigh and return the result
    //  (used with Cp polynomial for entropy)
    //
    scalar polyToddYoung::polyIntS(scalar Tlow, scalar Thigh)
    {
        return(polyIntS(Thigh) - polyIntS(Tlow));
    }

    scalarField polyToddYoung::polyIntS(scalar Tlow, scalarField Thigh)
    {
        return(polyIntS(Thigh) - polyIntS(Tlow));
    }

    scalarField polyToddYoung::polyIntS(scalarField Tlow, scalarField Thigh)
    {
        return(polyIntS(Thigh) - polyIntS(Tlow));
    }


    //- Write the function coefficients
    void polyToddYoung::writeData()
    {
        Info<< a0_ << token::SPACE
            << a1_ << token::SPACE
            << a2_ << token::SPACE
            << a3_ << token::SPACE
            << a4_ << token::SPACE
            << a5_ << token::SPACE
            << a6_ << nl;
    }


    //- Write the function coefficients
    void polyToddYoung::writeData(Ostream& os)
    {
        os  << a0_ << token::SPACE
            << a1_ << token::SPACE
            << a2_ << token::SPACE
            << a3_ << token::SPACE
            << a4_ << token::SPACE
            << a5_ << token::SPACE
            << a6_;
    }

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam


// ************************************************************************* //
