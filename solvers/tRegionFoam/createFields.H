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


label TRefCell = 0;
scalar TRefValue = 1.0;
setRefCell(T, mesh.solutionDict().subDict("SIMPLE"), TRefCell, TRefValue);
mesh.setFluxRequired(T.name());
