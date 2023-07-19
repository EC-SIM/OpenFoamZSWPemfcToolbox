// Code changed and adapted to pemfc (J.P.Kone, 23.09.2016)

// for coversion to openfoam-5.0
// call to 'internalField()' is changed to 'ref()'

{
    Info << nl << "Solving energy" << endl;
    
    fv::options& fvOptions = fvOptionsCell; //Added S. Schneider

    fvScalarMatrix TcellEqn
    (
        fvm::div(rhoCpPhiCell, Tcell)
      + fvm::SuSp(-fvc::div(rhoCpPhiCell), Tcell)
      - fvm::laplacian(kCell, Tcell)
      - TsourceCell
    );

    // Apply under-relaxation: not allowed
    // Under-relaxation in solid conduction (which dominates) creates a large
    // thermal inertia.  HJ, 25/May/2011
    //TcellEqn.relax();

    fvOptions.constrain(TcellEqn);  //Added S. Schneider
    
    // Solve
    TcellEqn.solve();


    Info << "T min mean max     = "
         << Foam::gMin(Tcell.ref()) << "    " //S.Keuler, Bugfix for consistent output (parallel simulation) [next three lines]
         << Foam::gAverage(Tcell.ref()) << "    "
         << Foam::gMax(Tcell.ref())
         << nl <<endl;

    Info<< " sum TcellEqn.source() =   " << gSum(TcellEqn.source()) << nl;
    Info<< endl;
}
