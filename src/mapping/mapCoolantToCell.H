    // Mapping coolant subdomain.  Memory management (K.W, 2020)


    {

        scalarField rhoCpF
        (
            coolantMesh.nCells(),
            rhoCoolant.value()*CpCoolant.value()
        );

        scalarField kF
        (
            coolantMesh.nCells(),
            kCoolant.value()
        );

        scalarField TsourceCellF;
        (
            coolantMesh.nCells(),
            0
        );
	
	
	

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, coolantCellMap);
        kCellIn.rmap(kF, coolantCellMap);
        TsourceCellIn.rmap(TsourceCellF, coolantCellMap);
	
	
	//---------------------------------------------------------------
	
	// Map fuel fluxes
        labelList coolantInternalFaceMap
        (
            SubList<label>(coolantFaceMap, coolantMesh.nInternalFaces())
        );

        scalarField coolantInternalFaceMask
        (
            scalarField::subField(coolantFaceMask, coolantMesh.nInternalFaces())
        );

        //
        // ** recall: phi already incorporates rho **
        //

        scalarField rhoCpPhiF =
            CpCoolant.value()*phiCoolant.ref();

        phiCellIn.rmap
        (
            phiCoolant.ref()*coolantInternalFaceMask,
            coolantInternalFaceMap
        );

        rhoCpPhiCellIn.rmap
        (
            rhoCpPhiF*coolantInternalFaceMask,
            coolantInternalFaceMap
        );




        // Do flux boundary conditions
        forAll (coolantPatchesMap, patchI)
        {
            // Map only real matching patches.  HJ, 22/May/2011
            if
            (
                coolantPatchesMap[patchI] > -1
            )
            {
                // Patch maps
                labelField curFpm
                (
                    labelField::subField
                    (
                        coolantFaceMap,
                        coolantMesh.boundary()[patchI].size(),
                        coolantMesh.boundary()[patchI].patch().start()
                    )
                );

                scalarField curMask
                (
                    scalarField::subField
                    (
                        coolantFaceMask,
                        coolantMesh.boundary()[patchI].size(),
                        coolantMesh.boundary()[patchI].patch().start()
                    )
                );
		
		//11/03/2021 changed for parallel calculation
		word patchName = coolantMesh.boundary()[patchI].name();	//Name of patch in region mesh
		label patchID = mesh.boundaryMesh().findPatchID(patchName); //Find patch ID in global mesh

                curFpm -= mesh.boundaryMesh()[patchID].start();

                phiCell.boundaryFieldRef()[patchID].
                    scalarField::rmap
                    (
                        phiCoolant.boundaryFieldRef()[patchI]*curMask,
                        curFpm
                    );

                rhoCpPhiCell.boundaryFieldRef()[patchID].
                    scalarField::rmap
                    (
                        (
                            CpCoolant.value()
                            *phiCoolant.boundaryFieldRef()[patchI]
                        )*curMask,
                        curFpm
                    );

            }
        }
	
    }
