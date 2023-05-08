// anode BP temperature field (J.P.Kone, 22.11.2016)

    // regional field obtained from Tcell
    Info<< "Creating anode bipolar plate field TanodeBP" << endl;
    volScalarField TanodeBP
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            abpMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        abpMesh,
        Tinit,
        zeroGradientFvPatchScalarField::typeName
    );

