// obtain k(fuel mixture) from k(species components)

// Code changed and adapted to pemfc (J.P.Kone, 19.07.2016)
// for coversion to openfoam-4.0
// call to 'internalField()' is changed to 'ref()'

{
    scalarField& kFIn = kFuel.ref(); 
    kFIn = 0;

    // initially using a simple linear combination weighted by mass fraction

    forAll(fuelSpecies, I)
    {
        scalarField kI = kFuelL[I].polyVal(Tfuel.ref());
        kFIn += kI*Yfuel[I].ref();
    }
    kFIn *= 1e-2;   // k=0.01*(ToddYoung polynomial value)
                    // Table 3, J. Power Sources 110 (2002) 186-200 (page 190)

    kFuel.correctBoundaryConditions();

    Info << "min,mean,max(kFuel): "
         << gMin(kFIn)      << "   "
         << gAverage(kFIn)  << "   "
         << gMax(kFIn)      << endl;
}
