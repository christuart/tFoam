/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2013-2016 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of tFoam, a project implemented using OpenFOAM.

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

#include "tritiumGradientFvPatchScalarField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "surfaceFields.H"
#include "volFields.H"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::tritiumGradientFvPatchScalarField::tritiumGradientFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedGradientFvPatchScalarField(p, iF),
    store(p.size(), 0.0),
    flux(p.size(), 0.0),
//    uniformGradient_(),
    o_(p.size(), 1.23e-4),
    n_(p.size(), 0.74),
    delta_(p.size(), 3e-10),
    DName_("D"),
    timeStoreLastUpdated(-1.0)
{
	Info << "Executing constructor number 0" << endl;
	r_ = o_*pow(delta_,n_);
}

/*
Foam::tritiumGradientFvPatchScalarField::tritiumGradientFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const scalarField& fld
)
:
    fixedGradientFvPatchScalarField(p, iF, fld),
    uniformGradient_()
{}
*/


Foam::tritiumGradientFvPatchScalarField::tritiumGradientFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedGradientFvPatchField<scalar>(p, iF),
//    uniformGradient_(Function1<scalar>::New("uniformGradient", dict)),
    /*mode_(unknown),
    q_(p.size(), 0.0),
    h_(p.size(), 0.0),
    Ta_(p.size(), 0.0),
    QrPrevious_(p.size(), 0.0),
    QrRelaxation_(dict.lookupOrDefault<scalar>("relaxation", 1)),
    QrName_(dict.lookupOrDefault<word>("Qr", "none")),
    thicknessLayers_(),
    kappaLayers_()*/
    store(p.size(), 0.0),
    flux(p.size(), 0.0),
    o_(p.size(), 0.0),
    n_(p.size(), 0.0),
    delta_(p.size(), 0.0),
    r_(p.size(), 0.0),
    DName_("D"),
    timeStoreLastUpdated(-1.0)
{
	Info << "Executing constructor number 1" << endl;
    if (dict.found("coefficient") && dict.found("exponent") && dict.found("delta"))
    {
        o_ = scalarField("coefficient", dict, p.size());
        n_ = scalarField("exponent", dict, p.size());
        delta_ = scalarField("delta", dict, p.size());
        //~ Info << "o_ is set to " << o_[0] << " so that's cool" << endl;
        //~ Info << "n_ is set to " << n_[0] << " so that's cool" << endl;
        //~ Info << "delta_ is set to " << delta_[0] << " so that's cool" << endl;
        r_ = o_*pow(delta_,n_);
        //~ Info << "r_ is set to " << r_[0] << " so that's cool" << endl;
    }
    else
    {
        FatalErrorInFunction
            << "\n patch type '" << p.type()
            << "' either o (coefficient) or n (exponent) or delta were not found '"
            << "\n for patch " << p.name()
//            << " of field " << this->internalField().name()
//            << " in file " << this->internalField().objectPath()
            << "\n typical default values are: 1.23e-4; 0.74; 3e-10"
            << exit(FatalError);
    }
	Info << "This is what is re-evaluating" << endl;
    this->evaluate();
}


Foam::tritiumGradientFvPatchScalarField::tritiumGradientFvPatchScalarField
(
    const tritiumGradientFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedGradientFvPatchField<scalar>(ptf, p, iF, mapper)/*,
    uniformGradient_(ptf.uniformGradient_, false)*/
{
	Info << "Executing constructor number 2" << endl;}


Foam::tritiumGradientFvPatchScalarField::tritiumGradientFvPatchScalarField
(
    const tritiumGradientFvPatchScalarField& ptf
)
:
    fixedGradientFvPatchField<scalar>(ptf)/*,
    uniformGradient_(ptf.uniformGradient_, false)*/
{
	Info << "Executing constructor number 3" << endl;}


Foam::tritiumGradientFvPatchScalarField::tritiumGradientFvPatchScalarField
(
    const tritiumGradientFvPatchScalarField& ptf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedGradientFvPatchField<scalar>(ptf, iF),
    store(ptf.store),
    flux(ptf.flux),
//    uniformGradient_(ptf.uniformGradient_, false),
    o_(ptf.o_),
    n_(ptf.n_),
    delta_(ptf.delta_),
    r_(ptf.r_),
    DName_(ptf.DName_),
    timeStoreLastUpdated(ptf.timeStoreLastUpdated)
{
	Info << "Executing constructor number 4" << endl;
    // Evaluate the profile if defined
/*    if (ptf.uniformGradient_.valid())
    {
        this->evaluate();
    }*/
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::tritiumGradientFvPatchScalarField::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

	//Info << "We have reached updateCoeffs() of the tritiumGradient boundary!" << endl;
	
//    const scalar t = this->db().time().timeOutputValue();
    //this->gradient() = uniformGradient_->value(t);
    //this->gradient() = n_;
    //Info << (Field<scalar>&(this->patchInternalField()).size()) << " vs " << n_.size() << endl;
	//Info << "We set the gradient to the wrong thing!" << endl;
    //this->gradient() = pow(this->patchInternalField(),n_);
	//Info << "We set the gradient to another a a wrong thing!" << endl;
	
    //const fvPatchField<scalar>& pp =
    //    patch().lookupPatchField<volScalarField, scalar>(pName_);
        
    const fvMesh& mesh = dimensionedInternalField().mesh();
    const fvPatchField<scalar>& D =
        patch().lookupPatchField<volScalarField, scalar>(DName_);
    const label patchID = mesh.boundaryMesh().findPatchID(this->patch().name());
    flux = (r_ * pow(this->patchInternalField(),n_));
    const scalar timeStep = this->db().time().deltaTValue();
    if (timeStoreLastUpdated != this->db().time().timeOutputValue())
    {
        timeStoreLastUpdated = this->db().time().timeOutputValue();
        store = store + flux * mesh.magSf().boundaryField()[patchID] * timeStep;
    }
    //fvPatchField<scalar>& fluxField = 
    //    patch().lookupPatchField<volScalarField, scalar>("flux");
    this->gradient() = flux / D;

    fixedGradientFvPatchField<scalar>::updateCoeffs();
}


void Foam::tritiumGradientFvPatchScalarField::write(Ostream& os) const
{
    fixedGradientFvPatchField<scalar>::write(os);
//    uniformGradient_->writeData(os);
    this->writeEntry("value", os);
    flux.writeEntry("flux", os);
    store.writeEntry("store", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        tritiumGradientFvPatchScalarField
    );
}

// ************************************************************************* //
