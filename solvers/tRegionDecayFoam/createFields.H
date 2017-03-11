Info<< "Reading transportProperties\n" << endl;

IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

dimensionedScalar nu
(
    "nu",
    dimViscosity,
    transportProperties.lookup("nu")
);
dimensionedScalar lambda
(
    "lambda",
    dimensionSet(0, 0, -1, 0, 0, 0, 0),
    transportProperties.lookup("lambda")
);

Info<< "Reading field p\n" << endl;
/*volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    mesh
);
*/

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

Info<< "Reading field T\n" << endl;
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
Info<< "Reading field kappaEff\n" << endl;
volScalarField kappaEff
(
    IOobject
    (
        "kappaEff",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

/*
Info<< "Reading field U\n" << endl;
volVectorField V
(
    IOobject
    (
        "V",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    mesh
);


//#include "createPhi.H"

surfaceScalarField phi
(
    IOobject
    (
        "phi",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    fvc::flux(V)
);
*/

label TRefCell = 0;
scalar TRefValue = 1.0;
setRefCell(T, mesh.solutionDict().subDict("SIMPLE"), TRefCell, TRefValue);
mesh.setFluxRequired(T.name());
