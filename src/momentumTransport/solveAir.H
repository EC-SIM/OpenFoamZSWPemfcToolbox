// Code changed and adapted to pemfc (J.P.Kone, 19.07.2016)
// for coversion to openfoam-4.0
// call to 'internalField()' is changed to 'ref()'

{
    Info << nl << "Solving air flow" << endl;

    pAir.storePrevIter();

    // Read controls
    const dictionary& piso = airMesh.solutionDict().subDict("PISO");
    int nCorr(readInt(piso.lookup("nCorrectors")));
    int nNonOrthCorr(readInt(piso.lookup("nNonOrthogonalCorrectors")));

    fvVectorMatrix UEqn
    (
        fvm::div(phiAir, Uair)
      - fvm::laplacian(muAir, Uair)
    );

    cathodeZones.addResistance(UEqn);

    UEqn.relax();

    solve(UEqn == -fvc::grad(pAir));

    // --- PISO loop

    for (int corr = 0; corr < nCorr; corr++)
    {
        pAir.boundaryFieldRef().updateCoeffs();
        rUAair = 1.0/UEqn.A();
        Uair = UEqn.H()/UEqn.A();
        Uair.correctBoundaryConditions();

        phiAir = fvc::interpolate(rhoAir*Uair) & airMesh.Sf();

        for (int nonOrth = 0; nonOrth <= nNonOrthCorr; nonOrth++)
        {
            fvScalarMatrix pEqn
            (
                fvm::laplacian(rhoAir/UEqn.A(), pAir) == fvc::div(phiAir)
            );

            pEqn.setReference(pAirRefCell, pAirRefValue);
            pEqn.solve();

            if (nonOrth == nNonOrthCorr)
            {
                phiAir -= pEqn.flux();
            }
        }

        myContinuityErrs(phiAir);

        // Explicitly relax pressure for momentum corrector
        pAir.relax();

        Uair -= fvc::grad(pAir)/UEqn.A();
        Uair.correctBoundaryConditions();
    }
}
