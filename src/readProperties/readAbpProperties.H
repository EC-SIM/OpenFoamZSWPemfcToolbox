    // Code changed and adapted to pemfc (J.P.Kone, 13.06.2016)
    // abpMesh: anode bipolarPlate mesh
    // BP: bipolarplate

    Info<< "Reading anode bipolarPlate properties\n" << endl;

    IOdictionary abpProperties
    (
        IOobject
        (
            "abpProperties",
            runTime.constant(),
            abpMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    dimensionedScalar rhoAnodeBP
        (abpProperties.lookup("rho"));

    dimensionedScalar CpAnodeBP
        (abpProperties.lookup("Cp"));

    dimensionedScalar kAnodeBP
        (abpProperties.lookup("k"));


    Info<< "  anode bipolarPlate" << nl
        << "    rhoAnodeBipolarPlate = " << rhoAnodeBP << nl
        << "    CpAnodeBipolarPlate  = " << CpAnodeBP << nl
        << "    kAnodeBipolarPlate   = " << kAnodeBP << nl
        << endl;

