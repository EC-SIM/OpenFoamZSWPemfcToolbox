    // Code changed and adapted to pemfc (J.P.Kone, 05.10.2016)

    Info<< "Reading electrolyte properties\n" << endl;

    IOdictionary electrolyteProperties
    (
        IOobject
        (
            "electrolyteProperties",
            runTime.constant(),
            electrolyteMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );


    // Read electrolyte properties
    dimensionedScalar rhoElectrolyte(electrolyteProperties.lookup("rho"));
    dimensionedScalar CpElectrolyte(electrolyteProperties.lookup("Cp"));
    dimensionedScalar kElectrolyte(electrolyteProperties.lookup("k"));
    dimensionedScalar sigmaElectrolyte(electrolyteProperties.lookup("sigma"));

    // echo inputs from dictionary to stdout
    Info
        << "    rhoElectrolyte  = " << rhoElectrolyte  << nl
        << "    CpElectrolyte   = " << CpElectrolyte   << nl
        << "    kElectrolyte    = " << kElectrolyte    << nl
        << "    sigmaElectrolyte    = " << sigmaElectrolyte    << nl
        << endl;
