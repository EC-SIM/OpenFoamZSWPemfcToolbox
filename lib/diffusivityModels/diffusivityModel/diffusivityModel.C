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

\*---------------------------------------------------------------------------*/

#include "diffusivityModel.H"
#include "volFields.H" // OpenFOAM src file

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace diffusivityModels
{
  defineTypeNameAndDebug(diffusivityModel, 0);
  defineRunTimeSelectionTable(diffusivityModel, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

diffusivityModel::diffusivityModel
(
    fvMesh& mesh,
    scalarField& diff,
    const labelList& cells
)
:
    mesh_(mesh),
    diff_(diff),
    cells_(cells)
{}

diffusivityModel::diffusivityModel
(
    fvMesh& mesh,
    scalarField& diff,
    const labelList& cells,
    const dictionary& dict
)
:
    mesh_(mesh),
    diff_(diff),
    cells_(cells),
    dict_(dict)
{}


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //

diffusivityModel::~diffusivityModel()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

// none

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusivityModels
} // End namespace Foam

// ************************************************************************* //
