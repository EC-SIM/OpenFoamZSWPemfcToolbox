// Code changed and adapted to pemfc electrochemistry (J.P.Kone, 26.05.2016)

// for coversion to openfoam-4.0
// call to 'internalField()' is changed to 'ref()'

{
  label znId1 = electrolyteMesh.cellZones().findZoneID("layer");

  Info << nl << "Solve current" << endl;

  scalar stackVoltage = 0.0;

  for (int counter1=1; counter1<=numCells.value(); counter1++)
  {
	// S.S. 02/2021 Changed for calculation of one cell
	if (counter1 == 1)
	{
	  znId1 = electrolyteMesh.cellZones().findZoneID("electrolyte");
	}
	
    else
	{
	  Info << nl << "layer " << counter1 << endl;

	  char rLayer[16]; // string which will contain the number

	  sprintf (rLayer, "layer%d", counter1 ); // %d makes the result be a decimal integer

	  znId1 = electrolyteMesh.cellZones().findZoneID(rLayer);
	}
	
	labelList zn1(electrolyteMesh.cellZones()[znId1]);

	{
	  dimensionedScalar Ibar ("Ibar", dimensionSet(0, -2, 0, 0, 0, 1, 0), 0.0);
	  dimensionedScalar temp ("temp", dimensionSet(0, 0, 0, 1, 0, 0, 0), 0.0);

	  Ibar.value() = 0.0;
	  scalar rCurrentDV = 0.0;
	  scalar volume = 0.0;
	  scalar sumVolume = 0.0;
	  scalar counter2 = 0.0;

			  
	  scalarField iCell = idensityIn;

	  forAll(zn1, j)
		{
		  volume = mag(electrolyteMesh.V()[zn1[j]]);
		  rCurrentDV += idensityIn[zn1[j]]*volume;
		  sumVolume += volume;
		  counter2 += 1;
		  V.value() = voltage[zn1[j]];
		}

		// Collect across all processors
		reduce(sumVolume,plusOp<scalar>());
		reduce(rCurrentDV,plusOp<scalar>());
		reduce(counter2,plusOp<scalar>());

		Info << nl << "rCurrentDV: " << rCurrentDV << endl;
		Info << nl << "sumVolume: " << sumVolume << endl;

		Ibar.value() = rCurrentDV/sumVolume;

		Info << "min,max(Nernst): "
			  << Foam::min(Nernst) <<" , "
			  << Foam::max(Nernst) << endl;

		// voltage correction
		if (galvanostatic)
		{
			V += Rhat*(Ibar - Ibar0);

			Info << "    Ibar0 = " << Ibar0.value()
				  << "    Ibar = " << Ibar.value()
				  << "    V = " << V.value() << endl;

			forAll(zn1, j)
			{
				voltage[zn1[j]] = V.value();
			}
		}
		else
		{
			Info << "    Ibar = " << Ibar.value()
				 << "    V = " << V.value() << endl;
		}

		}
		stackVoltage += V.value();
	}
	Info << nl << "stack Voltage =" << stackVoltage << endl;        
}

// mean current density
Info << "min,mean,max(current): "
     << gMin(idensity.ref()) << " , "
     << gAverage(idensity.ref()) << " , "
     << gMax(idensity.ref()) << endl;
