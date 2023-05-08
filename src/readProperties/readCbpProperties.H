    // Code changed and adapted to pemfc (J.P.Kone, 13.06.2016)
    // cbpMesh: cathode bipolarPlate mesh
    // BP: bipolarplate

    Info<< "Reading cathode bipolarPlate properties\n" << endl;

    IOdictionary cbpProperties
    (
        IOobject
        (
            "cbpProperties",
            runTime.constant(),
            cbpMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    dimensionedScalar rhoCathodeBP
        (cbpProperties.lookup("rho"));

    dimensionedScalar CpCathodeBP
        (cbpProperties.lookup("Cp"));

    dimensionedScalar kCathodeBP
        (cbpProperties.lookup("k"));


    Info<< "  cathode bipolarPlate" << nl
        << "    rhoCathodeBipolarPlate = " << rhoCathodeBP << nl
        << "    CpCathodeBipolarPlate  = " << CpCathodeBP << nl
        << "    kCathodeBipolarPlate   = " << kCathodeBP << nl
        << endl;

