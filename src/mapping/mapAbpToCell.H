    // Mapping anode bipolarPlate subdomain.  Memory management

    // Code changed and adapted to pemfc (J.P.Kone, 13.06.2016)
    // abpMesh: anode side bipolarPlate mesh
    // BP: bipolarplate

    {
        // Create constant fields from bipolarPlate side
        scalarField rhoCpF
        (
            abpMesh.nCells(),
            rhoAnodeBP.value()*CpAnodeBP.value()
        );

        scalarField kF
        (
            abpMesh.nCells(),
            kAnodeBP.value()
        );

        // temperature source term set to zero in BP (J.P.Kone, 07.01.2017)
        scalarField TsourceCellF
        (
            abpMesh.nCells(),
            0
        );

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, abpCellMap);
        kCellIn.rmap(kF, abpCellMap);
        TsourceCellIn.rmap(TsourceCellF, abpCellMap);
    }
