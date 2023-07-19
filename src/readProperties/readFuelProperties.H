    Info<< "Reading fuel properties\n" << endl;

    IOdictionary fuelProperties
    (
        IOobject
        (
            "fuelProperties",
            runTime.constant(),
            fuelMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    dimensionedScalar muF(fuelProperties.lookup("mu"));
    dimensionedScalar rhoF(fuelProperties.lookup("rho"));
    dimensionedScalar CpF(fuelProperties.lookup("Cp"));
    dimensionedScalar kF(fuelProperties.lookup("k"));

    // hydraulic diameter for Reynolds number calculation
    //dimensionedScalar dHydFuel(fuelProperties.lookup("dHyd"));    //Commented S.Schneider 07/2023

    // echo input to output:
    Info << "    muF       = " << muF       << nl
         << "    rhoF      = " << rhoF      << nl
         << "    CpF       = " << CpF       << nl
         << "    kF        = " << kF        << nl
         //<< "    dHydFuel  = " << dHydFuel  << nl //Commented S.Schneider 07/2023
         << endl;

    //Commented S.Schneider 07/2023 
    /*
    bool ReF(true);
    if(dHydFuel.value() == 0)
    {
        ReF = false;
        Info << "Hydraulic diameter = 0: not calculating ReFuel" << nl << endl;
    }
    */
