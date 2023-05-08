// Code changed and adapted to pemfc (J.P.Kone, 22.11.2016)
// for coversion to openfoam-5.0
// call to 'internalField()' is changed to 'ref()'

{
    // Add coolant channel (K.W 2020)
    {
        forAll (TCoolant, cellI)
        {
            TCoolant[cellI] = Tcell[coolantCellMap[cellI]];
        }
        TCoolant.correctBoundaryConditions();
    }

    // anode bipolar plate
    {
        forAll (TanodeBP, cellI)
        {
            TanodeBP[cellI] = Tcell[abpCellMap[cellI]];
        }
        TanodeBP.correctBoundaryConditions();
    }

    // Fuel
    {
        forAll (Tfuel, cellI)
        {
            Tfuel[cellI] = Tcell[fuelCellMap[cellI]];
        }
        Tfuel.correctBoundaryConditions();
    }

    // electrolyte
    {
        forAll (Telectrolyte, cellI)
        {
            Telectrolyte[cellI] = Tcell[electrolyteCellMap[cellI]];
        }
        Telectrolyte.correctBoundaryConditions();
    }

    // Air
    {
        forAll (Tair, cellI)
        {
            Tair[cellI] = Tcell[airCellMap[cellI]];
        }
        Tair.correctBoundaryConditions();
    }


    // cathode bipolar plate
    {
        forAll (TcathodeBP, cellI)
        {
            TcathodeBP[cellI] = Tcell[cbpCellMap[cellI]];
        }
        TcathodeBP.correctBoundaryConditions();
    }


    //(K.W 2020)
    Info << "TCoolant min mean max = "
         << Foam::gMin(TCoolant.ref())     << "    "
         << Foam::gAverage(TCoolant.ref()) << "    "
         << Foam::gMax(TCoolant.ref())
         << endl;

    Info << "Telectrolyte min mean max = "
         << Foam::gMin(Telectrolyte.ref())     << "    "
         << Foam::gAverage(Telectrolyte.ref()) << "    "
         << Foam::gMax(Telectrolyte.ref())
         << endl;


    Info << "Tfuel min mean max = "
         << Foam::gMin(Tfuel.ref())     << "    "
         << Foam::gAverage(Tfuel.ref()) << "    "
         << Foam::gMax(Tfuel.ref())
         << endl;

    Info << "Tair min mean max  = "
         << Foam::gMin(Tair.ref())     << "    "
         << Foam::gAverage(Tair.ref()) << "    "
         << Foam::gMax(Tair.ref())
         << endl;
}

