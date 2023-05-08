
    // calculate mole fraction fields
    // ------------------------------

    // sum Y[i]/MW[i]
    // --------------
    sumAirYIonMI = 0;
    forAll(airSpecies, i)
    {
        sumAirYIonMI += Yair[i]/airSpecies[i].MW();
    }


    // X[i]
    // ---
    forAll(airSpecies, i)
    {
        Xair[i] = Yair[i]/airSpecies[i].MW()/sumAirYIonMI;
    }

