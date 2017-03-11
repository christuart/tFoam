/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2013-2016 OpenFOAM Foundation
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
    Foam::tritiumGradientFvPatchField

Group
    grpGenericBoundaryConditions

Description
    This boundary condition provides a uniform fixed gradient condition.

Usage
    \table
        Property     | Description             | Required    | Default value
        uniformGradient | uniform gradient     | yes         |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            tritiumGradient;
        uniformGradient constant 0.2;
    }
    \endverbatim

Note
    The uniformGradient entry is a Function1 type, able to describe time
    varying functions.  The example above gives the usage for supplying a
    constant value.

See also
    Foam::Function1Types
    Foam::fixedGradientFvPatchField

SourceFiles
    tritiumGradientFvPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef tritiumGradientFvPatchField_H
#define tritiumGradientFvPatchField_H

#include "fixedGradientFvPatchFields.H"
#include "Function1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


namespace Foam
{

/*---------------------------------------------------------------------------*\
              Class tritiumGradientFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class tritiumGradientFvPatchField
:
    public fixedGradientFvPatchField<Type>
{
public:

    // Public data

private:
    // Private data

        //- Gradient
        autoPtr<Function1<Type>> uniformGradient_;
        
        //- Outgassing Coefficient / [kg/m2s / (kg/m2)^0.74]
        scalarField o_;
        
        //- Outgassing Exponent / [kg/m2s / kg/m3]
        scalarField n_;
        
        //- Surface activity distance Delta / [m]
        scalarField delta_;
        
        //- Recalculated coefficient / [kg/m2s / (kg/m3)^0.74]
		scalarField r_;

public:

    //- Runtime type information
    TypeName("tritiumGradient");


    // Constructors

        //- Construct from patch and internal field
        tritiumGradientFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch and internal field and patch field
        tritiumGradientFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const Field<Type>& fld
        );

        //- Construct from patch, internal field and dictionary
        tritiumGradientFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given tritiumGradientFvPatchField
        //  onto a new patch
        tritiumGradientFvPatchField
        (
            const tritiumGradientFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        tritiumGradientFvPatchField
        (
            const tritiumGradientFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type>> clone() const
        {
            return tmp<fvPatchField<Type>>
            (
                new tritiumGradientFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        tritiumGradientFvPatchField
        (
            const tritiumGradientFvPatchField<Type>&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchField<Type>> clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return tmp<fvPatchField<Type>>
            (
                new tritiumGradientFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "tritiumGradientFvPatchField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
