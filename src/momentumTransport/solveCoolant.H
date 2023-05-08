// Calculation of Coolant flow (S.S 03.02.2020)

{
    Info << nl << "Solving Coolant flow" << endl;
    
    pCoolant.storePrevIter();

    // Read controls
    const dictionary& piso = coolantMesh.solutionDict().subDict("PISO");
    int nCorr(readInt(piso.lookup("nCorrectors")));
    int nNonOrthCorr(readInt(piso.lookup("nNonOrthogonalCorrectors")));
    
    fvVectorMatrix UEqn
    (
        fvm::div(phiCoolant, UCoolant)
      - fvm::laplacian(muCoolant, UCoolant)
    );

    UEqn.relax();
    
    solve(UEqn == -fvc::grad(pCoolant));
    
    
    for (int corr = 0; corr < nCorr; corr++)
    {
        pCoolant.boundaryFieldRef().updateCoeffs();
        rUACoolant = 1.0/UEqn.A();
        UCoolant = UEqn.H()/UEqn.A();
        UCoolant.correctBoundaryConditions();

        phiCoolant = fvc::interpolate(rhoCoolant*UCoolant) & coolantMesh.Sf();

        for (int nonOrth = 0; nonOrth <= nNonOrthCorr; nonOrth++)
        {
            fvScalarMatrix pEqn
            (
                fvm::laplacian(rhoCoolant/UEqn.A(), pCoolant)
				== fvc::div(phiCoolant)
            );

            pEqn.setReference(pCoolantRefCell, pCoolantRefValue);
            pEqn.solve();

            if (nonOrth == nNonOrthCorr)
            {
                phiCoolant -= pEqn.flux();
            }
        }

        myContinuityErrs(phiCoolant);

        // Explicitly relax pressure for momentum corrector
        pCoolant.relax();

        UCoolant -= fvc::grad(pCoolant)/UEqn.A();
        UCoolant.correctBoundaryConditions();
    }
}
