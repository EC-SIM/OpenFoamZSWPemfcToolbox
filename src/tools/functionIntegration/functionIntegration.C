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
    Ridder's method of root finiding given a function, bracketed root
    and accuracy.  Based on Numerical Recipes in C++, Section 9.2,
    page 362.

Author
    Jean-Paul Kone (25.10.2016).  All rights reserved.

\*----------------------------------------------------------------------------*/

#include "functionIntegration.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Func>
functionIntegration<Func>::functionIntegration
(
    const Func& f
)
:
    f_(f)
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Func>
scalar functionIntegration<Func>::integral
(
    scalar a,
    scalar b,
    int n
) const
{
    scalar x,tnm,sum,del;
    static scalar s;
    int it,j;

    if (n == 1) 
    {
        return (s=0.5*(b-a)*(f_(a)+f_(b)));
    } 
    else 
    {
        for (it=1,j=1;j<n-1;j++) it <<= 1;
        
        tnm=it;
        del=(b-a)/tnm; //This is the spacing of the points to be added.
        x=a+0.5*del;
        
        for (sum=0.0,j=1;j<=it;j++,x+=del) sum += f_(x);
        s=0.5*(s+(b-a)*sum/tnm); //This replaces s by its refined value.
        return s;
    }
    // Dummy return to keep compiler happy
    return s;
}
} // end of namespace Foam

// ************************************************************************* //
