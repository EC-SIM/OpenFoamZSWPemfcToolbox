// obtain k(air mixture) from k(species components)

// Code changed and adapted to pemfc (J.P.Kone, 19.07.2016)
// for coversion to openfoam-4.0
// call to 'internalField()' is changed to 'ref()'

{
    scalarField& kAIn = kAir.ref(); 
    kAIn = 0;

    // initially using a simple linear combination weighted by mass fraction

    forAll(airSpecies, I)
    {
        scalarField kI = kAirL[I].polyVal(Tair.ref());
        kAIn += kI*Yair[I].ref();
    }
    kAIn *= 1e-2;   // k=0.01*(ToddYoung polynomial value)
                    // Table 3, J. Power Sources 110 (2002) 186-200 (page 190)

    kAir.correctBoundaryConditions();

    Info << "min,mean,max(kAir): "
         << gMin(kAIn)      << "   "
         << gAverage(kAIn)  << "   "
         << gMax(kAIn)      << endl;
}
