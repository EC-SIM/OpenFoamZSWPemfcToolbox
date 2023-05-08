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
    functionIntegration

Description
    function integration.  Based on Numerical Recipes in C++, 

    Function is provided as a template parameter function object, evaluated
    using operator()(const scalar x)

Author
    Jean-Paul Kone (25.10.2016).  All rights reserved.

SourceFiles
    functionIntegration.C

\*---------------------------------------------------------------------------*/

#ifndef functionIntegration_H
#define functionIntegration_H

#include "scalar.H" // OpenFOAM src file

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class functionIntegration Declaration
\*---------------------------------------------------------------------------*/

template<class Func>
class functionIntegration
{
private:

    // Private data

        //- Reference to a function
        const Func& f_;
public:

    // Static data members

        //- Integral value

    // Constructors

        //- Construct given a function
        functionIntegration(const Func& f);

    // Destructor - default


    // Member Functions

        //- Return integral
        scalar integral(const scalar a, const scalar b, int n) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "functionIntegration.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
