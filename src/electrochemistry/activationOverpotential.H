//    Calculating activation overpotential via inverse Butler-Volmer-Equation

//    Activation term prescribed by Butler-Volmer Equation:
//    i = i0[exp(2*alpha*F*eta/RT-exp(-2*(1-alpha)*F*eta/RT)]
//    eta = Activation overpotential obtained by numerical iversion of BV eqn.
//    i = local current density
//    i0 = exchange current density
//    alpha = transfer coefficient 
//    R = universal gas constant
//    F = Faraday constant

//
//    initialize etaAnode


scalarField etaAnode(anodePatch.size(), 0);
{
    etaAnode = 
    (
        ((Rgas*anodeT)
        /(2*alphaCathodeElectrolyte.value()*F))
        *Foam::asinh(i)/(2*i0Anode)/(Foam::pow(conAnode/refConA.value(), conExponentA.value()))	//Adapted concentration dependency S.S. 17.05.2021
    );

    Info<< "min,mean,max (etaAnode) = " << gMin(etaAnode) << "  "
        << gAverage(etaAnode) << "  " << gMax(etaAnode) << nl;
}	
		
scalarField etaCathode(cathodePatch.size());
{
    // Compute cathodic overpotential
    etaCathode = 
    (
        ((Rgas*cathodeT)
        /(2*alphaCathodeElectrolyte.value()*F))
        *Foam::asinh((i+iLeak.value())/(2*i0Cathode)/(Foam::pow(conCathode/refConC.value(), conExponentC.value())))
    );

    Info<< "min,mean,max (etaCathode) = " << gMin(etaCathode) << "  "
        << gAverage(etaCathode) << "  " << gMax(etaCathode) << nl;
}

// End activation calculation

//  Save data for output

    scalarField& etaAIn = etaA.ref();
    smearPatchToMesh smearetaA(electrolyteMesh, electrolyteCathodeName);
    etaAIn = smearetaA.smear(etaAnode);
    etaA.correctBoundaryConditions();
    etaA.boundaryFieldRef()[electrolyteAnodeID] == etaAnode;

    scalarField& etaCIn = etaC.ref();
    smearPatchToMesh smearetaC(electrolyteMesh, electrolyteCathodeName);
    etaCIn = smearetaC.smear(etaCathode);
    etaC.correctBoundaryConditions();
    etaC.boundaryFieldRef()[electrolyteCathodeID] == etaCathode;



