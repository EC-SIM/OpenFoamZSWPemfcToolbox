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
    Set of electrochemical properties for a pemfc specie

\*---------------------------------------------------------------------------*/

#include "pemfcSpecie.H"
#include "IOstreams.H" // OpenFOAM src file
#include "dimensionedConstants.H" // OpenFOAM src file

// Code changed and adapted to pemfc (J.P.Kone, 19.12.2015)

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::pemfcSpecie::pemfcSpecie(Istream& is)
:
    name_(is),
    molWeight_(readScalar(is)),
    nElectrons_(readScalar(is)),
    rSign_(readLabel(is)),
    hForm_(readScalar(is)),
    sForm_(readScalar(is))
{
    is.check("pemfcSpecie::pemfcSpecie(Istream& is)");
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

Foam::Ostream& Foam::operator<<(Ostream& os, const pemfcSpecie& st)
{
    os  << st.name_ << tab
        << st.molWeight_ << tab
        << st.nElectrons_ << tab
        << st.rSign_ << tab
	<< st.hForm_ << tab
	<< st.sForm_;

    os.check("Ostream& operator<<(Ostream& os, const pemfcSpecie& st)");
    return os;
}


// ************************************************************************* //
