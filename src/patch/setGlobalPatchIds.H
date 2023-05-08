    // Code changed and adapted to pemfc (J.P.Kone, 13.06.2016)

    Info<< "Setting special patch IDs\n" << endl;

    // IDs of the named patches
    // ------------------------

    label anodeID = fuelMesh.boundaryMesh().findPatchID(anodeName);
    if (anodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << anodeName << " on the fuel mesh."
            << abort(FatalError);
    }

    label fuelInletID = fuelMesh.boundaryMesh().findPatchID(fuelInletName);
    if (fuelInletID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << fuelInletName << " on the fuel mesh."
            << abort(FatalError);
    }

    label fuelOutletID = fuelMesh.boundaryMesh().findPatchID(fuelOutletName);
    if (fuelOutletID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << fuelOutletName << " on the fuel mesh."
            << abort(FatalError);
    }

    // added (J.P. Kone, 13.06.2016)
    label fuelAbpID = fuelMesh.boundaryMesh().findPatchID(fuelAbpName);
    if (fuelAbpID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << fuelAbpName << " on the fuel mesh."
            << abort(FatalError);
    }


    label cathodeID = airMesh.boundaryMesh().findPatchID(cathodeName);
    if (cathodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << cathodeName << " on the air mesh."
            << abort(FatalError);
    }

    label airInletID = airMesh.boundaryMesh().findPatchID(airInletName);
    if (airInletID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << airInletName << " on the air mesh."
            << abort(FatalError);
    }

    label airOutletID = airMesh.boundaryMesh().findPatchID(airOutletName);
    if (airOutletID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << airOutletName << " on the air mesh."
            << abort(FatalError);
    }

    // added (J.P. Kone, 13.06.2016)
    label airCbpID = airMesh.boundaryMesh().findPatchID(airCbpName);
    if (airCbpID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << airCbpName << " on the air mesh."
            << abort(FatalError);
    }


    label electrolyteAnodeID =
        electrolyteMesh.boundaryMesh().findPatchID(electrolyteAnodeName);
    if (electrolyteAnodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << electrolyteAnodeName
            << " on the electrolyte mesh."
            << abort(FatalError);
    }

    label electrolyteCathodeID =
        electrolyteMesh.boundaryMesh().findPatchID(electrolyteCathodeName);
    if (electrolyteCathodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << electrolyteAnodeName
            << " on the electrolyte mesh."
            << abort(FatalError);
    }

    // added (J.P. Kone, 13.06.2016)
    label abpFuelID = abpMesh.boundaryMesh().findPatchID(abpFuelName);
    if (abpFuelID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << abpFuelName
	    << " on the anode bipolarPlate mesh."
            << abort(FatalError);
    }

    // added (J.P. Kone, 13.06.2016)
    label cbpAirID = cbpMesh.boundaryMesh().findPatchID(cbpAirName);
    if (cbpAirID == -1)
    {
        FatalErrorIn(args.executable())
             << "Cannot find patch " << cbpAirName
	     << " on the cathode bipolarPlate mesh."
             << abort(FatalError);
        }

    // added (K.W, 28.03.2020)
    label coolantAbpID = coolantMesh.boundaryMesh().findPatchID(coolantAbpName);
    if (coolantAbpID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << coolantAbpName
	    << " on the anode coolant mesh."
            << abort(FatalError);
    }


    // added (K. W, 28.03.2020)
    label abpCoolantID = abpMesh.boundaryMesh().findPatchID(abpCoolantName);
    if (abpCoolantID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << abpCoolantName
	    << " on the anode coolant bipolarPlate mesh."
            << abort(FatalError);
    }

 // added (K.W, 28.03.2020)
    label coolantCbpID = coolantMesh.boundaryMesh().findPatchID(coolantCbpName);
    if (coolantCbpID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << coolantCbpName
	    << " on the cathode coolant mesh."
            << abort(FatalError);
    }


    // added (K.W, 28.03.2020)
    label cbpCoolantID = cbpMesh.boundaryMesh().findPatchID(cbpCoolantName);
    if (cbpCoolantID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << cbpCoolantName
	    << " on the cathode coolant bipolarPlate mesh."
            << abort(FatalError);
    }

    // added (K.W, 09.06.2020)
    label coolantInletID = coolantMesh.boundaryMesh().findPatchID(coolantInletName);
    if (coolantInletID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << coolantInletName << " on the coolant mesh."
            << abort(FatalError);
    }

    // added (K.W, 09.06.2020)
    label coolantOutletID = coolantMesh.boundaryMesh().findPatchID(coolantOutletName);
    if (coolantOutletID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << coolantOutletName << " on the coolant mesh."
            << abort(FatalError);
    }

  
