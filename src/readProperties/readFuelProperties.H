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
    dimensionedScalar dHydFuel(fuelProperties.lookup("dHyd"));

    // echo input to output:
    Info << "    muF       = " << muF       << nl
         << "    rhoF      = " << rhoF      << nl
         << "    CpF       = " << CpF       << nl
         << "    kF        = " << kF        << nl
         << "    dHydFuel  = " << dHydFuel  << nl
         << endl;

    bool ReF(true);
    if(dHydFuel.value() == 0)
    {
        ReF = false;
        Info << "Hydraulic diameter = 0: not calculating ReFuel" << nl << endl;
    }
