/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2012-2015 OpenFOAM Foundation
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
    Foam::myPorosityModel

Description
    Top level model for porosity models

SourceFiles
    myPorosityModel.C
    myPorosityModelNew.C

\*---------------------------------------------------------------------------*/

#ifndef myPorosityModel_H
#define myPorosityModel_H

#include "fvMesh.H"
#include "dictionary.H"
#include "fvMatricesFwd.H"
#include "runTimeSelectionTables.H"
#include "coordinateSystem.H"
#include "dimensionedVector.H"
#include "keyType.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class myPorosityModel Declaration
\*---------------------------------------------------------------------------*/

class myPorosityModel
:
    public regIOobject
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        myPorosityModel(const myPorosityModel&);

        //- Disallow default bitwise assignment
        void operator=(const myPorosityModel&);


protected:

    // Protected data

        //- Porosity name
        word name_;

        //- Reference to the mesh database
        const fvMesh& mesh_;

        //-  Dictionary containing the parameters, (J. P. Kone, 11.09.2016)
        //dictionary dict_;

        //- Dictionary used for model construction
        const dictionary dict_;

        //- Model coefficients dictionary
        dictionary coeffs_;

        //- Porosity active flag
        bool active_;

        //- Name(s) of cell-zone
        keyType zoneName_;

        //- Cell zone IDs
        labelList cellZoneIDs_;

        //- Local co-ordinate system
        coordinateSystem coordSys_;
        
        //- porosity of the zone (0 < porosity <= 1)
        //  Placeholder for treatment of temporal terms.
        //  Currently unused.
        // added (J. P. Kone, 26.07.2016)
        scalar porosity_;


    // Protected Member Functions


        //- Transform the model data wrt mesh changes
        virtual void calcTransformModelData() = 0;

        //- Adjust negative resistance values to be multiplier of max value
        void adjustNegativeResistance(dimensionedVector& resist);

        //- Calculate the porosity force
        virtual void calcForce
        (
            const volVectorField& U,
            const volScalarField& rho,
            const volScalarField& mu,
            vectorField& force
        ) const = 0;

        virtual void correct(fvVectorMatrix& UEqn) const = 0;

        virtual void correct
        (
            fvVectorMatrix& UEqn,
            const volScalarField& rho,
            const volScalarField& mu
        ) const = 0;

        virtual void correct
        (
            const fvVectorMatrix& UEqn,
            volTensorField& AU
        ) const = 0;

        //- Return label index
        label fieldIndex(const label index) const;


public:

    //- Runtime type information
    TypeName("myPorosityModel");

    //- Selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        myPorosityModel,
        mesh,
        (
            const word& modelName,
            const word& name,
            const fvMesh& mesh,
            const dictionary& dict,
            const word& cellZoneName
        ),
        (modelName, name, mesh, dict, cellZoneName)
    );

    //- Constructor
    myPorosityModel
    (
        const word& name,
        const word& modelType,
        const fvMesh& mesh,
        const dictionary& dict,
        const word& cellZoneName = word::null
    );

    //- Return pointer to new myPorosityModel object created on the freestore
    //  from an Istream
    class iNew
    {
        //- Reference to the mesh database
        const fvMesh& mesh_;
        const word& name_;

    public:

        iNew
        (
            const fvMesh& mesh,
            const word& name
        )
        :
            mesh_(mesh),
            name_(name)
        {}

        autoPtr<myPorosityModel> operator()(Istream& is) const
        {
            const dictionary dict(is);

            return autoPtr<myPorosityModel>
            (
                myPorosityModel::New
                (
                    name_,
                    mesh_,
                    dict
                )
            );
        }
    };

    //- Selector
    static autoPtr<myPorosityModel> New
    (
        const word& name,
        const fvMesh& mesh,
        const dictionary& dict,
        const word& cellZoneName = word::null
    );

    //- Destructor
    virtual ~myPorosityModel();


    // Member Functions

        //- Return const access to the porosity model name
        inline const word& name() const;

        //- Return const access to the porosity active flag
        inline bool active() const;

        //- Return const access to the cell zone IDs
        inline const labelList& cellZoneIDs() const;

        //- Transform the model data wrt mesh changes
        virtual void transformModelData();

        // for reading pporosity value (J. P. Kone, 11.09.2016)
        inline const dictionary& dict() const;
        inline scalar porosity() const;
        inline scalar& porosity();

        //- Return the force over the cell zone(s)
        virtual tmp<vectorField> force
        (
            const volVectorField& U,
            const volScalarField& rho,
            const volScalarField& mu
        );

        //- Add resistance
        virtual void addResistance(fvVectorMatrix& UEqn);

        //- Add resistance
        virtual void addResistance
        (
            fvVectorMatrix& UEqn,
            const volScalarField& rho,
            const volScalarField& mu
        );

        //- Add resistance
        virtual void addResistance
        (
            const fvVectorMatrix& UEqn,
            volTensorField& AU,
            bool correctAUprocBC
        );


    // I-O

        //- Write
        virtual bool writeData(Ostream& os) const;

        //- Inherit read from regIOobject
        using regIOobject::read;

        //- Read porosity dictionary
        virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "myPorosityModelI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
