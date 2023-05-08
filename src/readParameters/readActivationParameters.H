// Code changed and adapted to pemfc electrochemistry (J.P.Kone, 17.10.2016)

    Info<< "Reading activation parameters\n" << endl;

    IOdictionary activationParameters
    (
        IOobject
        (
            "activationParameters",
            runTime.constant(),
            electrolyteMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    // Read electrode kinetic parameters
    // cathode and anode side for Butler-Volmer Equation (K.W, 2020)
    dimensionedScalar i0CathodeElectrolyteReference(activationParameters.lookup("i0CathodeReference"));
    dimensionedScalar i0AnodeElectrolyteReference(activationParameters.lookup("i0AnodeReference"));

    // echo inputs from dictionary to stdout
    Info
        << "    i0CathodeElectrolyteReference    = " << i0CathodeElectrolyteReference << nl
        << "    i0AnodeElectrolyteReference    = " << i0AnodeElectrolyteReference << nl
        << endl;

    dimensionedScalar EAnodeElectrolyte(activationParameters.lookup("EAnode"));

    dimensionedScalar ECathodeElectrolyte(activationParameters.lookup("ECathode"));
    dimensionedScalar alphaCathodeElectrolyte(activationParameters.lookup("alphaCathode"));	
	
	dimensionedScalar conExponentC(activationParameters.lookup("conExponentC")); //S.S. 17.05.2021
	dimensionedScalar conExponentA(activationParameters.lookup("conExponentA")); //S.S. 17.05.2021
	dimensionedScalar refConC(activationParameters.lookup("refConC")); //S.S. 17.05.2021
	dimensionedScalar refConA(activationParameters.lookup("refConA")); //S.S. 17.05.2021
	
	dimensionedScalar iLeak(activationParameters.lookup("iLeak")); //S.S. 09.06.2021
    dimensionedScalar fRidders(activationParameters.lookup("fRidders")); //S.S. 09.06.2021


    // echo inputs from dictionary to stdout
    //Info<< "activation overpotential = " << activationOverpotential << nl;
    Info
        << "    EAnodeElectrolyte   = " << EAnodeElectrolyte   << nl
        << "    ECathodeElectrolyte   = " << ECathodeElectrolyte   << nl
        << "    alphaCathodeElectrolyte    = " << alphaCathodeElectrolyte    << nl
        << endl;
