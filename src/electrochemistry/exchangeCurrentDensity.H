// calculate ExchangeCurrentDensity (Changed/amended K.W, 2020)
// i0Cathode = i0Refernce * exp(-EAct/Rgas * (1/T - 1/Tref))
// with 
// 	i0Refernce = fitting value
// 	EAct = ActivationEnergy

scalarField i0Cathode(cathodeT.size());
{
     i0Cathode =
     (
       
         i0CathodeElectrolyteReference.value()
        *Foam::exp(((-1.*ECathodeElectrolyte.value())/(Rgas))
        *((1.0/cathodeT)-(1.0/343.15))) //Changed to 343
     );

    Info<< "min,mean,max (i0Ca) = " << gMin(i0Cathode) << "  "
        << gAverage(i0Cathode) << "  " << gMax(i0Cathode) << nl;

}

scalarField i0Anode(cathodeT.size());
{
     i0Anode =
     (
        i0AnodeElectrolyteReference.value()
        *Foam::exp(((-1.*EAnodeElectrolyte.value())/(Rgas))
        *((1.0/anodeToCathode.faceInterpolate(anodeT))-(1.0/343.15))) 
     );

    Info<< "min,mean,max (anodeT) = " << gMin(anodeT) << "  "
        << gAverage(anodeT) << "  " << gMax(anodeT) << nl;
    Info<< "min,mean,max (i0An) = " << gMin(i0Anode) << "  "
        << gAverage(i0Anode) << "  " << gMax(i0Anode) << nl;

}

//(SS 14.10.2021 Postprocessing i0)
scalarField& i0CIn = i0C.ref();
smearPatchToMesh smeari0C(electrolyteMesh, electrolyteCathodeName);
i0CIn = smeari0C.smear(i0Cathode);
i0C.correctBoundaryConditions();
i0C.boundaryFieldRef()[electrolyteCathodeID] == i0Cathode;

scalarField& i0AIn = i0A.ref();
smearPatchToMesh smeari0A(electrolyteMesh, electrolyteCathodeName);
i0AIn = smeari0A.smear(i0Anode);
i0A.correctBoundaryConditions();
i0A.boundaryFieldRef()[electrolyteAnodeID] == i0Anode;
