// Code changed and adapted to pemfc electrochemistry (J.P.Kone, 27.05.2016)

// for coversion to openfoam-5.0
// call to 'boundaryField()' is changed to 'boundaryFieldRef()' for all volScalarFields

{
    // total mass rate at fuel inlet
    scalarField  rateInlet =
    (
        rhoFuel.boundaryFieldRef()[fuelInletID]
        *(
             Ufuel.boundaryFieldRef()[fuelInletID]
             &
             fuelMesh.Sf().boundaryField()[fuelInletID]
         )
    );
    
    // total mass rate at fuel outlet
    scalarField  rateOutlet =
    (
        rhoFuel.boundaryFieldRef()[fuelOutletID]
        *(
             Ufuel.boundaryFieldRef()[fuelOutletID]
             &
             fuelMesh.Sf().boundaryField()[fuelOutletID]
         )
    );
    
    // total mass rate at anode/electrolyte interface
    scalarField  rateAnode =
    (
        rhoFuel.boundaryFieldRef()[anodeID]
        *(
             Ufuel.boundaryFieldRef()[anodeID]
             &
             fuelMesh.Sf().boundaryField()[anodeID]
         )
    );

    Info<< "fuel mass rates: " << nl
        << "   inlet: " << Foam::mag(Foam::gSum(rateInlet))
        << "   outlet: " << Foam::mag(Foam::gSum(rateOutlet))
        << "   anode: " << Foam::mag(Foam::gSum(rateAnode))
        << nl;
    
    // --------------------------------------------------------------------
    // cathodeToAnode interpolation of mass fluxes added (J.P.Kone, 27.05.2016)
    
    forAll(fuelSpecies, I)
    {
        if(fuelSpecies[I].rSign() < 0)
        {
            scalar rateInletI = Foam::gSum
            (
                rateInlet*Yfuel[I].boundaryFieldRef()[fuelInletID]
            );

            scalar rateOutletI = Foam::gSum
            (
                rateOutlet*Yfuel[I].boundaryFieldRef()[fuelOutletID]
            );

            scalar rateElecI = Foam::gSum
            (
            	cathodeToAnode.faceInterpolate(mflux[I])
                *fuelMesh.magSf().boundaryField()[anodeID]
            );

            Info<< fuelSpecies[I].name() << " mass rates: " << nl;
            Info<< "   inlet = " << Foam::mag(rateInletI)
                << "   outlet = " << Foam::mag(rateOutletI)
                << "   chem = " << Foam::mag(rateElecI)
                << nl;

            Info<< "      utilization by chemRate = "
                << Foam::mag(rateElecI/rateInletI)
                << nl
                << "      utilization by outRate  = "
                << 1.0 - Foam::mag(rateOutletI/rateInletI)
                << nl;

            Info<< endl;
        }
    }
}
