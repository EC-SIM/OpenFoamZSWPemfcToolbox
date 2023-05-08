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
    Foam::myMeshWave

Description
    myFaceCellWave plus data

SourceFiles
    myMeshWave.C

\*---------------------------------------------------------------------------*/

#ifndef myMeshWave_H
#define myMeshWave_H

#include "myFaceCellWave.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class MeshWaveName Declaration
\*---------------------------------------------------------------------------*/

TemplateName(myMeshWave);


/*---------------------------------------------------------------------------*\
                           Class myMeshWave Declaration
\*---------------------------------------------------------------------------*/

// Code changed and converted to openfoam-4.0 (J.P.Kone, 09.09.2016)
// class trackingData is added, and td_ is passed as argument

template <class Type, class TrackingData = int>
class myMeshWave
:
    public myMeshWaveName
{
    // Private data

        //- Wall information for all faces
        List<Type> allFaceInfo_;

        //- Wall information for all cells
        List<Type> allCellInfo_;

        //- Wave calculation engine.
        myFaceCellWave<Type> calc_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        myMeshWave(const myMeshWave&);

        //- Disallow default bitwise assignment
        void operator=(const myMeshWave&);

    // added (J. P. Kone, 09.09.2016)  
    // Private static data

            //- Used as default trackdata value to satisfy default template
            //  argument.
            static int dummyTrackData_;

public:

    // Constructors

        //- Construct from mesh and list of changed faces with the Type
        //  for these faces. Iterates until nothing changes or maxIter reached.
        //  (maxIter can be 0)
        myMeshWave
        (
            const polyMesh& mesh,
            const labelList& initialChangedFaces,
            const List<Type>& changedFacesInfo,
            const label maxIter,
            TrackingData& td = dummyTrackData_
        );

        //- Construct from mesh, list of changed faces with the Type
        //  for these faces and initial field.
        //  Iterates until nothing changes or maxIter reached.
        //  (maxIter can be 0)
        myMeshWave
        (
            const polyMesh& mesh,
            const labelList& initialChangedFaces,
            const List<Type>& changedFacesInfo,
            const List<Type>& allCellInfo,
            const label maxIter,
            TrackingData& td = dummyTrackData_
        );


    // Member Functions

        //- Get allFaceInfo
        const List<Type>& allFaceInfo() const
        {
            return allFaceInfo_;
        }

        //- Get allCellInfo
        const List<Type>& allCellInfo() const
        {
            return allCellInfo_;
        }


        //- Iterate until no changes or maxIter reached. Returns number of
        //  unset cells (see getUnsetCells)
        label iterate(const label maxIter)
        {
            return calc_.iterate(maxIter);
        }

        //- Get number of unvisited cells, i.e. cells that were not (yet)
        //  reached from walking across mesh. This can happen from
        //  - not enough iterations done
        //  - a disconnected mesh
        //  - a mesh without walls in it
        label getUnsetCells() const
        {
            return calc_.getUnsetCells();
        }

        //- Get number of unvisited faces
        label getUnsetFaces() const
        {
            return calc_.getUnsetFaces();
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "myMeshWave.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
