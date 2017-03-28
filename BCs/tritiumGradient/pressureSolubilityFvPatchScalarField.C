/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2014 OpenFOAM Foundation
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

#include "pressureSolubilityFvPatchScalarField.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

pressureSolubilityFvPatchScalarField::pressureSolubilityFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(p, iF),
    p_(),
    s_(),
    curTimeIndex_(-1)
{}


pressureSolubilityFvPatchScalarField::pressureSolubilityFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchScalarField(p, iF),
    p_(0.0),
    s_(0.0),
    curTimeIndex_(-1)
{
//	Info << "Executing constructor number 1" << endl;
    if (dict.found("partialPressure") && dict.found("solubility"))
    {
        p_ = readScalar(dict.lookup("partialPressure"));
        s_ = readScalar(dict.lookup("solubility"));
    }
    else
    {
        FatalErrorInFunction
            << "\n patch type '" << p.type()
            << "' either partialPressure or solubility were not found '"
            << "\n for patch " << p.name()
//            << " of field " << this->internalField().name()
//            << " in file " << this->internalField().objectPath()
            << exit(FatalError);
    }
    this->evaluate();
}


pressureSolubilityFvPatchScalarField::pressureSolubilityFvPatchScalarField
(
    const pressureSolubilityFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchScalarField(ptf, p, iF, mapper),
    p_(ptf.pressure()),
    s_(ptf.solubility()),
    curTimeIndex_(-1)
{
    if (notNull(iF) && mapper.hasUnmapped())
    {
        WarningIn
        (
            "pressureSolubilityFvPatchScalarField::pressureSolubilityFvPatchScalarField\n"
            "(\n"
            "    const pressureSolubilityFvPatchScalarField&,\n"
            "    const fvPatch&,\n"
            "    const DimensionedField<scalar, volMesh>&,\n"
            "    const fvPatchFieldMapper&\n"
            ")\n"
        )   << "On field " << iF.name() << " patch " << p.name()
            << " patchField " << this->type()
            << " : mapper does not map all values." << nl
            << "    To avoid this warning fully specify the mapping in derived"
            << " patch fields." << endl;
    }
}


pressureSolubilityFvPatchScalarField::pressureSolubilityFvPatchScalarField
(
    const pressureSolubilityFvPatchScalarField& ptf
)
:
    fixedValueFvPatchScalarField(ptf),
    p_(ptf.pressure()),
    s_(ptf.solubility()),
    curTimeIndex_(-1)
{}


pressureSolubilityFvPatchScalarField::pressureSolubilityFvPatchScalarField
(
    const pressureSolubilityFvPatchScalarField& ptf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(ptf, iF),
    p_(ptf.pressure()),
    s_(ptf.solubility()),
    curTimeIndex_(-1)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void pressureSolubilityFvPatchScalarField::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    if (curTimeIndex_ != this->db().time().timeIndex())
    {
        fixedValueFvPatchField<scalar>::operator==
        (
            s_*sqrt(p_)
        );

        curTimeIndex_ = this->db().time().timeIndex();
    }

    fixedValueFvPatchField<scalar>::updateCoeffs();
}

void pressureSolubilityFvPatchScalarField::write(Ostream& os) const
{
    fixedValueFvPatchField<scalar>::write(os);
    //this->writeEntry("value", os);
    os.writeKeyword("partialPressure") << pressure() << token::END_STATEMENT << nl;
    os.writeKeyword("solubility") << solubility() << token::END_STATEMENT << nl;
    //p_.writeEntry("partialPressure", os);
    //s_.writeEntry("solubility", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam




// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        pressureSolubilityFvPatchScalarField
    );
}

// ************************************************************************* //

// ************************************************************************* //
