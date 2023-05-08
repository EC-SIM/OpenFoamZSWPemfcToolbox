/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright 2007 Hrvoje Jasak.  All rights reserved
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

#include "myContinuityErrs.H"
#include "fvCFD.H" // OpenFOAM src file

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

// Code changed and adapted (J.P.Kone, 14.09.2016)

void Foam::myContinuityErrs(const surfaceScalarField& phi)
{
    volScalarField contErr = fvc::div(phi);

    scalar sumLocalContErr = phi.mesh().time().deltaT().value()*
        mag(contErr)().weightedAverage(phi.mesh().V()).value();

    scalar globalContErr = phi.mesh().time().deltaT().value()*
        contErr.weightedAverage(phi.mesh().V()).value();

    Info<< "time step continuity errors : sum local = " << sumLocalContErr
        << ", global = " << globalContErr
        << endl;
}


// ************************************************************************* //
