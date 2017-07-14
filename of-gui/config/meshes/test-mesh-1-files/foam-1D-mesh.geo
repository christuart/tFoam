Point(1) = {0, 0, 0, 0.001};
Point(2) = {0, 0.001, 0, 0.001};
Point(3) = {0, 0.001, 0.001, 0.001};
Point(4) = {0, 0, 0.001, 0.001};
Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 1};

Transfinite Line {1} = 2 Using Progression 1;
Transfinite Line {2} = 2 Using Progression 1;
Transfinite Line {3} = 2 Using Progression 1;
Transfinite Line {4} = 2 Using Progression 1;

Line Loop(6) = {1, 2, 3, 4};
Plane Surface(6) = {6};

Transfinite Surface{6};
Recombine Surface{6};


surfaceVector[] = Extrude {0.005, 0, 0} {
  Surface{6};
  Layers{20};
  Recombine;
};

Physical Surface("outlet") = surfaceVector[0];
Physical Volume("internal") = surfaceVector[1];
Physical Surface("sides") = {surfaceVector[2], surfaceVector[3], surfaceVector[4], surfaceVector[5]};
Physical Surface("inlet") = {6}; // from Plane Surface (6)
