// Code changed and adapted to pemfc electrochemistry (J.P.Kone, 26.05.2016)

// Nernst Eqn
// E = E0 - RT/(zF) ln(Q)
// where
//    E = cell potential
//    E0 = standard cell potential at temperature of interest, T
//       = -deltaG_rxn/(zf)
//         where deltaG_rxn = Gibbs free energy of reaction at temperature T
//    R = universal gas constant
//    F = Faraday constant
//    Q = reaction quotient
//    z = moles of electrons transferred
//
// for reaction Sum(ai Ri) = Sum(bj Pj)
//
//    Q = Qrxn = product([Pj]^bj)/product([Ri]^ai)
//               where [X] denotes moleFraction of X
//
//    deltaG_rxn = deltaH_rxn - T*deltaS_rxn
//
//        deltaH_rxn = sum(bj*deltaH(Pj)) - sum(ai*deltaH(Ri)
//
//            deltaH(X) = deltaH_f^0(X) + intgral_T0^T(Cp_X(T)dT)
//
//        deltaS_rxn = sum(bj*S(Pj)) - sum(ai*S(Ri))
//
//            S(X) = S^0(X) + intgral_T0^T(Cp_X(T)/T dT)
//
// eg for rxn
//                H2 + (1/2)O2 = H2O
// at T = 1000K,
// using tabulated gas phase thermochemistry data from NIST chemistry web book
// http://webbook.nist.gov/chemistry/ (accessed 2012 july 10)
// we find
//                deltaH_rxn = -247.86 kJ/mol  (ie, per mole of H2)
//                deltaS_rxn = -0.0553 kJ/(mol K)
//                deltaG_rxn = deltaH_rxn - T*deltaS_rxn = -192.56 kJ/mol
//


    //scalar Tr = 298.15;    // reference temperature

    // initialize Qrxn, deltaHrxn, deltaSrxn

    scalarField Qrxn(cathodeT.size(), 1);
    scalarField deltaHrxn(cathodeT.size(), 0);
    scalarField deltaSrxn(cathodeT.size(), 0);

    // update with air reactants and products
    forAll(airSpecies, i)
    {      
        if (airSpecies[i].rSign() == 1) // S.Keuler (Dec. 2013): allow reactions on airside too
        {
            // product
            scalar stoiCoeffI = rxnSpCoef[airSpecies[i].name()];

            Qrxn *= Foam::pow(xAir[i], stoiCoeffI);

            scalarField Hi = stoiCoeffI*
            (
                 airSpecies[i].hForm()
                 + molarCpAir[i].polyInt(Tr, cathodeT)
            );
            deltaHrxn += Hi;

            scalarField Si = stoiCoeffI*
            (
                 airSpecies[i].sForm()
                 + molarCpAir[i].polyIntS(Tr, cathodeT)
            );
            deltaSrxn += Si;

            if (firstTime)
            {
                Info << airSpecies[i].name() << "  " << stoiCoeffI << nl;
                Info<< "   mean H(T) = " << gAverage(Hi) << nl;
                Info<< "   mean S(T) = " << gAverage(Si) << nl;
            }
        } 
        else if (airSpecies[i].rSign() == -1)
        {
            scalar stoiCoeffI = rxnSpCoef[airSpecies[i].name()];

            Qrxn /= Foam::pow(xAir[i], stoiCoeffI);

            scalarField Hi = stoiCoeffI*
            (
                 airSpecies[i].hForm()
                 + molarCpAir[i].polyInt(Tr, cathodeT)
            );
            deltaHrxn -= Hi;

            scalarField Si = stoiCoeffI*
            (
                 airSpecies[i].sForm()
                 + molarCpAir[i].polyIntS(Tr, cathodeT)
            );
            deltaSrxn -= Si;

            if (firstTime)
            {
                Info << airSpecies[i].name() << "  " << stoiCoeffI << nl;
                Info<< "   mean H(T) = " << gAverage(Hi) << nl;
                Info<< "   mean S(T) = " << gAverage(Si) << nl;
            }
        }
    }

    // update with fuel reactants and products
    forAll(fuelSpecies, i)
    {
        if (fuelSpecies[i].rSign() == 1)
        {
            // product
            scalar stoiCoeffI = rxnSpCoef[fuelSpecies[i].name()];

            Qrxn *= Foam::pow(xFuel[i], stoiCoeffI);

            scalarField Hi = stoiCoeffI*
            (
                 fuelSpecies[i].hForm()
                 + molarCpFuel[i].polyInt(Tr, cathodeT)
            );
            deltaHrxn += Hi;

            scalarField Si = stoiCoeffI*
            (
                 fuelSpecies[i].sForm()
                 + molarCpFuel[i].polyIntS(Tr, cathodeT)
            );
            deltaSrxn += Si;

            if (firstTime)
            {
                Info << fuelSpecies[i].name() << "  " << stoiCoeffI << nl;
                Info<< "   mean H(T) = " << gAverage(Hi) << nl;
                Info<< "   mean S(T) = " << gAverage(Si) << nl;
            }
        }
        else if (fuelSpecies[i].rSign() == -1)
        {
            // reactant
            scalar stoiCoeffI = rxnSpCoef[fuelSpecies[i].name()];

            Qrxn /= Foam::pow(xFuel[i], stoiCoeffI);

            scalarField Hi = stoiCoeffI*
            (
                 fuelSpecies[i].hForm()
                 + molarCpFuel[i].polyInt(Tr, cathodeT)
            );
            deltaHrxn -= Hi;

            scalarField Si = stoiCoeffI*
            (
                 fuelSpecies[i].sForm()
                 + molarCpFuel[i].polyIntS(Tr, cathodeT)
            );
            deltaSrxn -= Si;
            if (firstTime)
            {
                Info << fuelSpecies[i].name() << "  " << stoiCoeffI << nl;
                Info<< "   mean H(T) = " << gAverage(Hi) << nl;
                Info<< "   mean S(T) = " << gAverage(Si) << nl;
            }
        }
    }

    // Gibbs
    // -----
    scalarField deltaGrxn = deltaHrxn - cathodeT*deltaSrxn;

    if (firstTime)
    {
        Info<< " mean deltaHrxn = " << gAverage(deltaHrxn) << nl
            << " mean cathodeT  = " << gAverage(cathodeT)    << nl
            << " mean deltaSrxn = " << gAverage(deltaSrxn) << nl
            << " mean TdeltaS   = " << gAverage(cathodeT*deltaSrxn) << nl
            << " mean deltaG    = " << gAverage(deltaGrxn) << nl
        << endl;
    }

    //scalar zRxn = rxnSpCoef["e"];


    // Nernst
    // ------
    scalarField Nernst =
        (-deltaGrxn - Rgas*cathodeT*Foam::log(Qrxn))/(rxnSpCoef["e"]*F);

    Info<< "min,mean,max Nernst = " << gMin(Nernst) << "  "
        << gAverage(Nernst) << "  " << gMax(Nernst) << nl;

