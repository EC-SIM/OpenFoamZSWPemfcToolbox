    //abpMesh: anode side bipolarPlate mesh (J.P.Kone, 13.06.2016)

    //Changed K.W.
    fvMesh abpMesh
    (
        IOobject
        (
            "abp",
            runTime.timeName(),
            runTime,
            IOobject::MUST_READ
        )
    );

    // Read maps to global mesh
    // No mapping of face fields in solids.  HJ, 25/May/2011

    labelIOList abpCellMap
    (
        IOobject
        (
            "cellRegionAddressing",
            abpMesh.time().findInstance
            (
                abpMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            abpMesh,
            IOobject::MUST_READ
        )
    );

    labelIOList abpPatchesMap
    (
        IOobject
        (
            "boundaryRegionAddressing",
            abpMesh.time().findInstance
            (
                abpMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            abpMesh,
            IOobject::MUST_READ
        )
    );

