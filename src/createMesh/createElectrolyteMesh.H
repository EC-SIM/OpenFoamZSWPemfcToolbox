    // Changed K.W.
    fvMesh electrolyteMesh
    (
        IOobject
        (
            "electrolyte",
            runTime.timeName(),
            runTime,
            IOobject::MUST_READ
        )
    );


    // Read maps to global mesh

    labelIOList electrolyteFaceRegionAddressing
    (
        IOobject
        (
            "faceRegionAddressing",
            electrolyteMesh.time().findInstance
            (
                electrolyteMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            electrolyteMesh,
            IOobject::MUST_READ
        )
    );

    labelList electrolyteFaceMap(electrolyteFaceRegionAddressing.size());
    scalarField electrolyteFaceMask(electrolyteFaceRegionAddressing.size());

    forAll(electrolyteFaceMap, i)
    {
        electrolyteFaceMap[i] = mag(electrolyteFaceRegionAddressing[i]) - 1;
        electrolyteFaceMask[i] = sign(electrolyteFaceRegionAddressing[i]);
    }

    labelIOList electrolyteCellMap
    (
        IOobject
        (
            "cellRegionAddressing",
            electrolyteMesh.time().findInstance
            (
                electrolyteMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            electrolyteMesh,
            IOobject::MUST_READ
        )
    );

    labelIOList electrolytePatchesMap
    (
        IOobject
        (
            "boundaryRegionAddressing",
            electrolyteMesh.time().findInstance
            (
                electrolyteMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            electrolyteMesh,
            IOobject::MUST_READ
        )
    );
