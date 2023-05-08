    // Code changed and adapted  (K.W, 26.03.2020)
    // coolant: Coolant channel

    Info<< "Reading Coolant properties\n" << endl;

    IOdictionary coolantProperties
    (
        IOobject
        (
            "coolantProperties",
            runTime.constant(),
            coolantMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );
    dimensionedScalar muCoolant(coolantProperties.lookup("mu"));

    dimensionedScalar rhoCoolant(coolantProperties.lookup("rho"));

    dimensionedScalar CpCoolant(coolantProperties.lookup("Cp"));

    dimensionedScalar kCoolant(coolantProperties.lookup("k"));


    Info<< "   	Coolant" << nl
        << "    rhoCoolant = " << rhoCoolant << nl
        << "    CpCoolant  = " << CpCoolant << nl
        << "    kCoolant   = " << kCoolant << nl
        << "    muCoolant  = " << muCoolant << nl
        << endl;

