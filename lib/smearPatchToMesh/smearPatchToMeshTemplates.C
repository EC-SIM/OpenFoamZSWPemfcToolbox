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
    Smear interpolation

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

\*----------------------------------------------------------------------------*/

// Code changed and converted to openfoam-4.0 (J.P.Kone, 20.07.2016)

#include "smearPatchToMesh.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


template<class Type>
Foam::tmp<Foam::Field<Type> > Foam::smearPatchToMesh::smear
(
    const Field<Type>& pf
) const
{
    if (pf.size() != mesh_.boundary()[patchIndex_].size())
    {
        FatalErrorIn
        (
            "tmp<Field<Type> > Foam::smearPatchToMesh::smear\n"
            "(\n"
            "    const Field<Type>& pf\n"
            ") const"
        )   << "Field does not correspond to patch.  Field size = "
            << pf.size() << " patch size = "
            << mesh_.boundary()[patchIndex_].size()
            << abort(FatalError);
    }

    // Create result
    // layout changed (J.P.Kone, 20.07.2016)
    tmp<Field<Type>> tresult
    (
        new Field<Type> 
        (
            mesh_.nCells()
        )
    );
    Field<Type>& result = tresult.ref(); // added 'ref()' (J.P.Kone, 20.07.2016)

    const labelList& addr = addressing();

    forAll (result, i)
    {
        result[i] = pf[addr[i]];
    }

    return tresult;
}


// ************************************************************************* //
