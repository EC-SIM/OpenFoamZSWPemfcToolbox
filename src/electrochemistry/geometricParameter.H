// Code changed and adapted (J.P.Kone, 09.10.2016)
// ------------------------------------------------------------------
// compute thickness of electrolyte as
//
//     (electrolyte volume)/(average electrode interface area)
//
// Using average area of both electrode interfaces accommodates both
// planar and cylindrical cells.
// ------------------------------------------------------------------

dimensionedScalar tMem("tMem", dimLength, 0);    //construct dimensioned thickness
scalar anodeArea;
scalar cathodeArea;
scalar electrolyteVolume;
{
    // anode side area
    anodeArea = Foam::gSum
    (
        electrolyteMesh.magSf().boundaryField()[electrolyteAnodeID]
    );

    // cathode side area
    cathodeArea = Foam::gSum
    (
        electrolyteMesh.magSf().boundaryField()[electrolyteCathodeID]
    );

    // volume
    electrolyteVolume = Foam::gSum
    (
        electrolyteMesh.V()
    );

    // thickness value
    tMem.value() = 
    (
        electrolyteVolume/(0.5*(anodeArea + cathodeArea))
    );
}

Info<< "    electrolyte thickness (calculated) tMem = " << tMem << nl << endl;
