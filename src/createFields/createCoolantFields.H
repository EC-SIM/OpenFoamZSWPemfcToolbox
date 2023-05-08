// Coolant field (K.W 2020)

    // regional field obtained from Tcell
    Info<< "Creating Coolant field TCoolant" << endl;
    volScalarField TCoolant
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            coolantMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        coolantMesh,
        TinitCoolant,
        zeroGradientFvPatchScalarField::typeName
    );
	
	Info<< "Creating Coolant field p" << endl;
    volScalarField pCoolant
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            coolantMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        coolantMesh
    );



	Info<< "Creating Coolant field U" << endl;
    volVectorField UCoolant
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            coolantMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        coolantMesh
    );

	Info<< "Creating Coolant face flux field phi" << endl;
    surfaceScalarField phiCoolant
    (
        IOobject
        (
            "phi",
            runTime.timeName(),
            coolantMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        linearInterpolate(rhoCoolant*UCoolant) & coolantMesh.Sf()
    );

    // inverse matrix diagonal field rUA
    volScalarField rUACoolant
    (
        IOobject
        (
            "(1|A(U))",
            runTime.timeName(),
            coolantMesh
        ),
        coolantMesh,
        dimensionedScalar("rUA", dimTime/dimDensity, 1),
        zeroGradientFvPatchScalarField::typeName
    );

    // Pressure reference point in coolant
    label pCoolantRefCell = 0;
    scalar pCoolantRefValue = 0.0;

    setRefCell
    (
        pCoolant,
        coolantMesh.solutionDict().subDict("PISO"),
        pCoolantRefCell,
        pCoolantRefValue
    );
