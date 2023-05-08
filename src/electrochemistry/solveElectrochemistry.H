// Code changed and adapted to pemfc

// for coversion to openfoam-4.0
// call to 'boundaryField()' is changed to 'boundaryFieldRef()' for all volScalarFields

{
    Info << nl << "Solving electrochemistry" << endl;

    const scalar F = physicalConstant::F.value();         // C/mol
    const scalar FF = physicalConstant::FF.value();       // C/kmol
    const scalar Rgas = physicalConstant::Rgas.value();   // J/(mol K)

    const scalar Tr = 298.15;  //reference Temperature [K] for thermodynamics

    // Interpolate global temperature to electrolyte/fuel interface
    // ------------------------------------------------------------
    scalarField anodeT(electrolyteAnodePatch.size());
    {
        surfaceScalarField Ts = fvc::interpolate(Tcell);

        forAll (anodeT, faceI)
        {
            anodeT[faceI] = Ts[electrolyteAnodeMask[faceI]];
        }
    }

    Info << "min,max(anodeT): "
         << Foam::gMin(anodeT) <<" , "  //S.Keuler, Bugfix for consistent output (parallel simulation) [two lines]
         << Foam::gMax(anodeT) << endl;

    // Interpolate global temperature to electrolyte/air interface
    // ------------------------------------------------------------
    scalarField cathodeT(electrolyteCathodePatch.size());
    {
        surfaceScalarField Ts = fvc::interpolate(Tcell);

        forAll (cathodeT, faceI)
        {
            cathodeT[faceI] = Ts[electrolyteCathodeMask[faceI]];
        }
    }

    Info << "min,max(cathodeT): "
         << Foam::gMin(cathodeT) <<" , " 
         << Foam::gMax(cathodeT) << endl;
    

    // Interpolate anode mole fractions of fuel reactant species to cathode
    // --------------------------------------------------------------
    scalarField sumFYjOnMj    //sum(Yj/MWj)
    (
        Yfuel[0].boundaryFieldRef()[anodeID].size(),
        0
    );

    forAll(Yfuel, j)
    {
      sumFYjOnMj += Yfuel[j].boundaryFieldRef()[anodeID]/fuelSpecies[j].MW();
    }

    PtrList<scalarField> xFuel(fuelSpecies.size());
    const scalarField& pFuelPatch = pFuel.boundaryFieldRef()[anodeID]; //added hj 03-09-2013
    forAll(fuelSpecies,s)
    {
        if(fuelSpecies[s].rSign() != 0)    //reactant or product
        {
            xFuel.set
            (
                s,
                new scalarField(cathodeT.size(), 0)
            );

            xFuel[s] =
			anodeToCathode.faceInterpolate
                 (
                     Yfuel[s].boundaryFieldRef()[anodeID]/fuelSpecies[s].MW()
                     /sumFYjOnMj
                 );

            // ensure positivity
            xFuel[s] = Foam::max(xFuel[s], Foam::doubleScalarSmall);

            Info << "min,mean,max(X" << fuelSpecies[s].name() << "): "
                 << Foam::gMin(xFuel[s]) <<" , "
                 << Foam::gAverage(xFuel[s]) <<" , "
                 << Foam::gMax(xFuel[s]) << endl;
        }
        // for calculating water activity in the anode side 
        else
        {
            xFuel.set
            (
                s,
                new scalarField(anodeT.size(), 0)
            );
            xFuel[s] = 
            (
                Yfuel[s].boundaryFieldRef()[anodeID]/fuelSpecies[s].MW()
                /sumFYjOnMj
            );
        }     
    }


    // Calculate cathode mole fractions of oxidant and product species
    // -------------------------------------------------------------------
    scalarField sumAYjOnMj    //sum(Yj/MWj)
    (
        Yair[0].boundaryFieldRef()[cathodeID].size(),
        0
    );

    forAll(Yair, j)
    {
        sumAYjOnMj += Yair[j].boundaryFieldRef()[cathodeID]/airSpecies[j].MW();
    }

    PtrList<scalarField> xAir(airSpecies.size());
    const scalarField& pAirPatch = pAir.boundaryFieldRef()[cathodeID]; //added Qing 04.06.2014
    forAll(airSpecies, s)
    {
        if (airSpecies[s].rSign() != 0)    //reactant or product (S.Keuler, corrected 30.01.2014)
        {
            xAir.set
            (
                s,
                new scalarField(cathodeT.size(), 0)
            );
            xAir[s] =
            (
                Yair[s].boundaryFieldRef()[cathodeID]/airSpecies[s].MW()
                /sumAYjOnMj
            );

            // ensure positivity
            xAir[s] = Foam::max(xAir[s], Foam::doubleScalarSmall);

            Info << "min,mean,max(X" << airSpecies[s].name() << "): "
                 << Foam::gMin(xAir[s]) <<" , "
                 << Foam::gAverage(xAir[s]) <<" , "
                 << Foam::gMax(xAir[s]) << endl;
        }
    }




    // Calculate Nernst potential
    #include "NernstEqn.H"

    // Takes voltage from cell center of cell adjacent to boundary face and applies it at the face   
    forAll(electrolyteCathodePatch, facei)
    {
	    voltage.boundaryFieldRef()[electrolyteCathodeID][facei] = voltage[electrolyteCathodePatch.faceCells()[facei]];
    }

    // area specific resistance, R
    // scalarField R is based on cathodeT
    #include "ASRfunction.H"

    // Calculate cathode and anode exchange current density (K.W. 2020)
    #include "exchangeCurrentDensity.H"   
    
   // Calculate concentration dependency
   #include "cathodeLimitingCurrentDensity.H"   
   
   #include "tafelConcentration.H" //(S.S. 17.05.2021)

   // Calculate total Current
   #include "totalCurrent.H"

   // Calculate activation overpotential (cahnged K.W 2020)
   #include "activationOverpotential.H"

   // Calculate ohmic overpotential   
	
    #include "ohmicOverpotential.H"

	// Smear boundary current and potential over electrolyte volume
    #include "idensity.H"

	// Calculate mean current density
    #include "solveCurrent.H"  

    // Calculate electrochemical heating
    #include "electrochemicalHeating.H"

    // Calculate species sources and sinks and set
    // boundary conditions for mass fractions and velocity
    // at fluid/electrode interfaces


    // Fuel side
    {
        Info<< "fuel species electrochemical fluxes and YEqn BCs" << nl;
   
        label nSpec = fuelSpecies.size();
        List<scalarField> mflux(nSpec);
        List<Switch> isFlux(nSpec, "false");
        scalarField mfluxSum(i.size(),0); // I is replaced with i (K. W, 2020)
        scalarField iLeakFlux(i.size(),iLeak.value()); //S.S. 11.06.2021

        // mass fluxes for electrochemically active species
        forAll(fuelSpecies, s)
        {
            if(fuelSpecies[s].ne() != 0)
            {
                mflux[s] =
                (
                    fuelSpecies[s].rSign()*fuelSpecies[s].MW()
                    *(i+iLeakFlux)/(FF*fuelSpecies[s].ne())// I is replaced with i (K.W, 2020), add iLeak (S.S. 11.06.2021)
                );
                mfluxSum += mflux[s];
                isFlux[s] = "true";
                //Info<< s << " mflux[s] " << Foam::gSum(mflux[s]) << nl;
            }
            
            // Calculation of multiple inert species in fuel (molecular charge) (S.S, 02.02.2021)
            else if (fuelSpecies[s].ne() == 0)
            {
				scalarField mzero(i.size(),0);
                mflux[s] = mzero;
            }
            
            Info<< "s species[] isFlux[] mflux[] = "
                << s << " " << fuelSpecies[s].name() << " " << isFlux[s] << " "
                << Foam::gSum(mflux[s]) << nl;
        }
        Info<< endl;


        // mass fraction BCs for all species except the "inert" specie
        // (inert specie mass fraction is 1-complement of all others)
        forAll(Yfuel, s)
        {
            //Info<< fuelSpecies[s].name() << nl;

            if(fuelSpecies[s].name() != fuelInertSpecie)
            {
                scalarField gammaAnodeS =
                (
                    diffSpFuel[s].boundaryFieldRef()[anodeID]
                    *rhoFuel.boundaryFieldRef()[anodeID]
                );

                // interface patchField
                volScalarField& Ys = Yfuel[s];
                fixedGradientFvPatchScalarField& YsBC =
                    refCast<fixedGradientFvPatchScalarField>
                    (
                        Ys.boundaryFieldRef()[anodeID] 
                    );

                // gradient boundary condition
                // note:  fuel mass fluxes defined on cathode patch
                //        -> we interpolate to anode patch
                //        -> through cathodeToAnode (J.P.Kone, 27.05.2016)

                // initialize gradient
                YsBC.gradient() = 
                	cathodeToAnode.faceInterpolate(mflux[s])*(1.0 - YsBC);
			

                // add changes due to other species
                forAll(Yfuel, t)
                {
                    if((t != s) && isFlux[t])
                    {
                        YsBC.gradient() -= 
                        	cathodeToAnode.faceInterpolate(mflux[t])*YsBC;
				
                    }
                }
                YsBC.gradient() /= gammaAnodeS;
            }
        }
        Info<< endl;


        // Set the interface velocity condition
        Ufuel.boundaryFieldRef()[anodeID] ==
        (
            -cathodeToAnode.faceInterpolate(mfluxSum)
            /rhoFuel.boundaryFieldRef()[anodeID]
            *(fuelMesh.Sf().boundaryField()[anodeID])
            /(fuelMesh.magSf().boundaryField()[anodeID])
        );

        #include "H2utilization.H"
    }

    // Air side
    {
        Info<< nl << "air species electrochemical fluxes and YEqn BCs" << nl;

        label nSpec = airSpecies.size();       //number of species
        List<scalarField> mflux(nSpec);        //mass flux by species
        List<Switch> isFlux(nSpec, "false");   //true if species contributes
        scalarField mfluxSum(i.size(),0);      //sum over species // 
		scalarField iLeakFlux(i.size(),iLeak.value()); //S.S. 11.06.2021

        // mass fluxes for electrochemically active species
        forAll(airSpecies, s)
        {
            if(airSpecies[s].ne() != 0)
            {
                mflux[s] =
                (
                    airSpecies[s].rSign()*airSpecies[s].MW()
                    *(i+iLeakFlux)/(FF*airSpecies[s].ne()) // I is replaced with i (K.W, 2020), add iLeak (S.S. 11.06.2021)
                );
                mfluxSum += mflux[s];
                isFlux[s] = "true";
            }
            Info<< "s species[] isFlux[] mflux[] = "
                << s << " " << airSpecies[s].name() << " " << isFlux[s] << " "
                << Foam::gSum(mflux[s]) << nl;
        }
        Info<< endl;


        // mass fraction BCs for all species except the "inert" specie
        // (inert specie mass fraction is 1-complement of all others)
        forAll(Yair, s)
        {
            //Info << airSpecies[s].name() << nl;

            if(airSpecies[s].name() != airInertSpecie)
            {
                scalarField gammaCathodeS =
                (
                    diffSpAir[s].boundaryFieldRef()[cathodeID]
                    *rhoAir.boundaryFieldRef()[cathodeID]
                );

                // interface patchField
                volScalarField& Ys = Yair[s];
                fixedGradientFvPatchScalarField& YsBC =
                    refCast<fixedGradientFvPatchScalarField>
                    (
                        Ys.boundaryFieldRef()[cathodeID] 
                    );

                // gradient boundary condition
                // initialize gradient
                YsBC.gradient() = mflux[s]*(1.0 - YsBC);

                // add changes due to other species
                forAll(Yair, t)
                {
                    if((t != s) && isFlux[t])
                    {
                       YsBC.gradient() -= YsBC*mflux[t];
                    }
                }
                YsBC.gradient() /= gammaCathodeS;
            }
        }
        Info<< endl;


        // Set the interface velocity condition
        Uair.boundaryFieldRef()[cathodeID] ==
        (
            -(mfluxSum)
            /rhoAir.boundaryFieldRef()[cathodeID]
            *(airMesh.Sf().boundaryField()[cathodeID])
            /(airMesh.magSf().boundaryField()[cathodeID])
        );

        #include "O2utilization.H"
    }
}

