/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2009 OpenCFD Ltd.
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
    Foam::activeBaffleVelocityFvPatchVectorField

Description
    Foam::activeBaffleVelocityFvPatchVectorField

SourceFiles
    activeBaffleVelocityFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef activeBaffleVelocityFvPatchVectorField_H
#define activeBaffleVelocityFvPatchVectorField_H

#include "fvPatchFields.H"
#include "fixedValueFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                 Class activeBaffleVelocityFvPatch Declaration
\*---------------------------------------------------------------------------*/

class activeBaffleVelocityFvPatchVectorField
:
    public fixedValueFvPatchVectorField
{
    // Private data

        word pName_;
        word cyclicPatchName_;
        label cyclicPatchLabel_;

        //- Initial wall patch areas
        vectorField initWallSf_;

        //- Initial cyclic patch areas
        vectorField initCyclicSf_;

        //- Current fraction of the active baffle which is open
        scalar openFraction_;

        //- Fractional change to the active baffle openness per time-step
        scalar openFractionDelta_;

        label curTimeIndex_;


public:

    //- Runtime type information
    TypeName("activeBaffleVelocity");


    // Constructors

        //- Construct from patch and internal field
        activeBaffleVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        activeBaffleVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given activeBaffleVelocityFvPatchVectorField
        //  onto a new patch
        activeBaffleVelocityFvPatchVectorField
        (
            const activeBaffleVelocityFvPatchVectorField&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        activeBaffleVelocityFvPatchVectorField
        (
            const activeBaffleVelocityFvPatchVectorField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchVectorField> clone() const
        {
            return tmp<fvPatchVectorField>
            (
                new activeBaffleVelocityFvPatchVectorField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        activeBaffleVelocityFvPatchVectorField
        (
            const activeBaffleVelocityFvPatchVectorField&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchVectorField> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return tmp<fvPatchVectorField>
            (
                new activeBaffleVelocityFvPatchVectorField(*this, iF)
            );
        }


    // Member functions

        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchVectorField&,
                const labelList&
            );


        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
