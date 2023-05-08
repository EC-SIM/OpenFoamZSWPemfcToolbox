
    // calculate mole fraction fields
    // ------------------------------

    // sum Y[i]/MW[i]
    // --------------
    sumFuelYIonMI = 0;
    forAll(fuelSpecies, i)
    {
        sumFuelYIonMI += Yfuel[i]/fuelSpecies[i].MW();
    }


    // X[i]
    // ---
    forAll(fuelSpecies, i)
    {
        Xfuel[i] = Yfuel[i]/fuelSpecies[i].MW()/sumFuelYIonMI;
    }

