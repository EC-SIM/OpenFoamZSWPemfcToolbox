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

Description

\*---------------------------------------------------------------------------*/

#include "myMeshWave.H"
#include "polyMesh.H" //OpenFOAM src file

// Code changed and converted to openfoam-4.0 (J.P.Kone, 09.09.2016)
// class trackingData is added, and td_ is passed as argument

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class Type, class TrackingData>
int Foam::myMeshWave<Type, TrackingData>::dummyTrackData_ = 12345;

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Iterate, propagating changedFacesInfo across mesh, until no change (or 
// maxIter reached).
template <class Type, class TrackingData>
Foam::myMeshWave<Type, TrackingData>::myMeshWave
(
    const polyMesh& mesh,
    const labelList& changedFaces,
    const List<Type>& changedFacesInfo,
    const label maxIter,
    TrackingData& td
)
:
    allFaceInfo_(mesh.nFaces()),
    allCellInfo_(mesh.nCells()),
    calc_
    (
        mesh,
        changedFaces,
        changedFacesInfo,
        allFaceInfo_,
        allCellInfo_,
        maxIter,
        td
    )
{}


// Iterate, propagating changedFacesInfo across mesh, until no change (or 
// maxIter reached). Initial cell values specified.
template <class Type, class TrackingData>
Foam::myMeshWave<Type, TrackingData>::myMeshWave
(
    const polyMesh& mesh,
    const labelList& changedFaces,
    const List<Type>& changedFacesInfo,
    const List<Type>& allCellInfo,
    const label maxIter,
    TrackingData& td
)
:
    allFaceInfo_(mesh.nFaces()),
    allCellInfo_(allCellInfo),
    calc_
    (
        mesh,
        changedFaces,
        changedFacesInfo,
        allFaceInfo_,
        allCellInfo_,
        maxIter,
        td
    )
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


// ************************************************************************* //
