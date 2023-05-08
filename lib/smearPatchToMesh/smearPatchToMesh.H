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
    smearPatchToMesh

Description
    Smear data from patch to mesh, giving each cell the value of nearest
    patch face

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

SourceFiles
    smearPatchToMeshI.H
    smearPatchToMesh.C
    smearPatchToMeshIO.C

\*---------------------------------------------------------------------------*/

#ifndef smearPatchToMesh_H
#define smearPatchToMesh_H

#include "fvMesh.H" // OpenFOAM src file

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                       Class smearPatchToMesh Declaration
\*---------------------------------------------------------------------------*/

class smearPatchToMesh
{
    // Private data

        //- Mesh reference
        const fvMesh& mesh_;

        //- Patch index
        const label patchIndex_;

        //- Addressing pointer
        mutable labelList* addressingPtr_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        smearPatchToMesh(const smearPatchToMesh&);

        //- Disallow default bitwise assignment
        void operator=(const smearPatchToMesh&);


        //- Calculate addressing
        void calcAddressing() const;


public:

    // Constructors

        //- Construct from mesh and patch
        smearPatchToMesh
        (
            const fvMesh& mesh,
            const word& patchName
        );


    // Destructor

        ~smearPatchToMesh();


    // Member Functions

        //- Return addressing: patch face index for each cell
        const labelList& addressing() const;

        //- Smear patch field to volume
        template<class Type>
        tmp<Field<Type> > smear(const Field<Type>& pf) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "smearPatchToMeshTemplates.C"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
