{
    Info << nl << "Solving fuel scalars" << endl;
    
    fv::options& fvOptions = fvOptionsFuel;

    tmp<fv::convectionScheme<scalar> > yConvection
    (
        fv::convectionScheme<scalar>::New
        (
            fuelMesh,
            fuelYs,
            phiFuel,
            fuelMesh.divScheme("div(phi,y)")
        )
    );


    label inertIndex = -1;
    volScalarField Yt("Yt", 0.0*Yfuel[0]);
    
    // code structure changed (J. P. Kone, 18.09.2016)    
    for (label i = 0; i < Yfuel.size(); i++)
    {
        if (fuelSpecies[i].name() != fuelInertSpecie)
        {
            Info << " Yfuel["<< i << "].name() = " << Yfuel[i].name() << nl;

            volScalarField& Yi = Yfuel[i];
            volScalarField& diffFuelS = diffSpFuel[i];

            /*
            solve
            (
                fvm::ddt(rhoFuel, Yi)
              + yConvection->fvmDiv(phiFuel, Yi)
              - fvm::laplacian(rhoFuel*diffFuelS, Yi, "laplacian(diff,y)"),
                "Yi"
            );
            */
            
            fvScalarMatrix YiEqn
            (
                fvm::ddt(rhoFuel, Yi)
              + yConvection->fvmDiv(phiFuel, Yi)
              - fvm::laplacian(rhoFuel*diffFuelS, Yi, "laplacian(diff,y)")
            );
            
            fvOptions.constrain(YiEqn);
            
            YiEqn.solve();
            YiEqn.relax();
            

            //Yi.max(0.0);
            Yi.max(Foam::doubleScalarSmall);
            Yt += Yi;
        }
        else
        {
            Info << " Yfuel["<< i << "].name() = " << Yfuel[i].name() << nl;
            inertIndex = i;
            
        }
    }

    Info<< "inertIndex = " << inertIndex << nl;
    Yfuel[inertIndex] = scalar(1) - Yt;
    //Yfuel[inertIndex].max(0.0);
    Yfuel[inertIndex].max(Foam::doubleScalarSmall);

    forAll(Yfuel, i)
    {
        Info << "min,mean,max(" << Yfuel[i].name() << "): "
             << gMin(Yfuel[i]) <<" , "
             << gAverage(Yfuel[i]) <<" , "
             << gMax(Yfuel[i]) << endl;
    }
}

// calculate mole fractions
#include "getXfuel.H"

