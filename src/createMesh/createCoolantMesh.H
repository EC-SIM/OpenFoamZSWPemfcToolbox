    //coolantMesh: coolant channel mesh (K.W, 26.03.2020)

    fvMesh coolantMesh
    (
        IOobject
        (
            "coolant",
            runTime.timeName(),
            runTime,
            IOobject::MUST_READ
        )
    );

    labelIOList coolantFaceRegionAddressing
    (
        IOobject
        (
            "faceRegionAddressing",
            coolantMesh.time().findInstance(coolantMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            coolantMesh,
            IOobject::MUST_READ
        )
    );

    labelList coolantFaceMap(coolantFaceRegionAddressing.size());
    scalarField coolantFaceMask(coolantFaceRegionAddressing.size());

    forAll(coolantFaceMap, i)
    {
        coolantFaceMap[i] = mag(coolantFaceRegionAddressing[i]) - 1;
        coolantFaceMask[i] = sign(coolantFaceRegionAddressing[i]);
    }


    labelIOList coolantCellMap
    (
        IOobject
        (
            "cellRegionAddressing",
            coolantMesh.time().findInstance
            (
                coolantMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            coolantMesh,
            IOobject::MUST_READ
        )
    );


    labelIOList coolantPatchesMap
    (
        IOobject
        (
            "boundaryRegionAddressing",
            coolantMesh.time().findInstance
            (
                coolantMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            coolantMesh,
            IOobject::MUST_READ
        )
    );

