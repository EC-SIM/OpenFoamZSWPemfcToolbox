    Info<< "Reading air properties\n" << endl;

    IOdictionary airProperties
    (
        IOobject
        (
            "airProperties",
            runTime.constant(),
            airMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    dimensionedScalar muA(airProperties.lookup("mu"));
    dimensionedScalar rhoA(airProperties.lookup("rho"));
    dimensionedScalar CpA(airProperties.lookup("Cp"));
    dimensionedScalar kA(airProperties.lookup("k"));

    // hydraulic diameter for Reynolds number calculation
    dimensionedScalar dHydAir(airProperties.lookup("dHyd"));

    // echo input to output:
    Info << "    muA   = "    << muA       << nl
         << "    rhoA  = "    << rhoA      << nl
         << "    CpA   = "    << CpA       << nl
         << "    kA    = "    << kA        << nl
         << "    dHydAir  = " << dHydAir   << nl
         << endl;

    bool ReA(true);
    if(dHydAir.value() == 0)
    {
        ReA = false;
        Info << "Hydraulic diameter = 0: not calculating ReAir" << nl << endl;
    } 
