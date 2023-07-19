{
    Info << nl << "Solving air scalars" << endl;
    
    fv::options& fvOptions = fvOptionsAir; //Added S.Schneider

    tmp<fv::convectionScheme<scalar> > yConvection
    (
        fv::convectionScheme<scalar>::New
        (
            airMesh,
            airYs,
            phiAir,
            airMesh.divScheme("div(phi,y)")
        )
    );


    label inertIndex = -1;
    volScalarField Yt("Yt", 0.0*Yair[0]);

    // code structure changed (J. P. Kone, 18.09.2016)
    for (label i = 0; i < Yair.size(); i++)
    {
        if (airSpecies[i].name() != airInertSpecie)
        {
            Info << " Yair["<< i << "].name() = " << Yair[i].name() << nl;

            volScalarField& Yi = Yair[i];
            volScalarField& diffAirS = diffSpAir[i];

            //Cahnged S. Schneider
            fvScalarMatrix YiEqn
            (
                fvm::ddt(rhoAir, Yi)
              + yConvection->fvmDiv(phiAir, Yi)
              - fvm::laplacian(rhoAir*diffAirS, Yi, "laplacian(diff,y)")
            );
            
            //Added S. Schneider
            fvOptions.constrain(YiEqn);
            
            YiEqn.solve();
            YiEqn.relax();

            //Yi.max(0.0);
            Yi.max(Foam::doubleScalarSmall); //Changed K.W.
            Yt += Yi;
        }
        else
        {
            Info << " Yair["<< i << "].name() = " << Yair[i].name() << nl;
            inertIndex = i;
        }
    }
    
    Info<< "inertIndex = " << inertIndex << nl;
    Yair[inertIndex] = scalar(1) - Yt;
    //Yair[inertIndex].max(0.0);
    Yair[inertIndex].max(Foam::doubleScalarSmall);  //Changed K.W.

    forAll(Yair, i)
    {
        Info << "min,mean,max(" << Yair[i].name() << "): "
             << gMin(Yair[i]) <<" , "
             << gAverage(Yair[i]) <<" , "
             << gMax(Yair[i]) << endl;
    }
}

// calculate mole fractions
#include "getXair.H"

