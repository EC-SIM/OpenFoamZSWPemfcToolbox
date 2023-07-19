    //Changed K.W.
    fvMesh airMesh
    (
        IOobject
        (
            "air",
            runTime.timeName(),
            runTime,
            IOobject::MUST_READ
        )
    );

    // Read maps to global mesh

    // Note: faceRegionAddressing contains an offset by one and
    // sign management for a face flip.  airFaceMap and airFaceMask
    // are created to hold addressing and a (-1, 1) pre-multiplier
    // to manage the face flip.  HJ, 22/May/2011

    labelIOList airFaceRegionAddressing
    (
        IOobject
        (
            "faceRegionAddressing",
            airMesh.time().findInstance(airMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            airMesh,
            IOobject::MUST_READ
        )
    );

    labelList airFaceMap(airFaceRegionAddressing.size());
    scalarField airFaceMask(airFaceRegionAddressing.size());

    forAll(airFaceMap, i)
    {
        airFaceMap[i] = mag(airFaceRegionAddressing[i]) - 1;
        airFaceMask[i] = sign(airFaceRegionAddressing[i]);
    }

    labelIOList airCellMap
    (
        IOobject
        (
            "cellRegionAddressing",
            airMesh.time().findInstance(airMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            airMesh,
            IOobject::MUST_READ
        )
    );

    labelIOList airPatchesMap
    (
        IOobject
        (
            "boundaryRegionAddressing",
            airMesh.time().findInstance(airMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            airMesh,
            IOobject::MUST_READ
        )
    );
