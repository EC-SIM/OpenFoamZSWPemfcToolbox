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

Class
    regionInfo

Description
    Holds information regarding region of cell. Used in 'regioning' a mesh.

SourceFiles
    regionInfoI.H
    regionInfo.C

\*---------------------------------------------------------------------------*/

#ifndef regionInfo_H
#define regionInfo_H

#include "point.H"
#include "label.H"
#include "scalar.H"
#include "tensor.H"
#include "polyMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Code changed and converted to openfoam-4.0 (J.P.Kone, 06.07.2016)
// class trackingData is added, and td_ is passed as argument

namespace Foam
{

class polyPatch;

/*---------------------------------------------------------------------------*\
                           Class regionInfo Declaration
\*---------------------------------------------------------------------------*/

class regionInfo
{
    // Private data

        label region_;
        
    // Private Member Functions

        //- Update current cell/face type with neighbouring
        //  type. Return true if information needs to propagate,
        //  false otherwise.
        inline bool update
        (
            const regionInfo& w2,
            const label thisFaceI,
            const label thisCellI,
            const label neighbourFaceI,
            const label neighbourCellI
        );

public:

    // Constructors

        //- Construct null
        inline regionInfo();

        //- Construct from cType
        explicit inline regionInfo(const label);

        //- Construct as copy
        inline regionInfo(const regionInfo&);


    // Member Functions

        // Access

            inline label region() const
            {
                return region_;
            }


        // Needed by meshWave

            //- Check whether origin has been changed at all or
            //  still contains original (invalid) value.
            // 'TrackingData' added (J.P.Kone, 06.07.2016)
            template<class TrackingData>
            inline bool valid(TrackingData& td) const;

            //- Check for identical geometrical data. Used for cyclics checking.
            template<class TrackingData>
            inline bool sameGeometry
            (
                const polyMesh&,
                const regionInfo& w2,
                const scalar tol,
                TrackingData& td
            ) const;

            //- Convert any absolute coordinates into relative to (patch)face
            //  centre
            template<class TrackingData>
            inline void leaveDomain
            (
                const polyMesh&,
                const polyPatch& patch,
                const label patchFaceI,
                const point& faceCentre,
                TrackingData& td
            );

            //- Reverse of leaveDomain
            template<class TrackingData>
            inline void enterDomain
            (
                const polyMesh&,
                const polyPatch& patch,
                const label patchFaceI,
                const point& faceCentre,
                TrackingData& td
            );

            //- Apply rotation matrix to any coordinates
            template<class TrackingData>
            inline void transform
            (
                const polyMesh&,
                const tensor& rotTensor,
                TrackingData& td
            );

            //- Influence of neighbouring face.
            template<class TrackingData>
            inline bool updateCell
            (
                const polyMesh&,
                const label thisCellI,
                const label neighbourFaceI,
                const regionInfo& neighbourInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of neighbouring cell.
            template<class TrackingData>
            inline bool updateFace
            (
                const polyMesh&,
                const label thisFaceI,
                const label neighbourCellI,
                const regionInfo& neighbourInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of different value on same face.
            template<class TrackingData>
            inline bool updateFace
            (
                const polyMesh&,
                const label thisFaceI,
                const regionInfo& neighbourInfo,
                const scalar tol,
                TrackingData& td
            );
            // Same (like operator==)
            // member function added (J.P.Kone, 06.07.2016)
            template<class TrackingData>
            inline bool equal
            (
                const regionInfo& rhs,
                TrackingData& td
            ) const;

    // Member Operators

        // Note: Used to determine whether to call update.
        inline bool operator==(const regionInfo&) const;

        inline bool operator!=(const regionInfo&) const;


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const regionInfo&);
        friend Istream& operator>>(Istream&, regionInfo&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "regionInfoI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
