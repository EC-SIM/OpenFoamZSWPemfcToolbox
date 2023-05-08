// obtain mu(air mixture) from mu(species components)

// Code changed and adapted to pemfc (J.P.Kone, 19.07.2016)
// for coversion to openfoam-4.0
// call to 'internalField()' is changed to 'ref()'

{
    scalarField& mu = muAir.ref(); 
    mu = 0;

    // initially using a simple linear combination weighted by mass fraction

    forAll(airSpecies, I)
    {
        scalarField muI = muAirL[I].polyVal(Tair.ref());
        mu += muI*Yair[I].ref();
    }
    mu *= 1e-7;    //convert from micro-Poise to kg/(m s)
    muAir.correctBoundaryConditions();

   Info << "min,mean,max(muAir): "
        << gMin(mu)      << "   "
        << gAverage(mu)  << "   "
        << gMax(mu)      << endl;
}
