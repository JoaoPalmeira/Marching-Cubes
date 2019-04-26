#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*
   int triTable[256][16] also corresponds to the 256 possible combinations
   of vertices.
   The [16] dimension of the table is again the list of edges of the cube
   which are intersected by the surface.  This time however, the edges are
   enumerated in the order of the vertices making up the triangle mesh of
   the surface.  Each edge contains one vertex that is on the surface.
   Each triple of edges listed in the table contains the vertices of one
   triangle on the mesh.  The are 16 entries because it has been shown that
   there are at most 5 triangles in a cube and each "edge triple" list is
   terminated with the value -1.
   For example triTable[3] contains
   {1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
   This corresponds to the case of a cube whose vertex 0 and 1 are inside
   of the surface and the rest of the verts are outside (00000001 bitwise
   OR'ed with 00000010 makes 00000011 == 3).  Therefore, this cube is
   intersected by the surface roughly in the form of a plane which cuts
   edges 8,9,1 and 3.  This quadrilateral can be constructed from two
   triangles: one which is made of the intersection vertices found on edges
   1,8, and 3; the other is formed from the vertices on edges 9,8, and 1.
   Remember, each intersected edge contains only one surface vertex.  The
   vertex triples are listed in counter clockwise order for proper facing.
*/
const int triTable[256][16] =
{{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};

/*
   int edgeTable[256].  It corresponds to the 2^8 possible combinations of
   of the eight (n) vertices either existing inside or outside (2^n) of the
   surface.  A vertex is inside of a surface if the value at that vertex is
   less than that of the surface you are scanning for.  The table index is
   constructed bitwise with bit 0 corresponding to vertex 0, bit 1 to vert
   1.. bit 7 to vert 7.  The value in the table tells you which edges of
   the table are intersected by the surface.  Once again bit 0 corresponds
   to edge 0 and so on, up to edge 12.
   Constructing the table simply consisted of having a program run thru
   the 256 cases and setting the edge bit if the vertices at either end of
   the edge had different values (one is inside while the other is out).
   The purpose of the table is to speed up the scanning process.  Only the
   edges whose bit's are set contain vertices of the surface.
   Vertex 0 is on the bottom face, back edge, left side.
   The progression of vertices is clockwise around the bottom face
   and then clockwise around the top face of the cube.  Edge 0 goes from
   vertex 0 to vertex 1, Edge 1 is from 2->3 and so on around clockwise to
   vertex 0 again. Then Edge 4 to 7 make up the top face, 4->5, 5->6, 6->7
   and 7->4.  Edge 8 thru 11 are the vertical edges from vert 0->4, 1->5,
   2->6, and 3->7.
       4--------5     *---4----*
      /|       /|    /|       /|
     / |      / |   7 |      5 |
    /  |     /  |  /  8     /  9
   7--------6   | *----6---*   |
   |   |    |   | |   |    |   |
   |   0----|---1 |   *---0|---*
   |  /     |  /  11 /     10 /
   | /      | /   | 3      | 1
   |/       |/    |/       |/
   3--------2     *---2----*
*/
const int edgeTable[256]={
0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0   };

typedef struct {
   double x,y,z;
} XYZ;

typedef struct{
    int x,y,z;
} MATIND;

typedef struct {
   XYZ p[8];
   XYZ n[8];
   double val[8];
} GRIDCELL;

typedef struct {
   XYZ p[3];         /* Vertices */
   XYZ c;            /* Centroid */
   XYZ n;            /* Normal   */
   MATIND indexes[3];     /*VERTEX index*/
} TRIANGLE;

typedef struct{
   double z;   /*vertex*/
   unsigned long int total; /*total triangles*/
   int tri_index[10]; /*triangle indexes*/
} VERTEXZ;

typedef struct{
   unsigned long int flag;
   unsigned long int size;
   double y;
   VERTEXZ *z;
} VERTEXY;

typedef struct{
   unsigned long int flag;
   unsigned long int size;
   double x;
   VERTEXY *y;
} VERTEX;

#define ABS(x) (x < 0 ? -(x) : (x))
#define INDEX(a) (a-1)
#define MAX(a,b) (((a)>(b))?(a):(b))

// API
int PolygoniseCube(GRIDCELL,double,TRIANGLE *);
XYZ VertexInterp(double,XYZ,XYZ,double,double);
XYZ CalcGridNormal(int [],int,int,int);
XYZ NormalInterp(double,XYZ,XYZ,double,double);
void Normalise(XYZ *);
XYZ CalcNormal(XYZ,XYZ,XYZ);
int vertEqual(XYZ, XYZ);
unsigned long int *updateVert(VERTEX **, XYZ, unsigned int, unsigned long int);
XYZ getNormal(TRIANGLE *, VERTEX **, MATIND);
char **splitting(char *);
int growVertex(VERTEX **, unsigned long int *);
int growVertexY(VERTEXY **, unsigned long int, unsigned long int);
int growVertexZ(VERTEXZ **, unsigned long int, unsigned long int);
void allocVertex(VERTEX **,int total);
void allocVertexY (VERTEXY **, unsigned long int);
void allocVertexZ (VERTEXZ **, unsigned long int);

int NX = 200;
int NY = 160;
int NZ = 160;
unsigned long int totalcoord = 0;
unsigned long int sizecoord = 0;

int main(int argc,char **argv)
{
	unsigned int i,j,k,l,n,c;
	short int ***data = NULL;
	short int isolevel = 128,themax = 0,themin = 255;
	GRIDCELL grid;
	TRIANGLE triangles[10];
	TRIANGLE *tri = NULL;
    VERTEX *vert = NULL;
    XYZ grid_normal;
	unsigned long int ntri = 0;
    int nvert = 0;
	FILE *fptr;

	// Check the command line, must at least contain the name of a raw volume file
	if (argc < 2) {
		fprintf(stderr,"Usage: %s [options] volumetricfilename\n",argv[0]);
		fprintf(stderr,"Options\n");
		fprintf(stderr,"    -i n    choose an isosurface value\n");
		exit(-1);
	}

	// Parse the command line
	for (i=1;i<argc;i++) {
		if (strcmp(argv[i],"-i") == 0)
			isolevel = atof(argv[i+1]);
	}

	// Open and read the raw data
	fprintf(stderr,"Reading data ...\n");
	if ((fptr = fopen(argv[argc-1],"rb")) == NULL) {
	    fprintf(stderr,"File open failed\n");
		exit(-1);
	}

    //parsing parameters
    char **list;
    char param[60];
    for (i = 0; i < 10; i++){
        fgets (param, 60, fptr);
        list = splitting(param);
        if (strcmp(list[0],"DIMENSIONS") == 0){
            NX  = atoi(list[1]);
            NY = atoi(list[2]);
            NZ = atoi(list[3]);
        }
    }

    // Malloc the volumetric data, hardwired size!
    data = malloc(NX*sizeof(short int **));
    for (i=0;i<NX;i++)
        data[i] = malloc(NY*sizeof(short int *));
    for (i=0;i<NX;i++)
        for (j=0;j<NY;j++)
            data[i][j] = malloc(NZ*sizeof(short int));

   
	for (k=0;k<NZ;k++) {
		for (j=0;j<NY;j++) {
			for (i=0;i<NX;i++) {
				if ((c = fgetc(fptr)) == EOF) {
					fprintf(stderr,"Unexpected end of file\n");
					exit(-1);
				}
            data[i][j][k] = c;
            if (c > themax)
                themax = c;
            if (c < themin)
                themin = c;
			}
		}
	}
	fclose(fptr);

	fprintf(stderr,"Volumetric data range: %d -> %d\n",themin,themax);

	// Polygonise the grid 
	fprintf(stderr,"Polygonising data ...\n");
	for (i=0;i<NX-1;i++) {
		if (i % (NX/10) == 0)
			fprintf(stderr,"   Slice %d of %d\n",i,NX);
		for (j=0;j<NY-1;j++) {
			for (k=0;k<NZ-1;k++) {
				grid.p[0].x = i;
                grid.p[0].y = j;
                grid.p[0].z = k;
                grid.val[0] = data[i][j][k];
                /*grid_normal = CalcGridNormal(grd,i,j,k);
                grid.n[0].x = grid_normal.x;
                grid.n[0].y = grid_normal.y;
                grid.n[0].z = grid_normal.z;*/

                grid.p[1].x = i+1;
                grid.p[1].y = j;
                grid.p[1].z = k;
                grid.val[1] = data[i+1][j][k];
                /*grid_normal = CalcGridNormal(grd,i+1,j,k);
                grid.n[1].x = grid_normal.x;
                grid.n[1].y = grid_normal.y;
                grid.n[1].z = grid_normal.z;*/
            
                grid.p[2].x = i+1;
                grid.p[2].y = j+1;
                grid.p[2].z = k;
                grid.val[2] = data[i+1][j+1][k];
                /*grid_normal = CalcGridNormal(grd,i+1,j+1,k);
                grid.n[2].x = grid_normal.x;
                grid.n[2].y = grid_normal.y;
                grid.n[2].z = grid_normal.z;*/

                grid.p[3].x = i;
                grid.p[3].y = j+1;
                grid.p[3].z = k;
                grid.val[3] = data[i][j+1][k];
                /*grid_normal = CalcGridNormal(grd,i,j+1,k);
                grid.n[3].x = grid_normal.x;
                grid.n[3].y = grid_normal.y;
                grid.n[3].z = grid_normal.z;*/

                grid.p[4].x = i;
                grid.p[4].y = j;
                grid.p[4].z = k+1;
                grid.val[4] = data[i][j][k+1];
                /*grid_normal = CalcGridNormal(grd,i,j,k+1);
                grid.n[4].x = grid_normal.x;
                grid.n[4].y = grid_normal.y;
                grid.n[4].z = grid_normal.z;*/

                grid.p[5].x = i+1;
                grid.p[5].y = j;
                grid.p[5].z = k+1;
                grid.val[5] = data[i+1][j][k+1];
                /*grid_normal = CalcGridNormal(grd,i+1,j,k+1);
                grid.n[5].x = grid_normal.x;
                grid.n[5].y = grid_normal.y;
                grid.n[5].z = grid_normal.z;*/

                grid.p[6].x = i+1;
                grid.p[6].y = j+1;
                grid.p[6].z = k+1;
                grid.val[6] = data[i+1][j+1][k+1];
                /*grid_normal = CalcGridNormal(grd,i+1,j+1,k+1);
                grid.n[6].x = grid_normal.x;
                grid.n[6].y = grid_normal.y;
                grid.n[6].z = grid_normal.z;*/

                grid.p[7].x = i;
                grid.p[7].y = j+1;
                grid.p[7].z = k+1;
                grid.val[7] = data[i][j+1][k+1];
                /*grid_normal = CalcGridNormal(grd,i,j+1,k+1);
                grid.n[7].x = grid_normal.x;
                grid.n[7].y = grid_normal.y;
                grid.n[7].z = grid_normal.z;*/

				n = PolygoniseCube(grid,isolevel,triangles);
				tri = realloc(tri,(ntri+n)*sizeof(TRIANGLE));
				for (l=0;l<n;l++)
					tri[ntri+l] = triangles[l];
				ntri += n;
			}
		}
	}

    sizecoord = 5000;
    vert = calloc(5000,sizeof(VERTEX));
    for(i=0;i<sizecoord;i++){
        vert[i].flag = 0;
        vert[i].size = 20;
        vert[i].y = NULL;
    }

    // Now do something with the triangles ....
    // Here I just write them to a geom file
    fprintf(stderr,"Writing triangles ...\n");
    if ((fptr = fopen("output2.obj","w")) == NULL) {
        fprintf(stderr,"Failed to open output file\n");
        exit(-1);
    }

    fprintf(stderr, "Calculating normals\n");
    unsigned long int *index;
    int rbool = 0;
    for (i=0;i<ntri;i++) {
        for (k=0;k<3;k++)  {
            fprintf(fptr,"v %f %f %f\n",tri[i].p[k].x,tri[i].p[k].y,tri[i].p[k].z);
            index = updateVert(&vert,tri[i].p[k],i,sizecoord);
            rbool = growVertex(&vert,index);
            if (rbool == 1){
                allocVertex(&vert,2000);
            }
            if (rbool == 2){
                allocVertex(&vert,500);
            }
            if (index[0] == 1){
                rbool = growVertexY(&(vert[index[1]].y), vert[index[1]].flag, vert[index[1]].size);
                if (rbool == 1){
                    allocVertexY(&(vert[index[1]].y),vert[index[1]].size);
                    vert[index[1]].size +=10;
                }
            }
            if(index[0]==2){
                rbool = growVertexZ(&(vert[index[1]].y[index[2]].z),vert[index[1]].y[index[2]].flag,vert[index[1]].y[index[2]].size);
                if (rbool == 1){
                    VERTEXY *verty = *&(vert[index[1]].y);
                    VERTEXZ  *vertz = (verty)[index[2]].z;
                    allocVertexZ(&(vert[index[1]].y[index[2]].z),vert[index[1]].y[index[2]].size);
                    vert[index[1]].y[index[2]].size +=5;
                }
            }
            /*if(index[0]==4){
                rbool = growlist(&((vert[index[1]].y[index[2]].z)[index[3]].tri_index),(vert[index[1]].y[index[2]].z)[index[3].total);
            }*/
            tri[i].indexes[k].x = index[1];
            tri[i].indexes[k].y = index[2];
            tri[i].indexes[k].z = index[3];
        }
	}

    MATIND tri_ind;
    for (i=0;i<ntri;i++) {
        for (k=0;k<3;k++)  {
            tri_ind = tri[i].indexes[k];
            XYZ newnormal = getNormal(tri,&vert,tri_ind);
            fprintf(fptr,"vn %f %f %f\n",newnormal.x,newnormal.y,newnormal.z);
        }
    }
    for (i=0;i<ntri*3;i+=3) {
        fprintf(fptr,"f %d//%d %d//%d %d//%d\n",i+1,i+1,i+2,i+2,i+3,i+3);  
    }
	fclose(fptr);

	exit(0);
}

/*-------------------------------------------------------------------------
   Given a grid cell and an isolevel, calculate the triangular
   facets requied to represent the isosurface through the cell.
   Return the number of triangular facets, the array "triangles"
   will be loaded up with the vertices at most 5 triangular facets.
   0 will be returned if the grid cell is either totally above
   of totally below the isolevel.
*/
int PolygoniseCube(GRIDCELL g,double iso,TRIANGLE *tri)
{
    int i,ntri = 0;
    int cubeindex;
    XYZ vertlist[12];
    XYZ normlist[12];

    /*
      Determine the index into the edge table which
      tells us which vertices are inside of the surface
    */
    cubeindex = 0;
    if (g.val[0] < iso) cubeindex |= 1;
    if (g.val[1] < iso) cubeindex |= 2;
    if (g.val[2] < iso) cubeindex |= 4;
    if (g.val[3] < iso) cubeindex |= 8;
    if (g.val[4] < iso) cubeindex |= 16;
    if (g.val[5] < iso) cubeindex |= 32;
    if (g.val[6] < iso) cubeindex |= 64;
    if (g.val[7] < iso) cubeindex |= 128;

    /* Cube is entirely in/out of the surface */
    if (edgeTable[cubeindex] == 0)
        return(0);

    /* Find the vertices where the surface intersects the cube */
    if (edgeTable[cubeindex] & 1) {
        vertlist[0] = VertexInterp(iso,g.p[0],g.p[1],g.val[0],g.val[1]);
        //normlist[0] = NormalInterp(iso,g.n[0],g.n[1],g.val[0],g.val[1]);
    }
    if (edgeTable[cubeindex] & 2) {
        vertlist[1] = VertexInterp(iso,g.p[1],g.p[2],g.val[1],g.val[2]);
        //normlist[1] = NormalInterp(iso,g.n[1],g.n[2],g.val[1],g.val[2]);
    }
    if (edgeTable[cubeindex] & 4) {
        vertlist[2] = VertexInterp(iso,g.p[2],g.p[3],g.val[2],g.val[3]);
        //normlist[2] = NormalInterp(iso,g.n[2],g.n[3],g.val[2],g.val[3]);
    }
    if (edgeTable[cubeindex] & 8) {
        vertlist[3] = VertexInterp(iso,g.p[3],g.p[0],g.val[3],g.val[0]);
        //normlist[3] = NormalInterp(iso,g.n[3],g.n[0],g.val[3],g.val[0]);
    }
    if (edgeTable[cubeindex] & 16) {
        vertlist[4] = VertexInterp(iso,g.p[4],g.p[5],g.val[4],g.val[5]);
        //normlist[4] = NormalInterp(iso,g.n[4],g.n[5],g.val[4],g.val[5]);
    }
    if (edgeTable[cubeindex] & 32) {
        vertlist[5] = VertexInterp(iso,g.p[5],g.p[6],g.val[5],g.val[6]);
        //normlist[5] = NormalInterp(iso,g.n[5],g.n[6],g.val[5],g.val[6]);
    }
    if (edgeTable[cubeindex] & 64) {
        vertlist[6] = VertexInterp(iso,g.p[6],g.p[7],g.val[6],g.val[7]);
        //normlist[6] = NormalInterp(iso,g.n[6],g.n[7],g.val[6],g.val[7]);
    }
    if (edgeTable[cubeindex] & 128) {
        vertlist[7] = VertexInterp(iso,g.p[7],g.p[4],g.val[7],g.val[4]);
        //normlist[7] = NormalInterp(iso,g.n[7],g.n[4],g.val[7],g.val[4]);
    }
    if (edgeTable[cubeindex] & 256) {
        vertlist[8] = VertexInterp(iso,g.p[0],g.p[4],g.val[0],g.val[4]);
        //normlist[8] = NormalInterp(iso,g.n[0],g.n[4],g.val[0],g.val[4]);
    }
    if (edgeTable[cubeindex] & 512) {
        vertlist[9] = VertexInterp(iso,g.p[1],g.p[5],g.val[1],g.val[5]);
        //normlist[9] = NormalInterp(iso,g.n[1],g.n[5],g.val[1],g.val[5]);
    }
    if (edgeTable[cubeindex] & 1024) {
        vertlist[10] = VertexInterp(iso,g.p[2],g.p[6],g.val[2],g.val[6]);
        //normlist[10] = NormalInterp(iso,g.n[2],g.n[6],g.val[2],g.val[6]);
    }
    if (edgeTable[cubeindex] & 2048) {
        vertlist[11] = VertexInterp(iso,g.p[3],g.p[7],g.val[3],g.val[7]);
        //normlist[11] = NormalInterp(iso,g.n[3],g.n[7],g.val[3],g.val[7]);
    }

   /* Create the triangles */
    for (i=0;triTable[cubeindex][i]!=-1;i+=3) {
        tri[ntri].p[0] = vertlist[triTable[cubeindex][i  ]];
        tri[ntri].p[1] = vertlist[triTable[cubeindex][i+1]];
        tri[ntri].p[2] = vertlist[triTable[cubeindex][i+2]];
        XYZ normal = CalcNormal(tri[ntri].p[0],tri[ntri].p[1],tri[ntri].p[2]);
        tri[ntri].n = normal;
        ntri++;
    }

    return(ntri);
}

/*-------------------------------------------------------------------------
   Normalise a vector
*/
void Normalise(XYZ *p)
{
    double length;

    length = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
    if (length != 0) {
        p->x /= length;
        p->y /= length;
        p->z /= length;
    } else {
        p->x = 0;
        p->y = 0;
        p->z = 0;
    }  
}

/*-------------------------------------------------------------------------
   Calculate the unit normal at p given two other points 
   p1,p2 on the surface. The normal points in the direction 
   of p1 crossproduct p2
*/
XYZ CalcNormal(XYZ p,XYZ p1,XYZ p2)
{
    XYZ n,pa,pb;

    pa.x = p1.x - p.x;
    pa.y = p1.y - p.y;
    pa.z = p1.z - p.z;
    pb.x = p2.x - p.x;
    pb.y = p2.y - p.y;
    pb.z = p2.z - p.z;
    Normalise(&pa);
    Normalise(&pb);
    
    n.x = pa.y * pb.z - pa.z * pb.y;
    n.y = pa.z * pb.x - pa.x * pb.z;
    n.z = pa.x * pb.y - pa.y * pb.x;
    Normalise(&n);

    return(n);
}

/*-------------------------------------------------------------------------
   Return the point between two points in the same ratio as
   isolevel is between valp1 and valp2
*/
XYZ VertexInterp(double isolevel,XYZ p1,XYZ p2,double valp1,double valp2)
{
    double mu;
    XYZ p;

    if (ABS(isolevel-valp1) < 0.00001)
        return(p1);
    if (ABS(isolevel-valp2) < 0.00001)
        return(p2);
    if (ABS(valp1-valp2) < 0.00001)
        return(p1);
    mu = (isolevel - valp1) / (valp2 - valp1);
    p.x = p1.x + mu * (p2.x - p1.x);
    p.y = p1.y + mu * (p2.y - p1.y);
    p.z = p1.z + mu * (p2.z - p1.z);

    return(p);
}

/*-------------------------------------------------------------------------
   calculate an average value from 
      the normals on the single points
*/
XYZ NormalInterp(double isolevel,XYZ n1,XYZ n2,double valp1,double valp2)
{
    double mu;
    XYZ n;

    if (ABS(isolevel-valp1) < 0.00001)
        return(n1);
    if (ABS(isolevel-valp2) < 0.00001)
        return(n2);
    if (ABS(valp1-valp2) < 0.00001)
        return(n1);
    mu = (isolevel - valp1) / (valp2 - valp1);
    n.x = n1.x + mu * (n2.x - n1.x);
    n.y = n1.y + mu * (n2.y - n1.y);
    n.z = n1.z + mu * (n2.z - n1.z);
    Normalise(&n);
    return(n);
}

/*
   Estimate the normal for each point on the grid
*/
XYZ CalcGridNormal(int grid[],int i,int j,int k)
{
    int im1,ip1,jp1,jm1,kp1,km1;
    int index1,index2,index;
    long NxNy;
    XYZ n = {0.0,0.0,0.0};

    im1 = i - 1;
    ip1 = i + 1;
    jm1 = j - 1;
    jp1 = j + 1;
    km1 = k - 1;
    kp1 = k + 1;

    NxNy = NX * NY;
    index = k*NxNy + j*NY + i;

    if (im1 > 0 && ip1 < NX) {
        index1 = index - 1;
        index2 = index + 1;
        n.x = (grid[index1] - grid[index2]) / 2.0;
    } else if (im1 > 0) {
        index1 = index - 1;
        n.x = grid[index1] - grid[index];
    } else {
        index2 = index + 1;
        n.x = grid[index] - grid[index2];
    }

    if (jm1 > 0 && jp1 < NY) {
        index1 = index - NY;
        index2 = index + NY;
        n.y = (grid[index1] - grid[index2]) / 2.0;
    } else if (jm1 > 0) {
        index1 = index - NY;
        n.y = grid[index1] - grid[index];
    } else {
        index2 = index + NY;
        n.y = grid[index] - grid[index2];
    }

    if (km1 > 0 && kp1 < NZ) {
        index1 = index - NxNy;
        index2 = index + NxNy;
        n.z = (grid[index1] - grid[index2]) / 2.0;
    } else if (km1 > 0) {
        index1 = index - NxNy;
        n.z = grid[index1] - grid[index];
    } else {
        index2 = index + NxNy;
        n.z = grid[index] - grid[index2];
    }

    Normalise(&n);
    return(n);
}

int growVertexZ(VERTEXZ **vertz, unsigned long int flag, unsigned long int size){
    VERTEXZ *vertaux;
    if(flag == size - 5){
        vertaux = realloc(*vertz,(size+5)*sizeof(VERTEXZ));
        if (vertaux == NULL){
            fprintf(stderr,"Failed to realloc 3 ...\n");
            return 0;
        }
        *vertz = vertaux;
        return 1;
    }
    return 0;
}

int growVertexY(VERTEXY **verty, unsigned long int flag, unsigned long int size){
    VERTEXY *vertaux;
    VERTEXZ *vertz;
    if (flag == size-10){
        vertaux = realloc(*verty, (size+10)*sizeof(VERTEXY));
        if (vertaux == NULL){
            fprintf(stderr,"Failed to realloc 2...\n");
            return 0;
        }
        *verty = vertaux;
        return 1;
    }
    return 0;
}

int growVertex(VERTEX **vert, unsigned long int *index){
    VERTEX *vertaux;
    VERTEXY *verty;
    VERTEXZ *vertz;
    if (totalcoord == sizecoord -2000 || totalcoord == sizecoord - 500){
        vertaux = realloc(*vert, (sizecoord+2000)*sizeof(VERTEX));
        if (vertaux == NULL){
            fprintf(stderr,"Failed to realloc 1...\n");
            vertaux = realloc(*vert, (sizecoord+500)*sizeof(VERTEX));
            *vert = vertaux;
            sizecoord += 500;
            return 2;
        }
        else{
            *vert = vertaux;
            sizecoord += 2000;
        }
        return 1;
    }
    return 0;
}


/*init allocated space*/
void allocVertex(VERTEX **vert, int total){
    VERTEX *vertaux = *vert;
    VERTEXY *verty;
    VERTEXZ *vertz;
    unsigned long int i,j,k=0;
    for (i=sizecoord-total; i<sizecoord; i++){
        vertaux[i].flag = 0;
        vertaux[i].size = 20;
        vertaux[i].y = NULL;
    }
}


void allocVertexY (VERTEXY **verty, unsigned long int size){
    VERTEXZ *vertz;
    VERTEXY *vertaux = *verty;
    unsigned long int j,k=0;
    for (j=size; j<size+10; j++){
        vertaux[j].flag = 0;
        vertaux[j].size = 20;
        vertaux[j].z = NULL;
    }
}

void allocVertexZ (VERTEXZ **vertz, unsigned long int size){
    VERTEXZ *vertaux;
    unsigned long int j,k=0;
    vertaux = *vertz;
    for (j = size; j<size+5; j++){
        vertaux[j].total = 0;
    }
}

/* Update list of vertexes */
unsigned long int *updateVert(VERTEX **vert, XYZ p1, unsigned int index, unsigned long int vertex_size){
    unsigned long int i=1;
    unsigned long int j=1;
    unsigned long int k=1;
    unsigned long int auxk,auxi,auxj = 0;
    static unsigned long int ind [4];
    VERTEXY *verty;
    VERTEXZ *vertz;
    VERTEX *vertaux = *vert;
    int flag = -1;
    for (i=0;i<sizecoord && flag == -1;i++){
        if (vertaux[i].flag==0){
            vertaux[i].flag = 1;
            vertaux[i].x = p1.x;

            vertaux[i].y = calloc(20,sizeof(VERTEXY));
            verty = vertaux[i].y;
            vertaux[i].size = 20;
            for (auxi = 0;auxi<20;auxi++){
                verty[auxi].flag = 0;
            }

            verty[0].flag = 1;
            verty[0].y = p1.y;

            vertaux[i].y[0].z = calloc(10,sizeof(VERTEXZ));
            for (auxj = 0;auxj<10;auxj++){
                (vertaux[i].y[0].z)[auxj].total = 0;
                //(vertaux[i].y[0].z)[auxj].tri_index = calloc(20,sizeof(unsigned long int));
            }

            vertz = vertaux[i].y[0].z;
            verty[0].size = 10;

            vertz[0].z = p1.z;
            vertz[0].total=1;
            vertz[0].tri_index[0]=index; 
            flag = 0;
            totalcoord ++;
        }
        else{
            if(vertaux[i].x == p1.x){
                verty = vertaux[i].y;
                for (j=0;j<vertaux[i].size && flag == -1;j++){
                    if(verty[j].flag==0){
                        vertaux[i].flag++;
                        verty[j].flag = 1;
                        verty[j].y = p1.y;

                        (vertaux[i].y)[j].z = calloc(10,sizeof(VERTEXZ));
                        for (auxk=0;auxk<10;auxk++){
                            ((vertaux[i].y)[j].z)[auxk].total=0;
                            //((vertaux[i].y)[j].z)[auxk].tri_index = calloc(20,sizeof(unsigned long int));
                        }
                        vertz = (vertaux[i].y)[j].z;
                        verty[j].size = 10;

                        ((vertaux[i].y)[j].z)[0].total = 1;
                        ((vertaux[i].y)[j].z)[0].z = p1.z;
                        ((vertaux[i].y)[j].z)[0].tri_index[0]=index;
                        flag=1;
                    }
                    else{
                        if(verty[j].y == p1.y){
                            vertz = verty[j].z;
                            for (k=0;k<(vertaux[i].y)[j].size && flag == -1;k++){
                                if(vertz[k].total == 0){
                                    ((vertaux[i].y)[j].z)[k].z = p1.z;
                                    ((vertaux[i].y)[j].z)[k].total =1;
                                    //vertz[k].tri_index[0] = 2;
                                    ((vertaux[i].y)[j].z)[k].tri_index[0]=index;
                                    verty[j].flag ++;
                                    flag = 2;
                                }
                                else{
                                    if(vertz[k].z == p1.z){
                                        if (vertz[k].total<10){
                                            ((vertaux[i].y)[j].z)[k].tri_index[vertz[k].total] = index;
                                            vertz[k].total ++;
                                        }
                                        flag=4;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    //fprintf(stderr, "INDEXES %d %d %d\n", INDEX(i),INDEX(j),INDEX(k));

    ind[0] = flag;
    ind[1] = INDEX(i);
    ind[2] = INDEX(j);
    ind[3] = INDEX(k);
    return ind;
}

/*Compare two vertexes*/
int vertEqual(XYZ p1, XYZ p2){
    if (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z){
        return 0;
    }
    return 1;
}

/* Get Vertex Normal*/
XYZ getNormal(TRIANGLE *tri, VERTEX **vert, MATIND index){
    VERTEX *vertaux = *vert;
    XYZ newnormal={0.0,0.0,0.0};
    int *list;
    unsigned long int tot;
    unsigned long int i,j,k;
    /*i = index.x;
    j = index.y;
    k = index.z;*/
    int x = vertaux[index.x].x;
    int y = (vertaux[index.x].y)[index.y].y;
    int total = ((vertaux[index.x].y)[index.y]).z[index.z].total;
    list = ((vertaux[index.x].y)[index.y]).z[index.z].tri_index;

    for(i=0;i<total;i++){
        k = list[i];
        newnormal.x += tri[k].n.x;
        newnormal.y += tri[k].n.y;
        newnormal.z += tri[k].n.z;
    }
    newnormal.x = newnormal.x / tot;
    newnormal.y = newnormal.y / tot;
    newnormal.z = newnormal.z / tot;
    return newnormal;
}

char **splitting(char *str){
    char *pch;
    pch = strtok (str," ");
    char **list = malloc(4*sizeof(char *));
    for(int i=0; i<4; i++){
        list[i] = malloc(20*sizeof(char));
    }
    int index = 0;
    while (pch != NULL){
        list[index] = pch;
        pch = strtok (NULL, " ");
        index ++;
    }
    return list;
}