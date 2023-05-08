    // Code changed and adapted to pemfc (J.P.Kone, 14.09.2016)

    // Electrochemistry will be solved on the cathode

    const polyPatch& anodePatch = fuelMesh.boundaryMesh()[anodeID];
    const polyPatch& cathodePatch = airMesh.boundaryMesh()[cathodeID];
    const polyPatch& electrolyteAnodePatch = electrolyteMesh.boundaryMesh()[electrolyteAnodeID];
    const polyPatch& electrolyteCathodePatch = electrolyteMesh.boundaryMesh()[electrolyteCathodeID];


    myPatchToPatchInterpolation<polyPatch, polyPatch> anodeToCathode //Adapted for openfoam-5.0 (K.W, 2020)
    (
        anodePatch,
        cathodePatch
    );

    myPatchToPatchInterpolation<polyPatch, polyPatch> cathodeToAnode  //Adapted for openfoam-5.0 (K.W, 2020)
    (
        cathodePatch,
        anodePatch
    );

    // Create a mask from global faces to the anode surface on the fuel mesh
    labelList anodeMask(anodePatch.size());

    forAll (anodeMask, i)
    {
        anodeMask[i] = fuelFaceMap[anodePatch.start() + i];
    }

    // Create a mask from global faces to the cathode surface on the air mesh
    labelList cathodeMask(cathodePatch.size());

    forAll (cathodeMask, i)
    {
        cathodeMask[i] = airFaceMap[cathodePatch.start() + i];
    }

    // Create a mask from global faces to the anode surface on the electrolyte mesh
    labelList electrolyteAnodeMask(electrolyteAnodePatch.size());

    forAll (electrolyteAnodeMask, i)
    {
        electrolyteAnodeMask[i] = electrolyteFaceMap[electrolyteAnodePatch.start() + i];
    }

    // Create a mask from global faces to the cathode surface on the electrolyte mesh
    labelList electrolyteCathodeMask(electrolyteCathodePatch.size());

    forAll (electrolyteCathodeMask, i)
    {
        electrolyteCathodeMask[i] = electrolyteFaceMap[electrolyteCathodePatch.start() + i];
    }


    // Ohmic heating will occur in the electrolyte
    // Interpolation is no longer required (R.Nishida, 01.04.2014)

    myPatchToPatchInterpolation<polyPatch, polyPatch> fuelAnodeToElectrolyteAnode //Adapted for openfoam-5.0 (K.W, 2020)
    (
        fuelMesh.boundaryMesh()[anodeID],
        electrolyteMesh.boundaryMesh()[electrolyteAnodeID]
    );

    myPatchToPatchInterpolation<polyPatch, polyPatch> airCathodeToElectrolyteCathode  //Adapted for openfoam-5.0 (K.W, 2020)
    (
        airMesh.boundaryMesh()[cathodeID],
        electrolyteMesh.boundaryMesh()[electrolyteCathodeID]
    );
