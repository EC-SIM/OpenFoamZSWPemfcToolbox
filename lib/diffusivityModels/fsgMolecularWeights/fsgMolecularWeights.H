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

Class
    Foam::fsgMolecularWeightTable

Description
    A table of Fuller-Schettler-Giddings molecular weights [kg/kmol]
    for available molecules

Data
    Molecules listed in:
        Fuller, Ensley, and Giddings, Diffusion of Halogenated Hydrocarbons
        in Helium, Journal of Physical Chemistry v73 n11 November 1969.
	Omits aromatic/heterocyclic ring increments
    Molecular weight of air from:
        US Standard Atmosphere, 1976
    Other molecular weights calculated from:
        $FOAM_SRC/thermophysicalModels/specie/atomicWeights

SourceFiles
    fsgMolecularWeights.C

\*---------------------------------------------------------------------------*/

#ifndef fsgMolecularWeights_H
#define fsgMolecularWeights_H

#include "scalar.H" // OpenFOAM src file
#include "HashTable.H" // OpenFOAM src file

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace diffusivityModels
{

/*---------------------------------------------------------------------------*\
                           Class fsgMolecularWeights Declaration
\*---------------------------------------------------------------------------*/

class fsgMolecularWeightTable
:
    public HashTable<scalar>
{

public:

    // Public types and data

        //- Structure to hold the molecule name and molecular weight pair
        struct fsgMolecularWeight
        {
            char name[4];
            scalar molWeight;
        };

        static const int nMolecules = 29;

        //- Static table of the molecular weights of the known FSG molecules
        static const fsgMolecularWeight fsgMolecularWeights[nMolecules];


    // Constructors

        //- Construct from fsgMolecularWeights_
        fsgMolecularWeightTable();
};


// * * * * * * * * * * * * * * * * Global data  * * * * * * * * * * * * * * //

// Fuller-Schettler-Giddings molecular weights table for available molecules

extern fsgMolecularWeightTable fsgMolecularWeights;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusivityModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
