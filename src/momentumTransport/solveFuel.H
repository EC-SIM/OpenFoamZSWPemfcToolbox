// Code changed and adapted to pemfc (J.P.Kone, 19.07.2016)
// for coversion to openfoam-4.0
// call to 'internalField()' is changed to 'ref()'

{
    Info << nl << "Solving fuel flow" << endl;

    pFuel.storePrevIter();

    // Read controls
    const dictionary& piso = fuelMesh.solutionDict().subDict("PISO");
    int nCorr(readInt(piso.lookup("nCorrectors")));
    int nNonOrthCorr(readInt(piso.lookup("nNonOrthogonalCorrectors")));

    fvVectorMatrix UEqn
    (
        fvm::div(phiFuel, Ufuel)
      - fvm::laplacian(muFuel, Ufuel)
    );

    anodeZones.addResistance(UEqn);

    UEqn.relax();

    solve(UEqn == -fvc::grad(pFuel));


    //tmp<volScalarField> rUA;
    //rUA = 1.0/UEqn.A();

    // --- PISO loop

    for (int corr = 0; corr < nCorr; corr++)
    {
        pFuel.boundaryFieldRef().updateCoeffs();
        rUAfuel = 1.0/UEqn.A();
        Ufuel = UEqn.H()/UEqn.A();
        Ufuel.correctBoundaryConditions();

        phiFuel = fvc::interpolate(rhoFuel*Ufuel) & fuelMesh.Sf();

        for (int nonOrth = 0; nonOrth <= nNonOrthCorr; nonOrth++)
        {
            fvScalarMatrix pEqn
            (
                fvm::laplacian(rhoFuel/UEqn.A(), pFuel) == fvc::div(phiFuel)
            );

            pEqn.setReference(pFuelRefCell, pFuelRefValue);
            pEqn.solve();

            if (nonOrth == nNonOrthCorr)
            {
                phiFuel -= pEqn.flux();
            }
        }

        myContinuityErrs(phiFuel);

        // Explicitly relax pressure for momentum corrector
        pFuel.relax();

        Ufuel -= fvc::grad(pFuel)/UEqn.A();
        Ufuel.correctBoundaryConditions();
    }
}
