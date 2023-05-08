// Code changed and adapted to pemfc electrochemistry (J.P.Kone, 22.11.2016)

    //volumetric electrochemical Heating

    volScalarField electrochemicalHeating
    (
        IOobject
        (
            "electrochemicalHeating",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        0,
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField totalEntropySource
    (
        IOobject
        (
            "totalEntropySource",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        electrolyteMesh,
        0,
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "creating current density field\n" << endl;

// Variable name I is changed to i, K.W 2020    
volScalarField idensity
    (
        IOobject
        (
            "i",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "i",
            dimensionSet(0, -2, 0, 0, 0, 1, 0),
            Ibar0.value()
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField i0C // Variable name is changed (K.W 2020)
    (
        IOobject
        (
            "i0C",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "i0C",
            dimensionSet(0, -2, 0, 0, 0, 1, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField i0A // (K.W 2020)
    (
        IOobject
        (
            "i0A",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "i0A",
            dimensionSet(0, -2, 0, 0, 0, 1, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField iLC // (J.P.Kone, 05.07.2017)
    (
        IOobject
        (
            "iLC",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "iLC",
            dimensionSet(0, -2, 0, 0, 0, 1, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField etaOhmic
    (
        IOobject
        (
            "etaOhmic",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "etaOhmic",
            dimensionSet(1, 2, -3, 0, 0, -1, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );
    
    // Added SBB 02-02-14
 
    volScalarField etaA		//(K.W 2020)
    (
        IOobject
        (
            "etaA",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "etaA",
            dimensionSet(1, 2, -3, 0, 0, -1, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField etaC		// name is changed (K.W 2020)
    (
        IOobject
        (
            "etaC",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "etaC",
            dimensionSet(1, 2, -3, 0, 0, -1, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "creating voltage field\n" << endl;
    volScalarField voltage
    (
        IOobject
        (
            "V",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        V,
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "creating Nernst potential field\n" << endl;
    volScalarField NernstPot
    (
        IOobject
        (
            "NernstPot",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        1,
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField sigmaMem
    (
        IOobject
        (
            "sigmaMem",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "sigmaMem",
            dimensionSet(-1, -2, 3, 0, 0, 2, 0), // Siemens per meter (S/m)
            sigmaElectrolyte.value()
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    // regional field obtained from Tcell
    Info<< "Creating electrolyte field Telectrolyte" << endl;
    volScalarField Telectrolyte
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        Tinit,
        zeroGradientFvPatchScalarField::typeName
    );
    

    volScalarField lambda //(K.W 2020)
    (
        IOobject
        (
            "lambda",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        0,
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField waterActivity //(S.S 14.04.2020)
    (
        IOobject
        (
            "waterActivity",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        0,
        zeroGradientFvPatchScalarField::typeName
    );