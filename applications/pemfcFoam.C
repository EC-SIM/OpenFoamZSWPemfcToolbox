/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    pemfcSinglephaseNonIsothermalSolver

Description
    Solver that can simulate the single-phase non-isothermal operation of proton exchange membrane fuel cell (PEMFC)

\*---------------------------------------------------------------------------*/

#include <iostream> // standard file
#include <stdio.h> // standard file
#include <iomanip> // standard file

#include "fvOptions.H"
#include "fvCFD.H" // OpenFOAM src file
#include "atomicWeights.H" // OpenFOAM src file
#include "physicalConstants.H"
#include "specie.H" // OpenFOAM src file	
#include "speciesTable.H" // OpenFOAM src file
#include "pemfcSpecie.H"

#include "mypatchToPatchInterpolation.H" // new lib added (J.P.Kone, 20.07.2016) 
#include "myPatchToPatchInterpolation.H" // new lib added (J.P.Kone, 20.07.2016) 
#include "myContinuityErrs.H"
#include "initContinuityErrs.H" // OpenFOAM src file
#include "fixedGradientFvPatchFields.H" // OpenFOAM src file
#include "smearPatchToMesh.H"

#include "diffusivityModels.H"
#include "myPorosityModel.H" // new lib added (J.P.Kone, 26.07.2016)
#include "myIOporosityModelList.H" // new lib added (J.P.Kone, 26.07.2016)
#include "polyToddYoung.H"
#include "RiddersRoot.H"//added SBB
#include "functionIntegration.H" //added (J.P.Kone, 26.10.2016)
#include "functionElectrolyteResistance.H" //added (J.P.Kone, 26.10.2016)
#include "funcTotalCurrent.H" //added (K.Wagner, 19.05.2020)

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
#   define NO_CONTROL
#   define CREATE_MESH createMeshesPostProcess.H
#   include "postProcess.H"    
  
#   include "setRootCase.H"	// OpenFOAM src file
#   include "createTime.H"	// OpenFOAM src file

    // Complete cell components
#   include "createMesh.H" // OpenFOAM src file
#   include "readCellProperties.H"
#   include "createCellFields.H"

    //Coolant components (K.W 2020)
#   include "createCoolantMesh.H"
#   include "readCoolantProperties.H"
#   include "createCoolantFields.H"


    // BipolarPlate components
#   include "createAbpMesh.H"
#   include "createCbpMesh.H"
#   include "readAbpProperties.H"
#   include "readCbpProperties.H"
#   include "createAbpFields.H"
#   include "createCbpFields.H"

    // Air-related components
#   include "createAirMesh.H"
#   include "readAirProperties.H"
#   include "createAirFields.H"
#   include "createAirSpecies.H"

    // Electrolyte components
#   include "createElectrolyteMesh.H"
#   include "readElectrolyteProperties.H"
#   include "readActivationParameters.H"//Added SBB
#   include "createElectrolyteFields.H"


    // Fuel-related components
#   include "createFuelMesh.H"
#   include "readFuelProperties.H"
#   include "createFuelFields.H"
#   include "createFuelSpecies.H"
#   include "readRxnProperties.H"

#   include "setGlobalPatchIds.H"

    // calculate electrolyte thickness, tMem and thickness of gdl, tGdl
#   include "geometricParameter.H"

    // Cathode & Anode interpolation
#   include "createPatchToPatchInterpolation.H"

    // Gas diffusivity models
#   include "createDiffusivityModels.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    bool firstTime = true;
    
    int iLoop = 0;

    while (runTime.loop())
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;
        iLoop++;

    #   include "mapFromCell.H"    // map global T to fluid regions

    #   include "rhoFuel.H"
    #   include "rhoAir.H"

    #   include "muFuel.H"
    #   include "muAir.H"

    #   include "kFuel.H"
    #   include "kAir.H"

    #   include "solveAir.H" // air is solved before fuel
    #   include "solveFuel.H"
    #   include "solveCoolant.H"	//Coolant calculation (K.W 2020)

    // #   include "ReynoldsNumber.H" //Commented S.Schneider 07/2023

    #   include "diffusivityFuel.H"
    #   include "diffusivityAir.H"

    #   include "YairEqn.H" // Yair is solved before fuel
    #   include "YfuelEqn.H"

    if (iLoop > PreIterAct.value())
    {
    #   include "solveElectrochemistry.H"
    }
    else
    {
        Info<< "PreIteration: " << iLoop <<endl;
    }
    

    #   include "mapToCell.H"

    #   include "solveEnergy.H"

        runTime.write();

        if(firstTime)
        {
            firstTime = false;
        }

        Info<< "ExecutionTime = "
            << runTime.elapsedCpuTime()
            << " s\n\n" << endl;
    }

    Info<< "End\n" << endl;
    return(0);
}


// ************************************************************************* //
