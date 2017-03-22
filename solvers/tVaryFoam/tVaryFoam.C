/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
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

Application
    tVaryFoam

Description
    Transient solver for multi-zone tritium diffusion, with diffusion
    coefficients calculated as a function of temperature.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "simpleControl.H"
#include "tritiumGradientFvPatchScalarField.H"
//#include "GeometricField.H"
//#include "GeometricBoundaryField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"

    #include "createFields.H"

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.loop())
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;
            
        D = D0 * exp( -Q/(R*T) );
        
		fvScalarMatrix TEqn
		(
			fvm::ddt(C_T)
		  - fvm::laplacian(D, C_T)
		);
		
		TEqn.relax();
		solve(TEqn);
		C_T.correctBoundaryConditions();
		
		//tritiumGradientFvPatchScalarField tritiumPatchFieldI = C_T.boundaryField().patch[0];
		//forAll(C_T.boundaryField(),I) {
		//	tritiumGradientFvPatchScalarField& tritiumPatchFieldI = C_T.boundaryField()[I]
		//	flux.boundaryField()[I] = tritiumPathFieldI.flux;
		//flux

        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
