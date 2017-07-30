Info<< "Reading transportProperties\n" << endl;

#include "setGasConstant.H"

Info<< "Reading field D\n" << endl;
volScalarField D
(
    IOobject
    (
        "D",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
Info<< "Reading diffusion constant field D0\n" << endl;
volScalarField D0
(
    IOobject
    (
        "D0",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
Info<< "Reading diffusion activation energy field Q\n" << endl;
volScalarField Q
(
    IOobject
    (
        "Q",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading tritium field C_T\n" << endl;
volScalarField C_T
(
    IOobject
    (
        "C_T",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading temperature field T\n" << endl;
volScalarField T
(
    IOobject
    (
        "T",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Making flux tracking fields store and flux\n" << endl;
volScalarField store
(
    IOobject
    (
        "store",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedScalar("store", dimless, 0.0)
);
volScalarField flux
(
    IOobject
    (
        "flux",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedScalar("flux", dimless, 0.0)
);
