// calculating local current densities (J.P.Kone, 17.10.2016)

// from
// E_cell = E_Nernst - I*ASR - (R*T/alpha*F*ln(I/I0)) - (B*ln(IL/(IL-I)))
// where
// E_cell = cell potential
// E_Nernst = Nernst potential
// I = local current density
// ASR = area specific resistance
// R = gas constant
// T = temperature
// alpha = transfer coefficient
// F = Faraday's constant
// I0 = exchange current density
// IL = limiting current density

//Adapted for Butler-Volmer equation on anode ande cathode side (K.W, 2020)

scalarField i(cathodePatch.size());	//changed I to i (K.W 2020)
{
    scalar A;
	scalar C;
    scalar r0 = 1.0;
    scalar r1 = 0.0;
	scalar conC;
	scalar conA;
	scalar iLeakValue;
    scalar fRiddersValue;

    forAll (i, facei)
    {
		A = (Rgas*cathodeT[facei])/(2*alphaCathodeElectrolyte.value()*F);
		C = (Rgas*anodeT[facei])/(2*alphaCathodeElectrolyte.value()*F);
		
		conC = Foam::pow(conCathode[facei]/refConC.value(), conExponentC.value());
		conA = Foam::pow(conAnode[facei]/refConA.value(), conExponentA.value());
		
		iLeakValue = iLeak.value();
        
        
		
        //r1 = iLCathode[facei]- 1.0; // (J.P.Kone, 06.07.2017)
        //r1 = iLCathode[facei] * 10.0;
        fRiddersValue = fRidders.value();
        r1 = iLCathode[facei] * fRiddersValue;
        //r1 = gMax(iLCathode);
        //Info <<nl<<"r1 = " <<r1 <<endl;
               
        funcTotalCurrent ftc
        (
            Nernst[facei], 
            voltage.boundaryFieldRef()[electrolyteCathodeID][facei],
            R[facei],
            A,
            i0Cathode[facei],
			conC,
			C,
			i0Anode[facei],
			conA,
			iLeakValue
        );

        i[facei] = RiddersRoot<funcTotalCurrent>(ftc, 1.0e-5).root(r0,r1);
	}


}
