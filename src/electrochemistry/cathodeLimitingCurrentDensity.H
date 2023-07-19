// calculating cathode limiting current density (J.P.Kone, 01.11.2016)

//    reference: O'Hayre (2006) p.145
//    For cathode (air)
//    iLCathode = (n*F*CO2*DO2)/(tGDl)
//              = 10^3*(n*F*yO2*rhoAir*DO2)/(MO2*tGDl)
//    where
//    n = number of electrons exchanged by oxygen
//    F = Faraday constant
//    yO2 = mass fraction of oxygen
//    rhoAir = density of air
//    DO2 = diffusivity of oxygen
//    MO2 = molecular weight of oxygen
//    tGDL = thickness of the gas diffusion layer
//

scalarField iLCathode(cathodePatch.size(), 0);
{
    // Compute local cathode limiting current density
    // value is multiplied by 10^3 to convert kg/kmol to kg/mol

    forAll(airSpecies, s)
    {
        if (airSpecies[s].rSign() == -1)
        {

	      iLCathode =
                (
                    (airSpecies[s].ne()*F
                    *Yair[s].boundaryFieldRef()[cathodeID]
                    *rhoAir.boundaryFieldRef()[cathodeID]
                    *diffSpAir[s].boundaryFieldRef()[cathodeID])
                    /(airSpecies[s].MW()*(tMpl.value()+tSub.value()))  //Implementation of GDL as Substrat and MPL, S. Schneider 02.02.2020
                );
                iLCathode *=1.0e+3;
        }          
    }   
    Info<< "min,mean,max (iLCa) = " << gMin(iLCathode) << "  "
        << gAverage(iLCathode) << "  " << gMax(iLCathode) << nl;
}

//  Save data for output (J.P.Kone, 06.07.2017)
scalarField& iLCIn = iLC.ref();
smearPatchToMesh smeariLC(electrolyteMesh, electrolyteCathodeName);
iLCIn = smeariLC.smear(iLCathode);
iLC.correctBoundaryConditions();
iLC.boundaryFieldRef()[electrolyteCathodeID] == iLCathode;

