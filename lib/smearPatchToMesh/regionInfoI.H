/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Description

\*---------------------------------------------------------------------------*/

// Code changed and converted to openfoam-4.0 (J.P.Kone, 06.07.2016)
// class trackingData is added, and td_ is passed as argument


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Update this with w2 information
inline bool Foam::regionInfo::update
(
    const regionInfo& w2,
    const label thisFaceI,
    const label thisCellI,
    const label neighbourFaceI,
    const label neighbourCellI
)
{
    if (w2.region() == -1)
    {
        FatalErrorIn
        (
            "regionInfo::update(const regionInfo&, const label,"
            "const label, const label)"
        ) << "Problem: w2 is -1" << abort(FatalError);

        return false;
    }
    else if (region() != -1)
    {
        // our already set. Stop any transfer
        return false;
    }
    else
    {
        region_ = w2.region();

        return true;
    }
}
    

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Null constructor
inline Foam::regionInfo::regionInfo()
:
    region_(-1)
{}


// Construct from components
inline Foam::regionInfo::regionInfo(const label region)
:
    region_(region)
{}


// Construct as copy
inline Foam::regionInfo::regionInfo(const regionInfo& w2)
:
    region_(w2.region())
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class TrackingData>
inline bool Foam::regionInfo::valid(TrackingData& td) const
{
    return region_ != -1;
}


// No geometric data so never any problem on cyclics
template<class TrackingData>
inline bool Foam::regionInfo::sameGeometry
(
    const polyMesh&,
    const regionInfo& w2,
    const scalar tol,
    TrackingData& td
) const
{
    return true;
}


// No geometric data.
template<class TrackingData>
inline void Foam::regionInfo::leaveDomain
(
    const polyMesh&,
    const polyPatch& patch,
    const label patchFaceI,
    const point& faceCentre,
    TrackingData& td
)
{}


// No geometric data.
template<class TrackingData>
inline void Foam::regionInfo::transform
(
    const polyMesh&,
    const tensor& rotTensor,
    TrackingData& td
)
{}


// No geometric data.
template<class TrackingData>
inline void Foam::regionInfo::enterDomain
(
    const polyMesh&,
    const polyPatch& patch,
    const label patchFaceI,
    const point& faceCentre,
    TrackingData& td
)
{}


// Update this with neighbour information
template<class TrackingData>
inline bool Foam::regionInfo::updateCell
(
    const polyMesh&,
    const label thisCellI,
    const label neighbourFaceI,
    const regionInfo& neighbourInfo,
    const scalar tol,
    TrackingData& td
)
{
    return update
    (
        neighbourInfo,
        -1,
        thisCellI,
        neighbourFaceI,
        -1
    );
}    


// Update this with neighbour information
template<class TrackingData>
inline bool Foam::regionInfo::updateFace
(
    const polyMesh&,
    const label thisFaceI,
    const label neighbourCellI,
    const regionInfo& neighbourInfo,
    const scalar tol,
    TrackingData& td
)
{
    return update
    (
        neighbourInfo,
        thisFaceI,
        -1,
        -1,
        neighbourCellI
    );
}    

// Update this with neighbour information
template<class TrackingData>
inline bool Foam::regionInfo::updateFace
(
    const polyMesh&,
    const label thisFaceI,
    const regionInfo& neighbourInfo,
    const scalar tol,
    TrackingData& td
)
{
    return update
    (
        neighbourInfo,
        thisFaceI,
        -1,
        -1,
        -1
    );
}    

// Same (like operator==)
// member function added (J.P.Kone, 06.07.2016)
template<class TrackingData>
inline bool Foam::regionInfo::equal
(
    const regionInfo& rhs,
    TrackingData& td
) const
{
    return operator==(rhs);
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool Foam::regionInfo::operator==(const Foam::regionInfo& rhs) const
{
    return region() == rhs.region();
}


inline bool Foam::regionInfo::operator!=(const Foam::regionInfo& rhs) const
{
    return !(*this == rhs);
}


// ************************************************************************* //
