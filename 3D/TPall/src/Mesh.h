// --------------------------------------------------------------------------
// gMini,
// a minimal Glut/OpenGL app to extend                              
//
// Copyright(C) 2007-2009                
// Tamy Boubekeur
//                                                                            
// All rights reserved.                                                       
//                                                                            
// This program is free software; you can redistribute it and/or modify       
// it under the terms of the GNU General Public License as published by       
// the Free Software Foundation; either version 2 of the License, or          
// (at your option) any later version.                                        
//                                                                            
// This program is distributed in the hope that it will be useful,            
// but WITHOUT ANY WARRANTY; without even the implied warranty of             
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              
// GNU General Public License (http://www.gnu.org/licenses/gpl.txt)           
// for more details.                                                          
//                                                                          
// --------------------------------------------------------------------------

#pragma once

#include <vector>

#include "Vertex.h"
#include "Triangle.h"
#include "Edge.h"

class Mesh {
public:
    inline Mesh () {} 
    inline Mesh (const std::vector<Vertex> & v) : vertices (v) {}
    inline Mesh (const std::vector<Vertex> & v, 
                 const std::vector<Triangle> & t) : vertices (v), triangles (t)  {}
    inline Mesh (const Mesh & mesh) : vertices (mesh.vertices), triangles (mesh.triangles) {}
    inline virtual ~Mesh () {}
    std::vector<Vertex> & getVertices () { return vertices; }
    const std::vector<Vertex> & getVertices () const { return vertices; }
    std::vector<Triangle> & getTriangles () { return triangles; }
    const std::vector<Triangle> & getTriangles () const { return triangles; }
    void clear ();
    void clearGeometry ();
    void clearTopology ();
    void recomputeSmoothVertexNormals (unsigned int weight);
    void computeTriangleNormals (std::vector<Vec3> & triangleNormals);
    void collectOneRing (std::vector<std::vector<unsigned int> > & oneRing) const;

    // ############################
    void smooth( float lambda);
    void swell(float factor);
    void scale(int Axe, float factor);
    void smoothTaubin(float lambda, float mu, int iterations);
    void addNoise();
    // ############################

    void setUnitSphere(int nX, int nY);
    void setUnitCube(int nX, int nY);
    void setUnitTorus(float R, float r, int nX, int nY);
    void setUnitEmptyCylinder(float R, float r, int nX, int nY);
    void setUnitCylinder(float R, float r, int nX, int nY);
    void setUnitCone(float radius, float height, int nX, int nY);
    void setUnitCylinderCone(float radius, float height, int nX, int nY);
    void setUnitSupershape(int nX, int nY, float a, float b, float m, float n1, float n2, float n3);
    void setUnitDodecahedron(int nX, int nY);
    
private:
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
};

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
