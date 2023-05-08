// Code changed and adapted to pemfc (J.P.Kone, 05.07.2016)
// for coversion to openfoam-4.0
// type 'porousZones' is changed to 'myIOporosityModelList'
// all functions of class porousZone are replaced with
// equivalent funcions from new class porosityModel
// 'forAll' loop is changed to 'for' to avoid any ambiguity


    Info<< "Creating fuel field p" << endl;
    volScalarField pFuel
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            fuelMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        fuelMesh
    );

    Info<< "Creating fuel field rho" << endl;
    volScalarField rhoFuel
    (
        IOobject
        (
            "rho",
            runTime.timeName(),
            fuelMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fuelMesh,
        rhoF,
        zeroGradientFvPatchScalarField::typeName
    );
    rhoFuel.correctBoundaryConditions();

    Info<< "Creating fuel field mu" << endl;
    volScalarField muFuel
    (
        IOobject
        (
            "mu",
            runTime.timeName(),
            fuelMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        fuelMesh,
        muF,
        zeroGradientFvPatchScalarField::typeName
    );
    muFuel.correctBoundaryConditions();


    Info<< "Creating fuel field U" << endl;
    volVectorField Ufuel
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            fuelMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        fuelMesh
    );

    Info<< "Creating fuel face flux field phi" << endl;
    surfaceScalarField phiFuel
    (
        IOobject
        (
            "phi",
            runTime.timeName(),
            fuelMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        linearInterpolate(rhoFuel*Ufuel) & fuelMesh.Sf()
    );

    // inverse matrix diagonal field rUA
    volScalarField rUAfuel
    (
        IOobject
        (
            "(1|A(U))",
            runTime.timeName(),
            fuelMesh
        ),
        fuelMesh,
        dimensionedScalar("rUA", dimTime/dimDensity, 1),
        zeroGradientFvPatchScalarField::typeName
    );

    // Pressure reference point in fuel
    label pFuelRefCell = 0;
    scalar pFuelRefValue = 0.0;

    setRefCell
    (
        pFuel,
        fuelMesh.solutionDict().subDict("PISO"),
        pFuelRefCell,
        pFuelRefValue
    );


    // regional field obtained from Tcell
    Info<< "Creating fuel field Tfuel" << endl;
    volScalarField Tfuel
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            fuelMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fuelMesh,
        Tinit,
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "Creating fuel field cp" << endl;
    volScalarField cpFuel
    (
        IOobject
        (
            "cp",
            runTime.timeName(),
            fuelMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fuelMesh,
        CpF,
        zeroGradientFvPatchScalarField::typeName
    );
    cpFuel.correctBoundaryConditions();

    Info<< "Creating fuel field k" << endl;
    volScalarField kFuel
    (
        IOobject
        (
            "k",
            runTime.timeName(),
            fuelMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fuelMesh,
        kF,
        zeroGradientFvPatchScalarField::typeName
    );
    kFuel.correctBoundaryConditions();


    Info<< "Creating fuel porous zones" << endl;
    myIOporosityModelList anodeZones(fuelMesh);

    Info<< "    number of porous zones = " << anodeZones.UPtrList<myPorosityModel>::size() << nl;

    for(label i=0;i<anodeZones.UPtrList<myPorosityModel>::size();i++)
    {
        label iz = fuelMesh.cellZones().findZoneID(anodeZones.UPtrList<myPorosityModel>::operator[](i).name());
        Info<< "    zone " << anodeZones.UPtrList<myPorosityModel>::operator[](i).cellZoneIDs()
            << " " << anodeZones.UPtrList<myPorosityModel>::operator[](i).name()
            << " : size = " << fuelMesh.cellZones()[iz].size()
            << "; porosity = " << anodeZones.UPtrList<myPorosityModel>::operator[](i).porosity() << nl;
    }
    Info<< endl;

fv::options& fvOptionsFuel(fv::options::New(fuelMesh));
