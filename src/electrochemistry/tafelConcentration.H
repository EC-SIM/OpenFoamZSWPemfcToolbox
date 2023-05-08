//Calculate molar concentration for implementing in Tafel equatation (S.S. 17.05.2021)

scalarField conCathode(cathodePatch.size(),0);
{
  forAll(airSpecies, s)
    {
        if (airSpecies[s].name() == "O2")			//Changed (S.S.03/2021)
        {
            conCathode =
            (
			  Yair[s].boundaryFieldRef()[cathodeID]
			  * rhoAir.boundaryFieldRef()[cathodeID]
			  / airSpecies[s].MW()
			);
			
        }
    }
}


scalarField conAnode(anodePatch.size(),0);
{
  forAll(fuelSpecies, s)
    {
        if (fuelSpecies[s].name() == "H2")			//Changed (S.S.03/2021)
        {
            conAnode =
            (
			  anodeToCathode.faceInterpolate(Yfuel[s].boundaryFieldRef()[anodeID])
			  * anodeToCathode.faceInterpolate(rhoFuel.boundaryFieldRef()[anodeID])
			  / fuelSpecies[s].MW()
			);
        }
    }
}

