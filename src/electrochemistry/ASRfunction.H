// Area Specific Resistance function (J.P.Kone, 17.10.2016)

scalarField R(cathodePatch.size()); //ohm*m^2

if (fuelCellType == "PEMFC")
{
    // membrane ionic conductivity
    #include "sigmaElectrolyte.H"

    // ionic resistance (in the membrane)
	//Calculation corrected 2021/04/21 S. Schneider
	scalarField RiMem(cathodePatch.size());
	forAll (RiMem, facei)
    {
        RiMem[facei] = 1/sigmaElectrolyte[facei] * tMem.value();
    }
    
    Info<< "min,mean,max (RiMem) = " 
        << gMin(RiMem) << "  "
        << gAverage(RiMem) << "  " 
        << gMax(RiMem) << nl;
        //<< sumRiMem << nl;

    // electronic resistance (in the GDL)
    // Define different properties for substrate and MPL (K.W 2020)
    scalar ReMpl = 2.0*tMpl.value()*(1.0/(kappaMpl.value()*epsilonMpl.value()));
    scalar ReSub = 2.0*tSub.value()*(1.0/(kappaSub.value()*epsilonSub.value()));


    // electronic resistance (in the catalyst layer)
    scalar ReCl = 2.0*tCl.value()*(1.0/(kappaCl.value()*epsilonCl.value()));

    // contact resistance (between gas diffusion layer and bipolar plate) 
    scalar Rc = 2*rContact.value();

    // electronic resistance (in the bipolar plate)
    scalar ReBp = 2.0*tBp.value()*(1.0/(kappaBp.value()));

    // total cell internal resistance (changed K.W 2020)
    R = RiMem + ReMpl + ReSub + ReCl + Rc +ReBp;
}
else 
{
    FatalError
	<< "not available fuel cell type! (available==>PEMFC)" <<nl
	<< exit (FatalError);
}

Info << "min,mean,max (ASR)= "
     << gMin(R) <<" "
     << gAverage(R) << " "
     << gMax(R) << nl;
