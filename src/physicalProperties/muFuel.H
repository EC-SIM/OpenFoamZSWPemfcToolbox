// obtain mu(fuel mixture) from mu(species components)

// Code changed and adapted to pemfc (J.P.Kone, 19.07.2016)
// for coversion to openfoam-4.0
// call to 'internalField()' is changed to 'ref()'

{
    scalarField& mu = muFuel.ref(); 
    mu = 0;

    // initially using a simple linear combination weighted by mass fraction

    forAll(fuelSpecies, I)
    {
        scalarField muI = muFuelL[I].polyVal(Tfuel.ref());
        mu += muI*Yfuel[I].ref();
    }
    mu *= 1e-7;    //convert from micro-Poise to kg/(m s)
    muFuel.correctBoundaryConditions();

   Info << "min,mean,max(muFuel): "
        << gMin(mu)      << "   "
        << gAverage(mu)  << "   "
        << gMax(mu)      << endl;
}
