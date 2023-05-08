// Code changed and adapted to pemfc (J.P.Kone, 05.07.2016)
// for coversion to openfoam-4.0
// type 'porousZones' is changed to 'myIOporosityModelList'
// all functions of class porousZone are replaced with
// equivalent funcions from new class porosityModel
// 'forAll' loop is changed to 'for' to avoid any ambiguity

    Info<< "Creating air field p" << endl;
    volScalarField pAir
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            airMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        airMesh
    );

    Info<< "Creating air field rho" << endl;
    volScalarField rhoAir
    (
        IOobject
        (
            "rho",
            runTime.timeName(),
            airMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        airMesh,
        rhoA,
        zeroGradientFvPatchScalarField::typeName
    );
    rhoAir.correctBoundaryConditions();

    Info<< "Creating air field mu" << endl;
    volScalarField muAir
    (
        IOobject
        (
            "mu",
            runTime.timeName(),
            airMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        airMesh,
        muA,
        zeroGradientFvPatchScalarField::typeName
    );
    muAir.correctBoundaryConditions();

    Info<< "Creating air field U" << endl;
    volVectorField Uair
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            airMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        airMesh
    );

    Info<< "Creating air face flux field phi" << endl;
    surfaceScalarField phiAir
    (
        IOobject
        (
            "phi",
            runTime.timeName(),
            airMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        linearInterpolate(rhoAir*Uair) & airMesh.Sf()
    );

    // inverse matrix diagonal field rUA
    volScalarField rUAair
    (
        IOobject
        (
            "(1|A(U))",
            runTime.timeName(),
            airMesh
        ),
        airMesh,
        dimensionedScalar("rUA", dimTime/dimDensity, 1),
        zeroGradientFvPatchScalarField::typeName
    );

    // Pressure reference point in air
    label pAirRefCell = 0;
    scalar pAirRefValue = 0.0;

    setRefCell
    (
        pAir,
        airMesh.solutionDict().subDict("PISO"),
        pAirRefCell,
        pAirRefValue
    );


    // regional field obtained from Tcell
    Info<< "Creating air field Tair" << endl;
    volScalarField Tair
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            airMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        airMesh,
        Tinit,
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "Creating air field cp" << endl;
    volScalarField cpAir
    (
        IOobject
        (
            "cp",
            runTime.timeName(),
            airMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        airMesh,
        CpA,
        zeroGradientFvPatchScalarField::typeName
    );
    cpAir.correctBoundaryConditions();

    Info<< "Creating air field k" << endl;
    volScalarField kAir
    (
        IOobject
        (
            "k",
            runTime.timeName(),
            airMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        airMesh,
        kA,
        zeroGradientFvPatchScalarField::typeName
    );
    kAir.correctBoundaryConditions();


    Info<< "Creating air porous zones" << endl;
    myIOporosityModelList cathodeZones(airMesh);

    Info<< "    number of porous zones = " << cathodeZones.UPtrList<myPorosityModel>::size() << nl;

    for(label i=0;i<cathodeZones.UPtrList<myPorosityModel>::size();i++)
    {
        label iz = airMesh.cellZones().findZoneID(cathodeZones.UPtrList<myPorosityModel>::operator[](i).name());
        Info<< "    zone " << cathodeZones.UPtrList<myPorosityModel>::operator[](i).cellZoneIDs()
            << " " << cathodeZones.UPtrList<myPorosityModel>::operator[](i).name()
            << " : size = " << airMesh.cellZones()[iz].size()
            << "; porosity = " << cathodeZones.UPtrList<myPorosityModel>::operator[](i).porosity() << nl;;
    }
    Info<< endl;
    
fv::options& fvOptionsAir(fv::options::New(airMesh));