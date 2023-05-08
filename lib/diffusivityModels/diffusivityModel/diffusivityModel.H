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

Namespace
    Foam::diffusivityModels

Description
    A namespace for various incompressible diffusivityModel implementations

Class
    Foam::diffusivityModel

Description
    Base class for diffusivity models used for laminar incompressible
    flow with two component mixtures and/or porous media.

SourceFiles
    diffusivityModel.C
    newDiffusivityModel.C

\*---------------------------------------------------------------------------*/

#ifndef diffusivityModel_H
#define diffusivityModel_H

#include "IOdictionary.H" // OpenFOAM src file
#include "typeInfo.H" // OpenFOAM src file
#include "runTimeSelectionTables.H" // OpenFOAM src file
#include "volFieldsFwd.H" // OpenFOAM src file
#include "dimensionedScalar.H" // OpenFOAM src file
#include "autoPtr.H" // OpenFOAM src file
#include "tmp.H" // OpenFOAM src file
#include "scalarField.H" // OpenFOAM src file
#include "labelList.H" // OpenFOAM src file
#include "fvMesh.H" // OpenFOAM src file

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Code changed and converted to openfoam-4.0 (J.P.Kone 07.07.2016)

namespace Foam
{

namespace diffusivityModels
{

/*---------------------------------------------------------------------------*\
                Class diffusivityModel Declaration
\*---------------------------------------------------------------------------*/

class diffusivityModel
{

protected:

    // protected data

        // diffusivity mesh
        fvMesh& mesh_;

        // Reference to the diffusivity field
        scalarField& diff_;

        // Reference to the list of cell labels for which
        // diffusivity is to be calculated
        const labelList& cells_;

        // Dictionary containing the parameters
        const dictionary dict_;


    // Protected member functions

        //- Disallow copy construct
        diffusivityModel(const diffusivityModel&);

        //- Disallow default bitwise assignment
        void operator=(const diffusivityModel&);


public:

    //- Runtime type information
    TypeName("diffusivityModel");


    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            diffusivityModel,
            dictionary,
            (
                fvMesh& mesh,
                scalarField& diff,
                const labelList& cells, // List<int>& changed to labelList& (J. P. Kone 07.07.2016)
                const dictionary& dict
            ),
            (mesh, diff, cells, dict)
        );


    // Selectors

        //- Return a reference to the selected diffusivity model
        static autoPtr<diffusivityModel> New
        (
            fvMesh& mesh,
            scalarField& diff,
            const labelList& cells, // List<int>& changed to labelList& (J. P. Kone 07.07.2016)
            const dictionary& dict
        );


    // Constructors

        //- Construct null
        diffusivityModel
        (
            fvMesh& mesh,
            scalarField& diff,
            const labelList& cells
        );

        //- Construct from mesh, field, labelList, and dictionary
        diffusivityModel
        (
            fvMesh& mesh,
            scalarField& diff,
            const labelList& cells,
            const dictionary& dict
        );


    // Destructor

        virtual ~diffusivityModel();


    // Member Functions

        //- Reference to mesh
        const fvMesh& mesh() const
        {
            return mesh_;
        }

	//- Reference to field
        const scalarField& diff() const
        {
            return diff_;
        }

	//- Reference to labelList
        const labelList& cells() const
        {
            return cells_;
        }

        //- Reference to dictionary
        const dictionary& dict() const
        {
            return dict_;
        }

        //- provide feedback for user
        virtual void writeData() = 0;

        //- evaluate the diffusivity
        virtual void evaluate() = 0;

        //- set the species
        virtual void setSpecies(word spA, word spB) = 0;

        //- boolean functions for model info
        virtual bool isFixed() = 0;
        virtual bool isBinary() = 0;
        virtual bool isKnudsen() = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusivityModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
