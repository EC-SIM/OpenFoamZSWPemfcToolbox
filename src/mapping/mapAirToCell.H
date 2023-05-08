    // Mapping air subdomain.  Memory management

    // Code changed and adapted to pemfc (J.P.Kone, 15.07.2016)
    // for coversion to openfoam-4.0
    // call to 'internalField()' is changed to 'ref()'
    // call to 'boundaryField()' is changed to 'boundaryFieldRef()'
    // all functions of class porousZone are replaced with
    // equivalent funcions from new class porosityModel
    // 'forAll' loop is changed to 'for' to avoid any ambiguity

    {
        // Create fields from air side

        // mass based heat capacity cp
        // cp [J/kg-K] = Cp/M where Cp = [J/mol-K], M = [kg/mol]

        scalarField& cp = cpAir.ref(); 
        cp = 0;

        forAll(airSpecies, i)
        {
            scalar rMWi = 1.0/(airSpecies[i].MW()*1e-3);    // 1/[kg/mol]
            scalarField cpi = molarCpAir[i].polyVal(Tair.ref())*rMWi;
            cp += Yair[i].ref()*cpi;
        }
        cpAir.correctBoundaryConditions();

        scalarField rhoCpF = cp*rhoAir.ref();

        Info << nl;
        Info<< "min mean max rhoCpF(air) = " << gMin(rhoCpF) << " "
            << gAverage(rhoCpF) << " " << gMax(rhoCpF) << nl;

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, airCellMap);


        //scalarField kIn(airMesh.nCells(), kA.value());
        scalarField& kIn = kAir.ref();

        for(label iz=0;iz<cathodeZones.UPtrList<myPorosityModel>::size();iz++)
        {
            label znId =
                airMesh.cellZones().findZoneID(cathodeZones.UPtrList<myPorosityModel>::operator[](iz).name());

            dimensionedScalar kZn(cathodeZones.UPtrList<myPorosityModel>::operator[](iz).dict().lookup("k"));
            scalar porZn = cathodeZones.UPtrList<myPorosityModel>::operator[](iz).porosity();

            labelList znCells(airMesh.cellZones()[znId]);

            forAll(znCells, cellI)
            {
                //kIn[znCells[cellI]] =
                //    kZn.value()*(1 - porZn) + kA.value()*porZn;
                    
                kIn[znCells[cellI]] *= porZn;
                kIn[znCells[cellI]] += (1 - porZn)*kZn.value();
            }
        }

        // Perform reverse mapping
        kCellIn.rmap(kIn, airCellMap);

        // temperature source term set to zero in air (J.P.Kone, 07.01.2017)
        scalarField TsourceCellF
        (
            airMesh.nCells(),
            0
        );
        // Perform reverse mapping
        TsourceCellIn.rmap(TsourceCellF, airCellMap);


        // Map air fluxes
        labelList airInternalFaceMap
        (
            SubList<label>(airFaceMap, airMesh.nInternalFaces())
        );

        scalarField airInternalFaceMask
        (
            scalarField::subField(airFaceMask, airMesh.nInternalFaces())
        );

        //
        // ** recall phi already incorporates rho **
        //

        scalarField rhoCpPhiF =
            //CpAir.value()*phiAir.ref();
            linearInterpolate(cpAir)*phiAir.ref();

        phiCellIn.rmap
        (
            phiAir.ref()*airInternalFaceMask,
            airInternalFaceMap
        );

        rhoCpPhiCellIn.rmap
        (
            rhoCpPhiF*airInternalFaceMask,
            airInternalFaceMap
        );


        // Do electrolyte interface    (hkr: 2010 Dec 03)
        scalarField cathodePhi = phiAir.boundaryFieldRef()[cathodeID];
        forAll(cathodePhi, faceI)
        {
            phiCell[cathodeMask[faceI]] = cathodePhi[faceI];
            //rhoCpPhiCell[cathodeMask[faceI]]= CpAir.value()*cathodePhi[faceI];
            rhoCpPhiCell[cathodeMask[faceI]]=
            (
                cpAir[faceI]*cathodePhi[faceI]
            );
        }

        // Do flux boundary conditions
        forAll (airPatchesMap, patchI)
        {
            // Map only real matching patches.  HJ, 22/May/2011
            if
            (
                airPatchesMap[patchI] > -1
            )
            {
                // Patch maps
                labelField curFpm
                (
                    labelField::subField
                    (
                        airFaceMap,
                        airMesh.boundary()[patchI].size(),
                        airMesh.boundary()[patchI].patch().start()
                    )
                );

                scalarField curMask
                (
                    scalarField::subField
                    (
                        airFaceMask,
                        airMesh.boundary()[patchI].size(),
                        airMesh.boundary()[patchI].patch().start()
                    )
                );
		
		//11/03/2021 changed for parallel calculation
		word patchName = airMesh.boundary()[patchI].name();	//Name of patch in region mesh
		label patchID = mesh.boundaryMesh().findPatchID(patchName); //Find patch ID in global mesh

                curFpm -= mesh.boundaryMesh()[patchID].start();

                
                phiCell.boundaryFieldRef()[patchID].
                    scalarField::rmap
                    (
                        phiAir.boundaryFieldRef()[patchI]*curMask,
                        curFpm
                    );
                
                rhoCpPhiCell.boundaryFieldRef()[patchID].
                    scalarField::rmap
                    (
                        (
                            cpAir.boundaryFieldRef()[patchI]
                            *phiAir.boundaryFieldRef()[patchI]
                        )*curMask,
                        curFpm
                    );

                //rhoCpPhiCell.boundaryField()[airPatchesMap[patchI]] *=
                //(
                //    CpAir.value()
                //);
            }
        }
    }
