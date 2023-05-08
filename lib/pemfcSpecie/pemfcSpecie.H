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
    Foam::pemfcSpecie

Description
    Set of electrochemical properties for a pemfc specie

SourceFiles
    pemfcSpecieI.H
    pemfcSpecie.C

\*---------------------------------------------------------------------------*/

#ifndef pemfcSpecie_H
#define pemfcSpecie_H

#include "word.H" // OpenFOAM src file
#include "scalar.H" // OpenFOAM src file
#include "Switch.H" // OpenFOAM src file

#include "fvMesh.H" // OpenFOAM src file

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Code changed and adapted to pemfc (J.P.Kone, 19.12.2015)

namespace Foam
{

class Istream;
class Ostream;

/*---------------------------------------------------------------------------*\
                           Class pemfcSpecie Declaration
\*---------------------------------------------------------------------------*/

class pemfcSpecie
{
    // Private data

        //- Name of pemfcSpecie
        word name_;

        //- Molecular weight of pemfcSpecie [kg/kmol]
        scalar molWeight_;

        //- Number of electrons this specie contributes to electrochemistry
        scalar nElectrons_;

        //- Is the specie consumed(-1), produced(+1), or inert(0)?
        label rSign_;

        //- Enthalpy of formation at 298.15 K
        scalar hForm_;

        //- Standard entropy at 298.15 K
        scalar sForm_;

        //- Reference to the finite volume mesh this pemfcSpecie is part of
	//const fvMesh& mesh_;


    // Private member functions

        //- Construct from components without name
        inline pemfcSpecie
        (
            const scalar molWeight,
            const scalar nElectrons,
            const label rSign,
            const scalar hForm,
            const scalar sForm
        );


public:

    // Constructors

        //- Construct from components with name
        inline pemfcSpecie
        (
            const word& name,
            const scalar molWeight,
            const scalar nElectrons,
            const label rSign,
            const scalar hForm,
            const scalar sForm
        );

        //- Construct as copy
        inline pemfcSpecie(const pemfcSpecie&);

        //- Construct as named copy
        inline pemfcSpecie(const word& name, const pemfcSpecie&);

	//- Construct and return a clone
	inline autoPtr<pemfcSpecie> clone() const;

        //- Construct from Istream
        pemfcSpecie(Istream&);


        //- Return pointer to new pemfcSpecie created on freestore from Istream
	autoPtr<pemfcSpecie> New(Istream& is);


    // Member Functions

        // Access

            //- Name
            inline word name() const;

            //- Molecular weight [kg/kmol]
            inline scalar MW() const;

            //- Number of electrons this specie contributes to electrochemistry
            inline scalar ne() const;

            //- Is the specie consumed(<0), produced(>0), or inert(0)?
            inline label rSign() const;

            //- Enthalpy of formation at 298.15 K
            inline scalar hForm() const;

            //- Standard entropy at 298.15 K
            inline scalar sForm() const;


    // Member operators

        inline void operator=(const pemfcSpecie&);


    // Ostream Operator

        friend Ostream& operator<<(Ostream&, const pemfcSpecie&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "pemfcSpecieI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
