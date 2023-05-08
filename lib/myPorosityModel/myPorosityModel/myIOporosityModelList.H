/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2012 OpenFOAM Foundation
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
    Foam::myIOporosityModelList

Description
    List of porosity models with IO functionality

SourceFiles
    myIOporosityModelList.C

\*---------------------------------------------------------------------------*/

#ifndef myIOporosityModelList_H
#define myIOporosityModelList_H

#include "IOdictionary.H"
#include "myPorosityModelList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class myIOporosityModelList Declaration
\*---------------------------------------------------------------------------*/

class myIOporosityModelList
:
    public IOdictionary,
    public myPorosityModelList
{
private:

    // Private Member Functions

        //- Create IO object if dictionary is present
        IOobject createIOobject(const fvMesh& mesh) const;

        //- Disallow default bitwise copy construct
        myIOporosityModelList(const myIOporosityModelList&);

        //- Disallow default bitwise assignment
        void operator=(const myIOporosityModelList&);


public:

    // Constructors

        //- Construct from mesh
        myIOporosityModelList(const fvMesh& mesh);


        //- Destructor
        virtual ~myIOporosityModelList()
        {}


    // Member Functions

        //- Read dictionary
        virtual bool read();
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
