// Code changed and adapted to pemfc (J.P.Kone, 11.07.2016)

// for coversion to openfoam-4.0
// type 'porousZones' is changed to 'myIOporosityModelList'
// all functions of class porousZone are replaced with
// equivalent funcions from new class porosityModel
// 'forAll' loop is changed to 'for' to avoid any ambiguity


    // Fuel side
    // ---------
    Info << nl << "Creating fuel diffusivity models" << endl;

    // scalarField passed to diff model to return calculated diffusivity
    // --------------------------------
    scalarField fuelDiff(fuelMesh.nCells());

    // pointerlist for all fuel diffusivity models
    // ------------------------------------------
    PtrList<autoPtr<diffusivityModels::diffusivityModel> >
        fuelDiffModels(1 + anodeZones.UPtrList<myPorosityModel>::size());


    // Diffusivity: whole fuel zone
    // ----------------------------
    label fuelZnId = fuelMesh.cellZones().findZoneID("fuel");
    Info<< "zone " << fuelZnId << " fuel" << nl;
    const labelList& fuelCells = fuelMesh.cellZones()[fuelZnId];
    const dictionary& fuelDiffDict = fuelProperties.subDict("diffusivity");

    fuelDiffModels.set
    (
        0,
        new autoPtr<diffusivityModels::diffusivityModel>
        (
            diffusivityModels::diffusivityModel::New
                (fuelMesh, fuelDiff, fuelCells, fuelDiffDict)
        )
    );


    // fuel porous zones
    // -----------------
    label nzFuel = anodeZones.UPtrList<myPorosityModel>::size();

    // initialize pointer lists
    PtrList<const labelList> fuelZnCells(nzFuel);
    PtrList<const dictionary> fuelZnDiffDict(nzFuel);
    PtrList<autoPtr<diffusivityModels::diffusivityModel> >
        fuelZnDiffusivity(nzFuel);

    for(label iz=0;iz<anodeZones.UPtrList<myPorosityModel>::size();iz++)
    {
        label znId =
            fuelMesh.cellZones().findZoneID(anodeZones.UPtrList<myPorosityModel>::operator[](iz).name());

        Info<< "anodeZone " << iz << ", fuelZone " << znId
            << ": " << anodeZones.UPtrList<myPorosityModel>::operator[](iz).name() << endl;
        Info<< "size = " << fuelMesh.cellZones()[znId].size() << nl;

        fuelZnCells.set
        (
            iz,
            new labelList(fuelMesh.cellZones()[znId])
        );

        fuelZnDiffDict.set
        (
            iz,
            new dictionary(anodeZones.UPtrList<myPorosityModel>::operator[](iz).dict().subDict("diffusivity"))
        );

        fuelDiffModels.set
        (
            iz + 1,
            new autoPtr<diffusivityModels::diffusivityModel>
            (
                diffusivityModels::diffusivityModel::New
                    (fuelMesh, fuelDiff, fuelZnCells[iz], fuelZnDiffDict[iz])
            )
        );
    }


    // Air side
    // --------
        Info << nl << "Creating air diffusivity models" << endl;

        // scalarField passed to diff model to return calculated diffusivity
        // --------------------------------
        scalarField airDiff(airMesh.nCells());

        // pointerlist for all air diffusivity models
        // ------------------------------------------
        PtrList<autoPtr<diffusivityModels::diffusivityModel> >
        airDiffModels(1 + cathodeZones.UPtrList<myPorosityModel>::size());


        // Diffusivity: whole air zone
        // ---------------------------
        label airZnId = airMesh.cellZones().findZoneID("air");
        Info<< "zone " << airZnId << " air" << nl;
        const labelList& airCells = airMesh.cellZones()[airZnId];
        const dictionary& airDiffDict = airProperties.subDict("diffusivity");

        airDiffModels.set
        (
            0,
            new autoPtr<diffusivityModels::diffusivityModel>
            (
                diffusivityModels::diffusivityModel::New
                    (airMesh, airDiff, airCells, airDiffDict)
            )
        );

        // air porous zones
        // ----------------
        label nzAir = cathodeZones.UPtrList<myPorosityModel>::size();

        // initialize pointer lists
        PtrList<const labelList> airZnCells(nzAir);
        PtrList<const dictionary> airZnDiffDict(nzAir);
        PtrList<autoPtr<diffusivityModels::diffusivityModel> >
            airZnDiffusivity(nzAir);

        for(label iz=0;iz<cathodeZones.UPtrList<myPorosityModel>::size();iz++)
        {
            label znId =
                airMesh.cellZones().findZoneID(cathodeZones.UPtrList<myPorosityModel>::operator[](iz).name());

            Info<< "cathodeZone " << iz << ", airZone " << znId
                << ": " << cathodeZones.UPtrList<myPorosityModel>::operator[](iz).name() << endl;
            Info<< "size = " << airMesh.cellZones()[znId].size() << nl;

            airZnCells.set
            (
                iz,
                new labelList(airMesh.cellZones()[znId])
            );

            airZnDiffDict.set
            (
                iz,
                new dictionary(cathodeZones.UPtrList<myPorosityModel>::operator[](iz).dict().subDict("diffusivity"))
            );

            airDiffModels.set
            (
                iz + 1,
                new autoPtr<diffusivityModels::diffusivityModel>
                (
                    diffusivityModels::diffusivityModel::New
                        (airMesh, airDiff, airZnCells[iz], airZnDiffDict[iz])
                )
            );
        }
