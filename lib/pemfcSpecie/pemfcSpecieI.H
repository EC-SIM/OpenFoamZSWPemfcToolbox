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

#include "pemfcSpecie.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Code changed and adapted to pemfc (J.P.Kone, 19.12.2015)

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components without name
inline pemfcSpecie::pemfcSpecie
(
    const scalar molWeight,
    const scalar nElectrons,
    const label rSign,
    const scalar hForm,
    const scalar sForm
)
:
    //name_(),
    molWeight_(molWeight),
    nElectrons_(nElectrons),
    rSign_(rSign),
    hForm_(hForm),
    sForm_(sForm)
{}


// Construct from components
inline pemfcSpecie::pemfcSpecie
(
    const word& name,
    const scalar molWeight,
    const scalar nElectrons,
    const label rSign,
    const scalar hForm,
    const scalar sForm
)
:
    name_(name),
    molWeight_(molWeight),
    nElectrons_(nElectrons),
    rSign_(rSign),
    hForm_(hForm),
    sForm_(sForm)
{}


// Construct as copy
inline pemfcSpecie::pemfcSpecie(const pemfcSpecie& st)
:
    name_(st.name_),
    molWeight_(st.molWeight_),
    nElectrons_(st.nElectrons_),
    rSign_(st.rSign_),
    hForm_(st.hForm_),
    sForm_(st.sForm_)
{}


// Construct as named copy
inline pemfcSpecie::pemfcSpecie(const word& name, const pemfcSpecie& st)
:
    name_(name),
    molWeight_(st.molWeight_),
    nElectrons_(st.nElectrons_),
    rSign_(st.rSign_),
    hForm_(st.hForm_),
    sForm_(st.sForm_)
{}


// Clone
inline Foam::autoPtr<pemfcSpecie>
Foam::pemfcSpecie::clone() const
{
    return autoPtr<pemfcSpecie>
    (
        new pemfcSpecie(*this)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Name
inline word pemfcSpecie::name() const
{
    return name_;
}

//- Molecular weight [kg/kmol]
inline scalar pemfcSpecie::MW() const
{
    return molWeight_;
}

//- No of electrons this pemfcSpecies contributes to electrochemistry
inline scalar pemfcSpecie::ne() const
{
    return nElectrons_;
}

//- Is the specie consumed(-1), produced(+1), or inert(0)?
inline label pemfcSpecie::rSign() const
{
    return rSign_;
}

//- enthalpy of formation [J/mol]
inline scalar pemfcSpecie::hForm() const
{
    return hForm_;
}

//- standard entropy [J/mol]
inline scalar pemfcSpecie::sForm() const
{
    return sForm_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void pemfcSpecie::operator=(const pemfcSpecie& st)
{
    name_ = st.name_;
    molWeight_ = st.molWeight_;
    nElectrons_ = st.nElectrons_;
    rSign_ = st.rSign_;
    hForm_ = st.hForm_;
    sForm_ = st.sForm_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
