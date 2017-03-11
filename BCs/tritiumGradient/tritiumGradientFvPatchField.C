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

\*---------------------------------------------------------------------------*/

#include "tritiumGradientFvPatchField.H"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
Foam::tritiumGradientFvPatchField<Type>::tritiumGradientFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedGradientFvPatchField<Type>(p, iF),
    uniformGradient_(),
    o_(p.size(), 1.23e-4),
    n_(p.size(), 0.74),
    delta_(p.size(), 3e-10)
{
	r_ = o_*pow(delta_,n_);
}


template<class Type>
Foam::tritiumGradientFvPatchField<Type>::tritiumGradientFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const Field<Type>& fld
)
:
    fixedGradientFvPatchField<Type>(p, iF, fld),
    uniformGradient_()
{}


template<class Type>
Foam::tritiumGradientFvPatchField<Type>::tritiumGradientFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fixedGradientFvPatchField<Type>(p, iF),
    uniformGradient_(Function1<Type>::New("uniformGradient", dict)),
    /*mode_(unknown),
    q_(p.size(), 0.0),
    h_(p.size(), 0.0),
    Ta_(p.size(), 0.0),
    QrPrevious_(p.size(), 0.0),
    QrRelaxation_(dict.lookupOrDefault<scalar>("relaxation", 1)),
    QrName_(dict.lookupOrDefault<word>("Qr", "none")),
    thicknessLayers_(),
    kappaLayers_()*/
    o_(p.size(), 0.0),
    n_(p.size(), 0.0),
    delta_(p.size(), 0.0),
    r_(p.size(), 0.0)
{
    if (dict.found("coefficient") && dict.found("exponent") && dict.found("delta"))
    {
        o_ = scalarField("coefficient", dict, p.size());
        n_ = scalarField("exponent", dict, p.size());
        delta_ = scalarField("delta", dict, p.size());
        r_ = o_*pow(delta_,n_);
    }
    else
    {
        FatalErrorInFunction
            << "\n patch type '" << p.type()
            << "' either o (coefficient) or n (exponent) or delta were not found '"
            << "\n for patch " << p.name()
            << " of field " << this->internalField().name()
            << " in file " << this->internalField().objectPath()
            << "\n typical default values are: 1.23e-4; 0.74; 3e-10"
            << exit(FatalError);
    }
    this->evaluate();
}


template<class Type>
Foam::tritiumGradientFvPatchField<Type>::tritiumGradientFvPatchField
(
    const tritiumGradientFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedGradientFvPatchField<Type>(ptf, p, iF, mapper),
    uniformGradient_(ptf.uniformGradient_, false)
{}


template<class Type>
Foam::tritiumGradientFvPatchField<Type>::tritiumGradientFvPatchField
(
    const tritiumGradientFvPatchField<Type>& ptf
)
:
    fixedGradientFvPatchField<Type>(ptf),
    uniformGradient_(ptf.uniformGradient_, false)
{}


template<class Type>
Foam::tritiumGradientFvPatchField<Type>::tritiumGradientFvPatchField
(
    const tritiumGradientFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedGradientFvPatchField<Type>(ptf, iF),
    uniformGradient_(ptf.uniformGradient_, false)
{
    // Evaluate the profile if defined
    if (ptf.uniformGradient_.valid())
    {
        this->evaluate();
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void Foam::tritiumGradientFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    const scalar t = this->db().time().timeOutputValue();
    this->gradient() = uniformGradient_->value(t);
    //this->gradient() = r_ * pow(this->patchInternalField(),n_);

    fixedGradientFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void Foam::tritiumGradientFvPatchField<Type>::write(Ostream& os) const
{
    fixedGradientFvPatchField<Type>::write(os);
    uniformGradient_->writeData(os);
    this->writeEntry("value", os);
}


// ************************************************************************* //
