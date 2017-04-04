/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
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
    Foam::pressureSolubilityFvPatchScalarField

Group
    grpGenericBoundaryConditions

Description
    This boundary condition supplies a fixed value constraint, and is the base
    class for a number of other boundary conditions.

    \heading Patch usage

    \table
        Property     | Description             | Required    | Default value
        value        | Patch face values       | yes         |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            fixedValue;
        value           uniform 0;  // example for scalar field usage
    }
    \endverbatim

SourceFiles
    pressureSolubilityFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef pressureSolubilityFvPatchScalarField_H
#define pressureSolubilityFvPatchScalarField_H

//#include "fvPatchField.H"
#include "Random.H"
#include "fixedValueFvPatchFields.H"
//#include "InterpolationTable.H"
#ifdef USING_OF30
#include "DataEntry.H"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class pressureSolubilityFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class pressureSolubilityFvPatchScalarField
:
    public fixedValueFvPatchField<scalar>
{

private:
        
        //- External partial pressure / [Pa]
        scalar p_;

        //- Solubility / [kg/m3 / sqrt(Pa)]
        scalar s_;

        //- Current time index
        label curTimeIndex_;

public:

    //- Runtime type information
    TypeName("pressureSolubility");


    // Constructors

        //- Construct from patch and internal field
        pressureSolubilityFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        pressureSolubilityFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping the given pressureSolubilityFvPatchScalarField
        //  onto a new patch
        pressureSolubilityFvPatchScalarField
        (
            const pressureSolubilityFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        pressureSolubilityFvPatchScalarField
        (
            const pressureSolubilityFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<scalar> > clone() const
        {
            return tmp<fvPatchField<scalar> >
            (
                new pressureSolubilityFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        pressureSolubilityFvPatchScalarField
        (
            const pressureSolubilityFvPatchScalarField&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchField<scalar> > clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchField<scalar> >
            (
                new pressureSolubilityFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Return amplitude
            scalar pressure() const
            {
                return p_;
            }

            scalar& pressure()
            {
                return p_;
            }

            //- Return amplitude
            scalar solubility() const
            {
                return s_;
            }

            scalar& solubility()
            {
                return s_;
            }


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        //- Write
        virtual void write(Ostream&) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
//#   include "pressureSolubilityFvPatchScalarField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
