// Code changed and adapted to pemfc (J.P.Kone, 01.06.2016)

    Info<< nl << "Reading air pemfcSpeciesProperties" << nl;

    // pemfcSpecies info
    //-----------------
    IOdictionary airSpeciesDict
    (
        IOobject
        (
            "pemfcSpeciesProperties",
            runTime.constant(),
            airMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    // species names
    // -------------
    speciesTable airSpeciesNames(airSpeciesDict.lookup("speciesList"));

    //forAll(airSpeciesNames, i)
    //{
    //    Info<< "airSpeciesNames[" << i << "] " << airSpeciesNames[i] << nl;
    //}


    // species data
    // ------------
    PtrList<pemfcSpecie> airSpecies(airSpeciesNames.size());

    forAll(airSpeciesNames, i)
    {
        pemfcSpecie tmp(airSpeciesDict.lookup(airSpeciesNames[i]));
        airSpecies.set
        (
            i,
            new pemfcSpecie(tmp)
        );
    }


if (Pstream::myProcNo() == 0)
{
    // header
          //airSpecies[0] O2        31.9988        4        -1        0
    cout<< "              "
        << std::setiosflags(std::ios::left)
        << std::setw(8)
        << "name"
        << std::setw(16)
        << "MW[kg/kmol]"
        << std::setw(8)
        << "ne"
        << std::setw(8)
        << "rSign"
        << std::setw(12)
        << "hForm"
        << std::setw(8)
        << "sForm"
        << nl;

    forAll(airSpeciesNames, i)
    {
        cout<< "airSpecies[" << i    << "] "
            << std::setiosflags(std::ios::left)
            << std::setw(8)
            << airSpecies[i].name()
            << std::setw(16)
            << airSpecies[i].MW()
            << std::setw(8)
            << airSpecies[i].ne()
            << std::setw(8)
            << airSpecies[i].rSign()
            << std::setw(12)
            << airSpecies[i].hForm()
            << std::setw(8)
            << airSpecies[i].sForm()
            << nl;
    }
    Info << nl;
}


    // background specie for mass fraction calculations
    // -----------------
    // labelled inertSpecie but NOT necessarily chemically inert)
    //
    word airInertSpecie(airSpeciesDict.lookup("inertPemfcSpecie"));
    Info << "airInertSpecie = " << airInertSpecie << nl << endl;

    if (!airSpeciesNames.contains(airInertSpecie))
    {
        FatalErrorIn(args.executable())
            << "Specified inert specie '" << airInertSpecie << "' not found in "
            << "species list. Available species:" << airSpeciesNames
            << exit(FatalError);
    }


    // toddYoung polynomials
    // from Todd&Young, J. Power Sources 110 (2002) pp 186-200

    // molar Cp polynomials
    // --------------------
    dictionary& tyDictAirCp = airSpeciesDict.subDict("CpCoeffs");
    PtrList<polyToddYoung> molarCpAir(airSpecies.size());
    forAll(airSpecies, i)
    {
        polyToddYoung tmp(tyDictAirCp.lookup(airSpecies[i].name()));
        molarCpAir.set
        (
            i,
            new polyToddYoung(tmp)
        );
        Info<< "CpCoeffs(" << airSpecies[i].name() << ") ";
        molarCpAir[i].writeData();
    }
    Info<< endl;

    // mu polynomials
    // --------------
    dictionary& tyDictAirMu = airSpeciesDict.subDict("muCoeffs");
    PtrList<polyToddYoung> muAirL(airSpecies.size());
    forAll(airSpecies, i)
    {
        polyToddYoung tmp(tyDictAirMu.lookup(airSpecies[i].name()));
        muAirL.set
        (
            i,
            new polyToddYoung(tmp)
        );
        Info<< "muCoeffs(" << airSpecies[i].name() << ") ";
        muAirL[i].writeData();
    }
    Info<< endl;

    // k polynomials
    // --------------
    dictionary& tyDictAirK = airSpeciesDict.subDict("kCoeffs");
    PtrList<polyToddYoung> kAirL(airSpecies.size());
    forAll(airSpecies, i)
    {
        polyToddYoung tmp(tyDictAirK.lookup(airSpecies[i].name()));
        kAirL.set
        (
            i,
            new polyToddYoung(tmp)
        );
        Info<< "kCoeffs(" << airSpecies[i].name() << ") ";
        kAirL[i].writeData();
    }
    Info<< endl;

    // T=353 (J.P.Kone, 01.06.2016)
    //Changed reference temperature from 353K to 343K (K.W 2020) 
    Info<< "ToddYoung checks" << nl;
    forAll(airSpecies, i)
    {
        Info<< airSpecies[i].name() << nl;
        Info<< "    Cp(343) = " << molarCpAir[i].polyVal(343) << nl;
        Info<< "    mu(343) = " << muAirL[i].polyVal(343) << nl;
        Info<< "    k(343)  = " << kAirL[i].polyVal(343) << nl;
    }
    Info<< nl;


    // mass fraction fields and files
    // ------------------------------
    multivariateSurfaceInterpolationScheme<scalar>::fieldTable airYs;
    PtrList<volScalarField> Yair(airSpecies.size());
    
    forAll(airSpecies, i)
    {
        Yair.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "Y" + airSpecies[i].name() + "air",
                    runTime.timeName(),
                    airMesh,
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                airMesh
            )
        );
        airYs.add(Yair[i]);
    }

    // initialize sum Y[i]/MW[i] field for X[i] calclations
    // ----------------------------------------------------
    volScalarField sumAirYIonMI ("YoM", 0.0*Yair[0]);


    // mole fraction fields and files
    // ------------------------------
    multivariateSurfaceInterpolationScheme<scalar>::fieldTable airXs;
    PtrList<volScalarField> Xair(airSpecies.size());

    forAll(airSpecies, i)
    {
        Xair.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "X" + airSpecies[i].name() + "air",
                    runTime.timeName(),
                    airMesh,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                airMesh,
                dimensionedScalar("x", dimensionSet(0, 0, 0, 0, 0, 0, 0), 1),
                zeroGradientFvPatchScalarField::typeName
            )
        );
        airXs.add(Xair[i]);
    }

    // calculate X fields
    // -----------------
    #include "getXair.H"


    // diffusivity fields and files
    // ------------------------------
    multivariateSurfaceInterpolationScheme<scalar>::fieldTable airDiffs;
    PtrList<volScalarField> diffSpAir(airSpecies.size());

    forAll(airSpecies, i)
    {
        diffSpAir.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "diff" + airSpecies[i].name() + "air",
                    runTime.timeName(),
                    airMesh,
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                airMesh,
                dimensionedScalar("d", dimensionSet(0, 2, -1, 0, 0, 0, 0), 1),
                zeroGradientFvPatchScalarField::typeName
            )
        );
        airDiffs.add(diffSpAir[i]);
    }

