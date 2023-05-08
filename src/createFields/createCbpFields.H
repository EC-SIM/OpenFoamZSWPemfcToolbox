// cathode BP temperature field (J.P.Kone, 22.11.2016)

    // regional field obtained from Tcell
    Info<< "Creating cathode bipolar plate field TcathodeBP" << endl;
    volScalarField TcathodeBP
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            cbpMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        cbpMesh,
        Tinit,
        zeroGradientFvPatchScalarField::typeName
    );

