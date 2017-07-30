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

dimensionedScalar D
(
    "D",
    dimViscosity,
    transportProperties.lookup("D")
);
dimensionedScalar lambda
(
    "lambda",
    dimensionSet(0, 0, -1, 0, 0, 0, 0),
    transportProperties.lookup("lambda")
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
