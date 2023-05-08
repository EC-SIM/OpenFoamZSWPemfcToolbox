//  membrane protonic conductivity sigma(S/m) (J.P.Kone, 30.09.2016)

//  sigma = (0.5139*lambda-0.326)exp[1268*(1/303-1/T)]
//  lambda = membrane water content
//         = 0.043 + 17.18*a - 39.85*a^2 + 36.0*a^3 for 0<a<=1
//         or   
//         = 14 + 1.4*(a-1) for 1<=a<=3
//  a = water activity
//    = xH2O*pAir/pSat
//  pSat = saturation pressure of water vapor (atm)
//  log pSat = -2.1794 + 0.02953*(T) - 9.1837*10^-5*(T)^2
//             + 1.4454*10^-7*(T)^3

scalarField sigmaElectrolyte(electrolyteCathodePatch.size());
{
    // saturation pressure of water vapor
    // value multiply by 101325 to convert to Pa
    scalar T0 = 273.15;         // 273.15 Kelvin = 0 Celsius
    scalar b = 10;

    scalarField pSatAnode =
    (
        pow(b, (-2.1794 + (0.02953*(anodeT-T0))
        - (9.1837e-5*sqr(anodeT-T0))
        + (1.4454e-7*pow((anodeT-T0),3))))
    );
    pSatAnode *= 101325.0;
    Info << nl << "min,max(pSatAnode): "
         << Foam::min(pSatAnode) <<" , "
         << Foam::max(pSatAnode) << endl;

 
    scalarField pSatCathode =
    (
        pow(b, (-2.1794 + (0.02953*(cathodeT-T0))
        - (9.1837e-5*sqr(cathodeT-T0))
        + (1.4454e-7*pow((cathodeT-T0),3))))
    );
    pSatCathode *= 101325.0;
    Info << "min,max(pSatCathode): "
         << Foam::min(pSatCathode) <<" , "
         << Foam::max(pSatCathode) << endl;


    // water activity
    scalarField wActivity(electrolyteCathodePatch.size());

    scalarField wActivityAnode(electrolyteAnodePatch.size());
    forAll(fuelSpecies, fsi)
    {
        if (fuelSpecies[fsi].name() == "H2O")			//Changed (S.S.03/2021)
        {
            wActivityAnode = xFuel[fsi]*pFuelPatch/pSatAnode;
        }
    }

    Info << "min,mean,max(wActivityAnode): "
         << gMin(wActivityAnode) <<" , "
         << gAverage(wActivityAnode) <<"  "
         << gMax(wActivityAnode) << endl;


    scalarField wActivityCathode(electrolyteCathodePatch.size());
    forAll(airSpecies, fsi)
    {
        if (airSpecies[fsi].name() == "H2O")			//Changed (S.S.03/2021)
        {
            wActivityCathode = xAir[fsi]*pAirPatch/pSatCathode;
        }
    }

    Info << "min,mean,max(wActivityCathode): "
         << gMin(wActivityCathode) <<" , "
         << gAverage(wActivityCathode) <<"  "
         << gMax(wActivityCathode) << endl;

	//Changed, improved Calculation of membrane water content (K.W 2020)
    wActivity = (anodeToCathode.faceInterpolate(wActivityAnode)
				+ wActivityCathode)/2;
 
    Info << "min,mean,max(a): "
         << gMin(wActivity) <<" , "
         << gAverage(wActivity) <<"  "
         << gMax(wActivity) << endl;


    // membrane water content
    scalarField wContent(electrolyteCathodePatch.size());
    forAll(wContent, facei)
    {        
        if ((wActivity[facei]>0.0) && (wActivity[facei]<1.0))
        {
            wContent[facei] =
            (
                0.043
                + (17.18*wActivity[facei])
                - (39.85*sqr(wActivity[facei]))
                + (36.0*pow(wActivity[facei],3))
            );
        }
        else if ((wActivity[facei]>=1.0) && (wActivity[facei]<=3.0))
        {
            wContent[facei] =
            (
                14.0
                + (1.4*(wActivity[facei] - 1.0))
            );
        }
    }
    Info << "min, mean, max (lambda): "
         << gMin(wContent) <<" , "
         << gAverage(wContent)<<"  "
         << gMax(wContent) <<endl;


    // membrane conductivity
    sigmaElectrolyte =
    (
        ((0.5139*wContent) - 0.326)
        *Foam::exp(1268.0*((1.0/303.0) - (1.0/cathodeT)))
    );
 
    scalarField& sigmaMemIn = sigmaMem.ref();
    smearPatchToMesh smearsigmaMem(electrolyteMesh, electrolyteCathodeName);
    sigmaMemIn = smearsigmaMem.smear(sigmaElectrolyte);
    sigmaMem.correctBoundaryConditions();
    sigmaMem.boundaryFieldRef()[electrolyteCathodeID] == sigmaElectrolyte;

    Info << "min,mean,max(sigma): "
         << gMin(sigmaMem.ref()) << "  "
         << gAverage(sigmaMem.ref()) << "  "
         << gMax(sigmaMem.ref()) << endl;
	
    
    //Postprocessing membrane water content (K.W, 2020)
    scalarField& lambdaIn = lambda.ref();
    smearPatchToMesh smearlambda(electrolyteMesh, electrolyteCathodeName);
    lambdaIn = smearlambda.smear(wContent);
    lambda.correctBoundaryConditions();
    lambda.boundaryFieldRef()[electrolyteCathodeID] == wContent;
    
    //Postprocessing membrane water activity (S.S, 29.03.2021)
    scalarField& waterActivityIn = waterActivity.ref();
    smearPatchToMesh smearwaterActivity(electrolyteMesh, electrolyteCathodeName);
    waterActivityIn = smearwaterActivity.smear(wActivity);
    waterActivity.correctBoundaryConditions();
    waterActivity.boundaryFieldRef()[electrolyteCathodeID] == wActivity;
    
}
