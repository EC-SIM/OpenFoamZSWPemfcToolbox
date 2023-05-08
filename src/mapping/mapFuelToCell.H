    // Mapping fuel subdomain.  Memory management

    // Code changed and adapted to pemfc (J.P.Kone, 11.07.2016)
    // for coversion to openfoam-4.0
    // call to 'internalField()' is changed to 'ref()'
    // call to 'boundaryField()' is changed to 'boundaryFieldRef()'
    // all functions of class porousZone are replaced with
    // equivalent funcions from new class porosityModel
    // 'forAll' loop is changed to 'for' to avoid any ambiguity

    {
        // Create fields from fuel side

        // mass based heat capacity cp
        // cp [J/kg-K] = Cp/M where Cp = [J/mol-K], M = [kg/mol]

        scalarField& cp = cpFuel.ref();
        cp = 0;

        forAll(fuelSpecies, i)
        {
            // molarCp[i].polyVal() returns [J/(mol K)]
            // species[i].MW() returns {kg/kmol]
            // we require cp [J/(kg K] = [J/(mol K)]/[kg/mol]

            scalar rMWi = 1.0/(fuelSpecies[i].MW()*1e-3);    // 1/[kg/mol]
            scalarField cpi =
            (
                 molarCpFuel[i].polyVal(Tfuel.ref())*rMWi
            );
            cp += Yfuel[i].ref()*cpi;
        }
        cpFuel.correctBoundaryConditions();

        scalarField rhoCpF = cp*rhoFuel.ref();

        Info<< "min mean max rhoCpF(fuel) = " << gMin(rhoCpF) << " "
            << gAverage(rhoCpF) << " " << gMax(rhoCpF) << nl;
        Info << endl;

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, fuelCellMap);


        //scalarField kIn(fuelMesh.nCells(), kF.value());
        scalarField& kIn = kFuel.ref();

        for(label iz=0;iz<anodeZones.UPtrList<myPorosityModel>::size();iz++)
        {
            label znId =
                fuelMesh.cellZones().findZoneID(anodeZones.UPtrList<myPorosityModel>::operator[](iz).name());

            dimensionedScalar kZn(anodeZones.UPtrList<myPorosityModel>::operator[](iz).dict().lookup("k"));
            scalar porZn = anodeZones.UPtrList<myPorosityModel>::operator[](iz).porosity();

            labelList znCells(fuelMesh.cellZones()[znId]);

            forAll(znCells, cellI)
            {
                //kIn[znCells[cellI]] =
                //    kZn.value()*(1 - porZn) + kF.value()*porZn;

                kIn[znCells[cellI]] *= porZn;
                kIn[znCells[cellI]] += (1 - porZn)*kZn.value();
            }
        }

        // Perform reverse mapping
        kCellIn.rmap(kIn, fuelCellMap);

        // temperature source term set to zero in fuel (J.P.Kone, 07.01.2017)
        scalarField TsourceCellF
        (
            fuelMesh.nCells(),
            0
        );
        // Perform reverse mapping
        TsourceCellIn.rmap(TsourceCellF, fuelCellMap);


        // Map fuel fluxes
        labelList fuelInternalFaceMap
        (
            SubList<label>(fuelFaceMap, fuelMesh.nInternalFaces())
        );

        scalarField fuelInternalFaceMask
        (
            scalarField::subField(fuelFaceMask, fuelMesh.nInternalFaces())
        );

        //
        // ** recall: phi already incorporates rho **
        //

        scalarField rhoCpPhiF =
            //CpFuel.value()*phiFuel.internalField();
            linearInterpolate(cpFuel)*phiFuel.ref();

        phiCellIn.rmap
        (
            phiFuel.ref()*fuelInternalFaceMask,
            fuelInternalFaceMap
        );

        rhoCpPhiCellIn.rmap
        (
            rhoCpPhiF*fuelInternalFaceMask,
            fuelInternalFaceMap
        );


        // Do electrolyte interface    (hkr: 2010 Dec 03)
        scalarField anodePhi = phiFuel.boundaryFieldRef()[anodeID];
        forAll(anodePhi, faceI)
        {
            phiCell[anodeMask[faceI]] = anodePhi[faceI];
            //rhoCpPhiCell[anodeMask[faceI]] = CpFuel.value()*anodePhi[faceI];
            rhoCpPhiCell[anodeMask[faceI]] =
            (
                cpFuel[faceI]*anodePhi[faceI]
            );
        }

        // Do flux boundary conditions
        forAll (fuelPatchesMap, patchI)
        {
            // Map only real matching patches.  HJ, 22/May/2011
            if
            (
                fuelPatchesMap[patchI] > -1
            )
            {
                // Patch maps
                labelField curFpm
                (
                    labelField::subField
                    (
                        fuelFaceMap,
                        fuelMesh.boundary()[patchI].size(),
                        fuelMesh.boundary()[patchI].patch().start()
                    )
                );

                scalarField curMask
                (
                    scalarField::subField
                    (
                        fuelFaceMask,
                        fuelMesh.boundary()[patchI].size(),
                        fuelMesh.boundary()[patchI].patch().start()
                    )
                );
		
		//11/03/2021 changed for parallel calculation
		word patchName = fuelMesh.boundary()[patchI].name();	//Name of patch in region mesh
		label patchID = mesh.boundaryMesh().findPatchID(patchName); //Find patch ID in global mesh

                curFpm -= mesh.boundaryMesh()[patchID].start();

                phiCell.boundaryFieldRef()[patchID].
                    scalarField::rmap
                    (
                        phiFuel.boundaryFieldRef()[patchI]*curMask,
                        curFpm
                    );

                rhoCpPhiCell.boundaryFieldRef()[patchID].
                    scalarField::rmap
                    (
                        (
                            cpFuel.boundaryFieldRef()[patchI]
                            *phiFuel.boundaryFieldRef()[patchI]
                        )*curMask,
                        curFpm
                    );

                //rhoCpPhiCell.boundaryField()[fuelPatchesMap[patchI]] *=
                //(
                //    CpFuel.value()
                //);
            }
        }
    }
