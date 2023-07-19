    //Changed K.W.
    fvMesh fuelMesh
    (
        IOobject
        (
            "fuel",
            runTime.timeName(),
            runTime,
            IOobject::MUST_READ
        )
    );

    // Read maps to global mesh

    labelIOList fuelFaceRegionAddressing
    (
        IOobject
        (
            "faceRegionAddressing",
            fuelMesh.time().findInstance(fuelMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            fuelMesh,
            IOobject::MUST_READ
        )
    );

    labelList fuelFaceMap(fuelFaceRegionAddressing.size());
    scalarField fuelFaceMask(fuelFaceRegionAddressing.size());

    forAll(fuelFaceMap, i)
    {
        fuelFaceMap[i] = mag(fuelFaceRegionAddressing[i]) - 1;
        fuelFaceMask[i] = sign(fuelFaceRegionAddressing[i]);
    }

    labelIOList fuelCellMap
    (
        IOobject
        (
            "cellRegionAddressing",
            fuelMesh.time().findInstance(fuelMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            fuelMesh,
            IOobject::MUST_READ
        )
    );

    labelIOList fuelPatchesMap
    (
        IOobject
        (
            "boundaryRegionAddressing",
            fuelMesh.time().findInstance(fuelMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            fuelMesh,
            IOobject::MUST_READ
        )
    );
