    // Mapping cathode bipolarPlate subdomain.  Memory management

    // Code changed and adapted to pemfc (J.P.Kone, 13.06.2016)
    // cbpMesh: cathode side bipolarPlate mesh
    // BP: bipolarplate

    {
        // Create constant fields from bipolarPlate side
        scalarField rhoCpF
        (
            cbpMesh.nCells(),
            rhoCathodeBP.value()*CpCathodeBP.value()
        );

        scalarField kF
        (
            cbpMesh.nCells(),
            kCathodeBP.value()
        );

        // temperature source term set to zero in BP (J.P.Kone, 07.01.2017)
        scalarField TsourceCellF
        (
            cbpMesh.nCells(),
            0
        );

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, cbpCellMap);
        kCellIn.rmap(kF, cbpCellMap);
        TsourceCellIn.rmap(TsourceCellF, cbpCellMap);
    }
