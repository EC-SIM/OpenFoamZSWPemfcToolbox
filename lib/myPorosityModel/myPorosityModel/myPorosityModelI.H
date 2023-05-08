/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2012-2013 OpenFOAM Foundation
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

inline const Foam::word& Foam::myPorosityModel::name() const
{
    return name_;
}


inline bool Foam::myPorosityModel::active() const
{
    return active_;
}


inline const Foam::labelList& Foam::myPorosityModel::cellZoneIDs() const
{
    return cellZoneIDs_;
}

   // Access
   // added (J. P. Kone, 26.07.2016)

        //- dictionary values used for the porousZone
        inline const Foam::dictionary& Foam::myPorosityModel::dict() const
        {
            return dict_;
        }

        //- Return porosity
        inline Foam::scalar Foam::myPorosityModel::porosity() const
        {
            return porosity_;
        }
         
        //- Edit access to porosity
        inline Foam::scalar& Foam::myPorosityModel::porosity()
        {
            return porosity_;
        }


// ************************************************************************* //
