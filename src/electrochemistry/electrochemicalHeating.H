 
//  code chaged and adapted to pemfc (J.P.Kone, 19.07.2016)

    // Calculating volumetric electrochamical heating

    // Calculating volumetric electrochamical heating
    // Tsource = (I/tMEA)*[eta - (T*DeltaS/n*F)]
    // I = current density
    // tMEA = thickness of membrane electrode assembly
    // eta = overpotentials
    //     = etaAct + etaCon + etaOhm
    // etaAct = activation overpotential
    // etaCon = conenctration overpotential
    // etaOhm = ohmic overpotential
    // T = temperature
    // DeltaS = entropy change
    // n = number of electron transferred
    // F = Faraday's constant

    Info << nl << "Calculating electrochemical heating" << endl;

    // iEA applied in cell volumes (not patch)
    scalarField iEA = idensity.ref();

    //scalar Tr = 298.15;  //reference Temperature [K] for enthalpy calculations

    //fuel side accumulators
    scalarField sReactSrcFuel(cathodeT.size(),0);
    scalarField sProdSrcFuel(cathodeT.size(),0);

    PtrList<scalarField> sSpFuel(fuelSpecies.size()); // species entropies

    forAll(fuelSpecies, i)
    {
        if(fuelSpecies[i].ne() != 0)
        {
            polyToddYoung& CpI = molarCpFuel[i];          
            scalarField sSpI = CpI.polyIntS(Tr, cathodeT);
            sSpFuel.set(i, sSpI);

            if (fuelSpecies[i].rSign() > 0)
            {
                // product
               sProdSrcFuel += (fuelSpecies[i].sForm() + sSpI)/fuelSpecies[i].ne();
            }
            else if (fuelSpecies[i].rSign() < 0)
            {
                // reactant
                sReactSrcFuel += (fuelSpecies[i].sForm() + sSpI)/fuelSpecies[i].ne();
            }
            else
            {
                Info<< "ERROR: rSign = 0 but ne != 0" << nl;
            }
        }
    }

     /*----------------------------------------------------------------------- */

    // air side accumulators
    scalarField sReactSrcAir(cathodeT.size(),0);
    scalarField sProdSrcAir(cathodeT.size(),0);

    PtrList<scalarField> sSpAir(airSpecies.size());

    forAll(airSpecies, i)
    { 
        if(airSpecies[i].ne() != 0)
        {
            polyToddYoung& CpI = molarCpAir[i];        
            scalarField sSpI = CpI.polyIntS(Tr, cathodeT);
            sSpAir.set(i, sSpI);

            if (airSpecies[i].rSign() > 0) 
            {
                // product
                sProdSrcAir += (airSpecies[i].sForm() + sSpI)/airSpecies[i].ne();
            }
            else if (airSpecies[i].rSign() < 0)
            {
                // reactant
                sReactSrcAir += (airSpecies[i].sForm() + sSpI)/airSpecies[i].ne();
            }
            else
            {
                Info<< "ERROR: rSign = 0 but ne != 0" << nl;
            }
        }
    }

    scalar tMEA = tMem.value();		//Changed, reference height for heat source is membran thickness (S.S, 01.2021)

    scalarField volMolRate = iEA/F/tMEA; // [mol/(m^3 s)]
    
    scalarField anodeEntropyChange =
    (
        (sProdSrcFuel - sReactSrcFuel)
    );
    Info<< "anodeEntropyChange  min max: "
        << gMin(anodeEntropyChange) << " "
        << gMax(anodeEntropyChange) << endl;

    scalarField cathodeEntropyChange =
    (
        (sProdSrcAir - sReactSrcAir)
    );
    Info<< "cathodeEntropyChange  min max: "
        << gMin(cathodeEntropyChange) << " "
        << gMax(cathodeEntropyChange) << endl;

    scalarField totalEntropyChange =
    (
        anodeEntropyChange + cathodeEntropyChange
    );
    Info<< "totalEntropyChange  min max: "
        << gMin(totalEntropyChange) << " "
        << gMax(totalEntropyChange) << endl;

    scalarField entropySource(cathodePatch.size());
    entropySource =
    (
        -cathodeT*totalEntropyChange
    );

    scalarField& totalEntropySourceIn = totalEntropySource.ref();
    smearPatchToMesh smearentropySource(electrolyteMesh, electrolyteCathodeName);
    totalEntropySourceIn = smearentropySource.smear(entropySource);
    totalEntropySource.correctBoundaryConditions();
    
    Info<< "totalEntropySource  min max: "
        << gMin(totalEntropySource.ref()) << " "
        << gAverage(totalEntropySource.ref()) << "  "
        << gMax(totalEntropySource.ref()) << endl;

    scalarField& electrochemicalHeatingIn = electrochemicalHeating.ref();
    electrochemicalHeatingIn =
    (
        (iEA*(etaA.ref()+ etaC.ref() + etaOhmic.ref()))/tMEA //Changed, etaAct on anode and cathode side  (K.W 2020)
        + totalEntropySource.ref()*volMolRate // this should not be the first term
    );
 
    electrochemicalHeating.correctBoundaryConditions();

    Info << "min,mean,max(electrochemicalHeating): "
         << gMin(electrochemicalHeating.ref()) << "  "
         << gAverage(electrochemicalHeating.ref()) << "  "
         << gMax(electrochemicalHeating.ref()) << endl;

