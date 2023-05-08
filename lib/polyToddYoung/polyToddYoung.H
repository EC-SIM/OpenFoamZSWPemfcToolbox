/*-------------------------------------------------------------------------*\
scalarField tau = T*1e-3;
int n = 7;
aH2O = (37.373, -41.205, 146.01, -217.08, 181.54, -79.409, 14.015);
\*-------------------------------------------------------------------------*/

#ifndef polyToddYoung_H
#define polyToddYoung_H

#include "scalar.H" // OpenFOAM src file
#include "scalarField.H" // OpenFOAM src file
#include "FixedList.H" // OpenFOAM src file  //2012.02.01

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class polyToddYoung Declaration
\*---------------------------------------------------------------------------*/

class polyToddYoung
  
{

public:

    // public data
    static const int nCoeffs_ = 7;
    typedef FixedList<scalar, nCoeffs_> coeffArray;    //2012.02.01


private:

    // private types and data

    // - coefficients
    coeffArray tyCoeffs_;     //2012.02.01
    scalar a0_, a1_, a2_, a3_, a4_, a5_, a6_;

    //        --------------- molar Cp coefficients --------------
    //Sp      a0      a1      a2       a3      a4       a5      a6      Cp1473K
    //H2    21.157, 56.036, -150.55, 199.29, -136.15, 46.903, -6.4725    32.146
    //H2O   37.373, -41.205, 146.01, -217.08, 181.54, -79.409, 14.015    46.831
    //O2    34.850  -57.975  203.68  -300.37  231.72  -91.821  14.776    36.469
    //N2    29.027  4.8987  -38.040  105.17  -113.56  55.554  -10.350    34.756
    //-------------------------------------------------------------------------


public:

    // Constructors

    // Construct from coefficients
    polyToddYoung
    (
        scalar a0, scalar a1, scalar a2,
        scalar a3, scalar a4, scalar a5, scalar a6
    );

    //- Construct from Istream
    polyToddYoung(Istream& is);

    //- Construct from dictionary     //2012.02.01
    polyToddYoung(const dictionary& dict);

    //- Construct as a named copy      //2012.02.01
    inline polyToddYoung(const word&, const polyToddYoung&);


    // Destructor

    ~polyToddYoung()
    {}


    // Member Functions

    //- Evaluate the polynomial function and return the result
    scalar polyVal(scalar T);

    //- Evaluate the polynomial function and return the result
    scalarField polyVal(scalarField T);


    // in case of Cp(T) polynomial, integral of Cp(T)dT useful for enthalpy

    //- Evaluate the integral of the polynomial function and return the result
    scalar polyInt(scalar T);

    //- Evaluate the integral of the polynomial function and return the result
    scalarField polyInt(scalarField T);

    //- Evaluate definite integral from Tlow to Thigh and return result
    scalar polyInt(scalar Tlow, scalar Thigh);

    //- Evaluate definite integral from Tlow to Thigh and return result
    scalarField polyInt(scalar Tlow, scalarField Thigh);

    //- Evaluate definite integral from Tlow to Thigh and return result
    scalarField polyInt(scalarField Tlow, scalarField Thigh);


    // in case of Cp(T) polynomial, integral of Cp(T)/T dT useful for entropy

    //- Evaluate the integral of the polynomial function divided by T
    //  and return the result
    scalar polyIntS(scalar T);
    scalarField polyIntS(scalarField T);

    //- Evaluate definite integral from Tlow to Thigh
    //  of the polynomial divided by T and return result
    scalar polyIntS(scalar Tlow, scalar Thigh);
    scalarField polyIntS(scalar Tlow, scalarField Thigh);
    scalarField polyIntS(scalarField Tlow, scalarField Thigh);


    //- Write the function coefficients
    void writeData();
    void writeData(Ostream& os);
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
