// Code changed and adapted to pemfc (J.P.Kone, 19.07.2016)
// for coversion to openfoam-4.0
// call to 'internalField()' is changed to 'ref()'


{
    // Map to cell.  In practice, fields for rho, Cp and k will come from
    // components (air, fuel, electrolyte) but currently they are constant

    scalarField& rhoCpCellIn = rhoCpCell.ref();
    rhoCpCellIn = 0.0;

    scalarField& kCellIn = kCell.ref();
    kCellIn = 0.0;

    scalarField& TsourceCellIn = TsourceCell.ref();
    TsourceCellIn = 0.0;


    scalarField& phiCellIn = phiCell.ref();
    phiCellIn = 0.0;

    scalarField& rhoCpPhiCellIn = rhoCpPhiCell.ref();
    rhoCpPhiCellIn = 0.0;

#   include "mapFuelToCell.H"
#   include "mapAirToCell.H"

#   include "mapElectrolyteToCell.H"
#   include "mapCoolantToCell.H"	//Implementation Coolant channel (K.W, 2020)
#   include "mapAbpToCell.H"
#   include "mapCbpToCell.H"

    // Correct boundary conditions
    rhoCpCell.correctBoundaryConditions();
    kCell.correctBoundaryConditions();

    // Check continuity of cell fluxes, as they appear in the energy equation
    myContinuityErrs(phiCell);
}

