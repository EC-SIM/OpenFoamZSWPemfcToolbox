// Code changed and adapted to pemfc electrochemistry (J.P.Kone, 27.05.2016)

// for coversion to openfoam-5.0 K.W.
// call to 'boundaryField()' is changed to 'boundaryFieldRef()' for all volScalarFields

{
    // total air mass rate at inlet
    scalarField  rateInlet =
    (
        rhoAir.boundaryFieldRef()[airInletID]
        *(
             Uair.boundaryFieldRef()[airInletID]
             &
             airMesh.Sf().boundaryField()[airInletID]
         )
    );

    // total air mass rate at outlet
    scalarField  rateOutlet =
    (
        rhoAir.boundaryFieldRef()[airOutletID]
        *(
             Uair.boundaryFieldRef()[airOutletID]
             &
             airMesh.Sf().boundaryField()[airOutletID]
         )
    );

    // total air mass rate at cathode/electrolyte interface
    scalarField  rateCathode =
    (
        rhoAir.boundaryFieldRef()[cathodeID]
        *(
             Uair.boundaryFieldRef()[cathodeID]
             &
             airMesh.Sf().boundaryField()[cathodeID]
         )
    );

    Info<< "air mass rates: " << nl
        << "   inlet: " << Foam::mag(Foam::gSum(rateInlet))
        << "   outlet: " << Foam::mag(Foam::gSum(rateOutlet))
        << "   cathode: " << Foam::mag(Foam::gSum(rateCathode))
        << nl;

    // --------------------------------------------------------------------
    // Foam::mag added (J.P.Kone, 27.05.2016)

    forAll(airSpecies, I)
    {
        if(airSpecies[I].rSign() < 0)
        {
            scalar rateInletI = Foam::mag
            (
                gSum
                (
                    rateInlet*Yair[I].boundaryFieldRef()[airInletID]
                )
            );

            scalar rateOutletI = Foam::mag
            (
                gSum
                (
                    rateOutlet*Yair[I].boundaryFieldRef()[airOutletID]
                )
            );

            scalar rateElecI = Foam::mag
            (
                gSum
                (
                    mflux[I]*airMesh.magSf().boundaryField()[cathodeID]
                )
            );

            Info<< airSpecies[I].name() << " mass rates: " << nl;
            Info<< "   inlet = " << rateInletI
                << "   outlet = " << rateOutletI
                << "   chem = " << rateElecI
                << nl;

            Info<< "      utilization by chemRate = "
                << rateElecI/rateInletI
                << nl;
            Info<< "      utilization by outRate  = "
                << (rateInletI - rateOutletI)/rateInletI
                << nl;

            Info<< endl;
        }
    }
}
