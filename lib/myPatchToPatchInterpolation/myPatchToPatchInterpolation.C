/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
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

#include "myPatchToPatchInterpolation.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class FromPatch, class ToPatch>
const scalar
myPatchToPatchInterpolation<FromPatch, ToPatch>::directHitTol = 1e-5;

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class FromPatch, class ToPatch>
const labelList&
myPatchToPatchInterpolation<FromPatch, ToPatch>::pointAddr() const
{
    if (!pointAddressingPtr_)
    {
        calcPointAddressing();
    }

    return *pointAddressingPtr_;
}


template<class FromPatch, class ToPatch>
const FieldField<Field, scalar>&
myPatchToPatchInterpolation<FromPatch, ToPatch>::pointWeights() const
{
    if (!pointWeightsPtr_)
    {
        calcPointAddressing();
    }

    return *pointWeightsPtr_;
}


template<class FromPatch, class ToPatch>
const labelList&
myPatchToPatchInterpolation<FromPatch, ToPatch>::faceAddr() const
{
    if (!faceAddressingPtr_)
    {
        calcFaceAddressing();
    }

    return *faceAddressingPtr_;
}


template<class FromPatch, class ToPatch>
const FieldField<Field, scalar>&
myPatchToPatchInterpolation<FromPatch, ToPatch>::faceWeights() const
{
    if (!faceWeightsPtr_)
    {
        calcFaceAddressing();
    }

    return *faceWeightsPtr_;
}


template<class FromPatch, class ToPatch>
void myPatchToPatchInterpolation<FromPatch, ToPatch>::clearOut()
{
    deleteDemandDrivenData(pointAddressingPtr_);
    deleteDemandDrivenData(pointWeightsPtr_);
    deleteDemandDrivenData(pointDistancePtr_);
    deleteDemandDrivenData(faceAddressingPtr_);
    deleteDemandDrivenData(faceWeightsPtr_);
    deleteDemandDrivenData(faceDistancePtr_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class FromPatch, class ToPatch>
myPatchToPatchInterpolation<FromPatch, ToPatch>::myPatchToPatchInterpolation
(
    const FromPatch& fromPatch,
    const ToPatch& toPatch,
    intersection::algorithm alg,
    const intersection::direction dir
)
:
    fromPatch_(fromPatch),
    toPatch_(toPatch),
    alg_(alg),
    dir_(dir),
    pointAddressingPtr_(NULL),
    pointWeightsPtr_(NULL),
    pointDistancePtr_(NULL),
    faceAddressingPtr_(NULL),
    faceWeightsPtr_(NULL),
    faceDistancePtr_(NULL)
{}


// * * * * * * * * * * * * * * * * Destructor * * * * * * * * * * * * * * * //

template<class FromPatch, class ToPatch>
myPatchToPatchInterpolation<FromPatch, ToPatch>::~myPatchToPatchInterpolation()
{
    clearOut();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class FromPatch, class ToPatch>
const scalarField&
myPatchToPatchInterpolation<FromPatch, ToPatch>
::pointDistanceToIntersection() const
{
    if (!pointDistancePtr_)
    {
        calcPointAddressing();
    }

    return *pointDistancePtr_;
}


template<class FromPatch, class ToPatch>
const scalarField&
myPatchToPatchInterpolation<FromPatch, ToPatch>
::faceDistanceToIntersection() const
{
    if (!faceDistancePtr_)
    {
        calcFaceAddressing();
    }

    return *faceDistancePtr_;
}


template<class FromPatch, class ToPatch>
bool myPatchToPatchInterpolation<FromPatch, ToPatch>::movePoints()
{
    clearOut();

    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    #include "myCalcPatchToPatchWeights.C"
    #include "myPatchToPatchInterpolate.C"

// ************************************************************************* //
