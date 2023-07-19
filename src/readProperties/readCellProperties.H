    // Code changed and adapted to pemfc (J.P.Kone, 13.06.2016)

    Info<< "Reading cell properties\n" << endl;

    //Changed K.W.
    IOdictionary cellProperties
    (
        IOobject
        (
            "cellProperties",
            runTime.constant(),
            runTime,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    word fuelCellType(cellProperties.lookup("fuelCellType"));		//Qing, 04.06.2014
    dimensionedScalar PreIterAct(cellProperties.lookup("PreIterAct"));	//Qing, 22.06.2014

    Switch galvanostatic(cellProperties.lookup("galvanostatic"));
    dimensionedScalar V(cellProperties.lookup("voltage"));
    dimensionedScalar Ibar0(cellProperties.lookup("Ibar0"));
    dimensionedScalar Tinit(cellProperties.lookup("Tinit"));
    dimensionedScalar TinitCoolant(cellProperties.lookup("TinitCoolant")); //K.W 28.03.2020
    dimensionedScalar numCells(cellProperties.lookup("numCells"));
    dimensionedScalar Rhat("Rhat", dimensionSet(1, 4, -3, 0, 0, -2, 0), 0);
    dimensionedScalar kappaCl(cellProperties.lookup("kappaCl"));
    dimensionedScalar kappaMpl(cellProperties.lookup("kappaMpl"));	//K.W 2020
    dimensionedScalar kappaSub(cellProperties.lookup("kappaSub"));	//K.W 2020
    dimensionedScalar rContact(cellProperties.lookup("rContact"));
    dimensionedScalar kappaBp(cellProperties.lookup("kappaBp"));
    dimensionedScalar tCl(cellProperties.lookup("tCl"));
    dimensionedScalar tMpl(cellProperties.lookup("tMpl"));	//K.W 2020
    dimensionedScalar tSub(cellProperties.lookup("tSub"));	//K.W 2020
    dimensionedScalar tBp(cellProperties.lookup("tBp"));
    dimensionedScalar epsilonMpl(cellProperties.lookup("epsilonMpl"));	//K.W 2020
    dimensionedScalar epsilonSub(cellProperties.lookup("epsilonSub"));	//K.W 2020
    dimensionedScalar epsilonCl(cellProperties.lookup("epsilonCl"));


    // echo inputs from dictionary to stdout
    // -------------------------------------
    if (galvanostatic)
    {
        Info<< "galvanostatic run" << nl;
        Info<< "    V     = " << V << nl;
        Info<< "    Ibar0 = " << Ibar0 << nl;

        Rhat = cellProperties.lookup("Rhat");
        Info<< "    Rhat  = " << Rhat << nl;
    }
    else
    {
        Info<< "potentiostatic run" << nl;
        Info<< "    Ibar0 = " << Ibar0 << nl;
        Info<< "    V     = " << V << nl;
    }

    Info<< "CL electronic conductivity(kappaCl) = " << kappaCl << nl
        << "MpL electronic conductivity(kappaMpl) = " << kappaMpl << nl	//K.W 2020
        << "Sub electronic conductivity(kappaSub) = " << kappaSub << nl	//K.W 2020
        << "GDL - BP Contact resistance (rContact)= " << rContact << nl
        << "BP electronic conductivity(kappaBp) = " << kappaBp << nl
        << "CL thickness (tCl) = " << tCl << nl
        << "MPL thickness (tMpl) = " << tMpl << nl
        << "Sub thickness (tSub) = " << tSub << nl
        << "BP thickness (tBp) = " << tBp << nl
        << endl;

    Info<< "    Tinit = " << Tinit << nl;
    Info<< endl;

    Info<< "    Tinit Coolant = " << TinitCoolant << nl;	//K.W 2020
    Info<< endl;


    // special patch names
    // -------------------
    word anodeName(cellProperties.lookup("anodePatch"));
    word fuelInletName(cellProperties.lookup("fuelInletPatch"));
    word fuelOutletName(cellProperties.lookup("fuelOutletPatch"));
    word fuelAbpName(cellProperties.lookup("fuelAbpPatch")); // added (J.P. Kone, 13.06.2016)

    word cathodeName(cellProperties.lookup("cathodePatch"));
    word airInletName(cellProperties.lookup("airInletPatch"));
    word airOutletName(cellProperties.lookup("airOutletPatch"));
    word airCbpName(cellProperties.lookup("airCbpPatch")); // added (J.P. Kone, 13.06.2016)

    word electrolyteAnodeName
        (cellProperties.lookup("electrolyteAnodePatch"));

    word electrolyteCathodeName
        (cellProperties.lookup("electrolyteCathodePatch"));

    word abpFuelName
            (cellProperties.lookup("abpFuelPatch")); // added (J.P. Kone, 13.06.2016)

    word cbpAirName
                (cellProperties.lookup("cbpAirPatch")); // added (J.P. Kone, 13.06.2016)

    word coolantAbpName
                (cellProperties.lookup("coolantAbpPatch")); // added (K.W, 28.03.2020)
    word coolantCbpName
                (cellProperties.lookup("coolantCbpPatch")); // added (K.W, 28.03.2020)
    word abpCoolantName
                (cellProperties.lookup("abpCoolantPatch")); // added (K.W, 28.03.2020)
    word cbpCoolantName
                (cellProperties.lookup("cbpCoolantPatch")); // added (K.W, 28.03.2020)


    word coolantInletName(cellProperties.lookup("coolantInletPatch"));  // added (K.W, 19.04.2020)
    word coolantOutletName(cellProperties.lookup("coolantOutletPatch")); // added (K.W, 19.04.2020)


    // echo inputs from dictionary to stdout
    Info
        << "    anodeName              = " << anodeName << nl
        << "    fuelInletName          = " << fuelInletName << nl
        << "    fuelOutletName         = " << fuelOutletName << nl
		<< "    fuelAbpName            = " << fuelAbpName << nl // added (J.P. Kone, 13.06.2016)
        << "    cathodeName            = " << cathodeName << nl
        << "    airInletName           = " << airInletName << nl
        << "    airOutletName          = " << airOutletName << nl
		<< "    airCbpName             = " << airCbpName << nl // added (J.P. Kone, 13.06.2016)
        << "    electrolyteAnodeName   = " << electrolyteAnodeName << nl
        << "    electrolyteCathodeName = " << electrolyteCathodeName << nl
		<< "    abpFuelName            = " << abpFuelName << nl // added (J.P. Kone, 13.06.2016)
		<< "    cbpAirName             = " << cbpAirName << nl // added (J.P. Kone, 13.06.2016)
		<< "    coolantCbpName         = " << coolantCbpName << nl // added (K.W, 28.03.2020)
		<< "    coolantAbpName         = " << coolantAbpName << nl // added (K.W, 28.03.2020)
		<< "    cbpCoolantName         = " << cbpCoolantName << nl // added (K.W, 28.03.2020)
		<< "    abpCoolantName         = " << abpCoolantName << nl // added (K.W, 28.03.2020)
		<< "    coolantInletName       = " << coolantInletName << nl // added (K.W, 19.04.2020)
		<< "    coolantOutletName      = " << coolantOutletName << nl// added (K.W, 19.04.2020)      
		<< endl;

