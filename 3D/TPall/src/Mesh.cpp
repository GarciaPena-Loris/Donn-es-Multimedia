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

#include "Mesh.h"
#include "Vec3D.h"
#include <algorithm>

using namespace std;

void Mesh::clear () {
    clearTopology ();
    clearGeometry ();
}

void Mesh::clearGeometry () {
    vertices.clear ();
}

void Mesh::clearTopology () {
    triangles.clear ();
}

void Mesh::computeTriangleNormals (vector<Vec3> & triangleNormals) {
    for (vector<Triangle>::const_iterator it = triangles.begin ();
         it != triangles.end ();
         it++) {
        Vec3 e01 (vertices[it->getVertex (1)].getPosition ()
                - vertices[it->getVertex (0)].getPosition ());
        Vec3 e02 (vertices[it->getVertex (2)].getPosition ()
                - vertices[it->getVertex (0)].getPosition ());
        Vec3 n (Vec3::crossProduct (e01, e02));
        n.normalize ();
        triangleNormals.push_back (n);
    }
}

void Mesh::recomputeSmoothVertexNormals (unsigned int normWeight) {
    vector<Vec3> triangleNormals;
    computeTriangleNormals (triangleNormals);
    for (vector<Vertex>::iterator it = vertices.begin ();
         it != vertices.end ();
         it++)
        it->setNormal (Vec3 (0.0, 0.0, 0.0));
    vector<Vec3>::iterator itNormal = triangleNormals.begin ();
    vector<Triangle>::iterator it = triangles.begin ();
    for ( ; it != triangles.end (); it++, itNormal++)
        for (unsigned int  j = 0; j < 3; j++) {
            Vertex & vj = vertices[it->getVertex (j)];
            float w = 1.0; // uniform weights
            Vec3 e0 = vertices[it->getVertex ((j+1)%3)].getPosition ()
                    - vj.getPosition ();
            Vec3 e1 = vertices[it->getVertex ((j+2)%3)].getPosition ()
                    - vj.getPosition ();
            if (normWeight == 1) { // area weight
                w = Vec3::crossProduct (e0, e1).getLength () / 2.0;
            } else if (normWeight == 2) { // angle weight
                e0.normalize ();
                e1.normalize ();
                w = (2.0 - (Vec3::dotProduct (e0, e1) + 1.0)) / 2.0;
            }
            if (w <= 0.0)
                continue;
            vj.setNormal (vj.getNormal () + (*itNormal) * w);
        }
    Vertex::normalizeNormals (vertices);
}

//one-ring of each vertex, i.e. a list of vertices with which it shares an edge
void Mesh::collectOneRing (vector<vector<unsigned int> > & oneRing) const {
    //Initialisation le vecetur de o_one_ring de la taille du vecteur vertices
    oneRing.resize (vertices.size ());
    //Parcours les triangles et ajout les voisins dans le 1-voisinage
    //Tous les points opposés dans le triangle sont reliés
    for (unsigned int i = 0; i < triangles.size (); i++) {
        const Triangle & ti = triangles[i];
        for (unsigned int j = 0; j < 3; j++) {
            unsigned int vj = ti.getVertex (j);
            for (unsigned int k = 1; k < 3; k++) {
                unsigned int vk = ti.getVertex ((j+k)%3);
                if (find (oneRing[vj].begin (), oneRing[vj].end (), vk) == oneRing[vj].end ())
                    oneRing[vj].push_back (vk);
            }
        }
    }
}

// #################################################
// ################################################# Modification
// #################################################

void Mesh::smooth( float lambda ){
    
    vector<vector<unsigned int>> ring;
    collectOneRing(ring);

    vector<Vec3> new_positions;
    new_positions.resize(vertices.size());

    for (unsigned int i = 0; i < vertices.size(); ++i) {
        new_positions[i] = Vec3(0, 0, 0);

        for (unsigned int j = 0; j < ring[i].size(); ++j) {
            new_positions[i] += vertices[ring[i][j]].position;
        }

        new_positions[i] /= ring[i].size();
        new_positions[i] -= vertices[i].position;
    }

    for (unsigned int i = 0; i < vertices.size(); ++i) {
        vertices[i].position += lambda * new_positions[i];
    }
}

void Mesh::swell(float factor) {

    // Calcule les normales des sommets
    unsigned int normal;
    recomputeSmoothVertexNormals(normal);

    for (unsigned int i = 0; i < vertices.size(); ++i) {
        // Ajoute la normale multipliée par le facteur à la position du sommet
        vertices[i].position += factor * vertices[i].normal;
    }
}
void Mesh::scale(int Axe, float factor)
{
    for (long unsigned int i = 0; i < vertices.size(); ++i)
    {
        vertices[i].position[Axe] *= factor;
    }
}

void Mesh::smoothTaubin(float lambda, float mu, int iterations)
{
    for(int i = 0; i < iterations; ++i) {
        smooth(lambda);
        smooth(-mu);
    }
}

void Mesh::addNoise(){
    for( unsigned int i = 0 ; i < vertices.size() ; i ++ )
    {
        float factor = 0.03;
        const Vec3 & p = vertices[i].position;
        const Vec3 & n = vertices[i].normal;
        vertices[i].position = Vec3( p[0] + factor*((double)(rand()) / (double)(RAND_MAX))*n[0],
                p[1] + factor*((double)(rand()) / (double)(RAND_MAX))*n[1],
                p[2] + factor*((double)(rand()) / (double)(RAND_MAX))*n[2]);

    }
}

// #################################################
// #################################################
// #################################################


void Mesh::setUnitSphere(int nX, int nY)
{
    vertices.clear();
    triangles.clear();

    // Vertices
    for (int i = 0; i <= nX; i++)
    {
        float u = 2 * M_PI * i / nX;
        for (int j = 0; j <= nY; j++)
        {
            float v = M_PI * j / nY;
            float x = sin(v) * cos(u);
            float y = sin(v) * sin(u);
            float z = cos(v);
            vertices.push_back(Vec3(x, y, z));
        }
    }

    // Triangles
    for (int i = 0; i < nX; i++)
    {
        int index = i * (nY + 1);
        for (int j = 0; j < nY; j++)
        {
            int a = index + j;
            int b = a + 1;
            int c = a + (nY + 1);
            int d = c + 1;
            triangles.push_back(Triangle(a, b, c));
            triangles.push_back(Triangle(b, d, c));
        }
    }

    // Normales
    recomputeSmoothVertexNormals(1);
}

// Bonus
void Mesh::setUnitCube(int nX, int nY)
{
    vertices.clear();
    triangles.clear();

    // Calcul du pas entre deux vertices sur chaque axe
    float stepX = 1.0f / nX;
    float stepY = 1.0f / nY;

    // Création des vertices
    for (int k = 0; k < 6; k++) // Pour chaque face
    {
        for (int i = 0; i <= nX; i++) // Pour chaque colonne
        {
            for (int j = 0; j <= nY; j++) // Pour chaque ligne
            {
                float x, y, z;

                // Détermination de la position du vertex en fonction de la face
                switch (k)
                {
                case 0: // face arrière
                    x = -0.5f + i * stepX;
                    y = -0.5f + j * stepY;
                    z = -0.5f;
                    break;
                case 1: // face avant
                    x = -0.5f + i * stepX;
                    y = -0.5f + j * stepY;
                    z = 0.5f;
                    break;
                case 2: // face gauche
                    x = -0.5f;
                    y = -0.5f + i * stepY;
                    z = -0.5f + j * stepY;
                    break;
                case 3: // face droite
                    x = 0.5f;
                    y = -0.5f + i * stepY;
                    z = -0.5f + j * stepY;
                    break;
                case 4: // face basse
                    x = -0.5f + i * stepX;
                    y = -0.5f;
                    z = -0.5f + j * stepY;
                    break;
                case 5: // face haute
                    x = -0.5f + i * stepX;
                    y = 0.5f;
                    z = -0.5f + j * stepY;
                    break;
                }

                vertices.push_back(Vec3(x, y, z)); // Ajout du vertex à la liste
            }
        }
    }

    // Triangles
    for (int i = 0; i < 6; i++)
    {
        int index = i * (nX + 1) * (nY + 1);
        for (int j = 0; j < nX; j++)
        {
            for (int k = 0; k < nY; k++)
            {
                int a = index + j * (nY + 1) + k;
                int b = a + 1;
                int c = a + (nY + 1);
                int d = c + 1;

                if (i == 0 || i == 2 || i == 5) // face à inverser
                {
                    triangles.push_back(Triangle(a, b, c));
                    triangles.push_back(Triangle(b, d, c));
                }
                else // autres faces
                {
                    triangles.push_back(Triangle(a, c, b));
                    triangles.push_back(Triangle(b, c, d));
                }
            }
        }
    }

    // Normales
    recomputeSmoothVertexNormals(1);
}

void Mesh::setUnitTorus(float R, float r, int nX, int nY)
{
    vertices.clear();
    triangles.clear();

    // Vertices
    for (int i = 0; i <= nX; i++)
    {
        float u = 2 * M_PI * i / nX;
        for (int j = 0; j <= nY; j++)
        {
            float v = 2 * M_PI * j / nY;
            float x = (R + r * cos(v)) * cos(u);
            float y = (R + r * cos(v)) * sin(u);
            float z = r * sin(v);
            vertices.push_back(Vec3(x, y, z));
        }
    }

    // Triangles
    for (int i = 0; i < nX; i++)
    {
        int index = i * (nY + 1);
        for (int j = 0; j < nY; j++)
        {
            int a = index + j;
            int b = a + 1;
            int c = a + (nY + 1);
            int d = c + 1;
            triangles.push_back(Triangle(a, c, b));
            triangles.push_back(Triangle(b, c, d));
        }
    }

    // Normales
    recomputeSmoothVertexNormals(1);
}

void Mesh::setUnitEmptyCylinder(float radius, float height, int nX, int nY) // Cylindre creux
{
    vertices.clear();
    triangles.clear();

    // Calcul de la distance entre deux points horizontaux
    float angle = 2 * M_PI / nX;

    // Vertices
    // Points cotés
    for (int i = 0; i <= nX; i++)
    {
        float x = radius * cos(angle * i);
        float z = radius * sin(angle * i);
        for (int j = 0; j <= nY; j++)
        {
            float y = height * j / nY - height / 2;
            vertices.push_back(Vec3(x, y, z));
        }
    }

    // Triangles
    for (int i = 0; i < nX ; i++)
    {
        for (int j = 0; j < nY; j++)
        {
            int a = i * (nY + 1) + j;
            int b = a + 1;
            int c = a + (nY + 1);
            int d = c + 1;
            triangles.push_back(Triangle(a, b, c));
            triangles.push_back(Triangle(b, d, c));
        }
    }
    

    // Normales
    recomputeSmoothVertexNormals(1);
}

void Mesh::setUnitCylinder(float radius, float height, int nX, int nY) {
    vertices.clear();
    triangles.clear();

    float half_height = height / 2;

    // Vertices
    for (int i = 0; i <= nX; i++) {
        float u = 2 * M_PI * i / nX;
        for (int j = 0; j <= nY; j++) {
            float v = height * j / nY - half_height;
            float x = radius * cos(u);
            float y = radius * sin(u);
            float z = v;
            vertices.push_back(Vec3(x, y, z));
        }
    }

    // Pole Nord
    int top_vertex_index = vertices.size();
    vertices.push_back(Vec3(0, 0, half_height));

    // Pole Sud
    int bottom_vertex_index = vertices.size();
    vertices.push_back(Vec3(0, 0, -half_height));

    // Triangles
    for (int i = 0; i < nX; i++) {
        int index = i * (nY + 1);
        for (int j = 0; j < nY; j++) {
            int a = index + j;
            int b = a + 1;
            int c = a + (nY + 1);
            int d = c + 1;
            triangles.push_back(Triangle(a, c, b));
            triangles.push_back(Triangle(b, c, d));
        }
    }
    // Triangles Pole Nord
    for (int i = 0; i < nX; i++) {
        int a = bottom_vertex_index;
        int b = i * (nY + 1);
        int c = (i + 1) * (nY + 1);
        triangles.push_back(Triangle(a, c, b));
    }
    // Triangle Pole Sud
    for (int i = 0; i < nX; i++) {
        int a = top_vertex_index;
        int b = (nX - i - 1) * (nY + 1) + nY;
        int c = (nX - i) * (nY + 1) + nY;
        triangles.push_back(Triangle(a, b, c));
    }

    // Normales
    recomputeSmoothVertexNormals(1);
}

void Mesh::setUnitCone(float radius, float height, int nX, int nY)
{
    vertices.clear();
    triangles.clear();

    // Vertices
    for (int i = 0; i <= nX; i++)
    {
        float u = 2 * M_PI * i / nX;
        for (int j = 0; j <= nY; j++)
        {
            float v = j * height / nY;
            float x = radius * (1 - v/height) * cos(u);
            float y = radius * (1 - v/height) * sin(u);
            float z = -v/2;
            vertices.push_back(Vec3(x, y, z));
        }
    }

    // Triangles
    // Sous le cone
    for (int i = 1; i <= nX; i++)
    {
        int a = 0;
        int b = i * (nY + 1);
        int c = (i % nX + 1) * (nY + 1);
        triangles.push_back(Triangle(a, b, c));
    }

    // Sur le cone
    for (int i = 0; i < nX; i++)
    {
        for (int j = 0; j < nY; j++)
        {
            int a = i * (nY + 1) + j;
            int b = a + 1;
            int c = (i + 1) * (nY + 1) + j;
            int d = c + 1;
            triangles.push_back(Triangle(a, b, c));
            triangles.push_back(Triangle(b, d, c));
        }
    }

    // Normales
    recomputeSmoothVertexNormals(1);
}

void Mesh::setUnitCylinderCone(float radius, float height, int nX, int nY)
{
    vertices.clear();
    triangles.clear();

    // Cylinder
    this->setUnitCylinder(radius, height, nX, nY);

    // Cone
    Mesh cone;
    cone.setUnitCone(radius, height, nX, nY);

    // Translate à la fin du cylinder
    for (unsigned int i = 0; i < cone.vertices.size(); i++)
    {
        float posz = cone.vertices[i].getPosition()[2];
        posz -= height/2;
        Vec3 pos = Vec3(
            cone.vertices[i].getPosition()[0],
            cone.vertices[i].getPosition()[1],
            posz);
        cone.vertices[i].setPosition(pos);
    }

    // Combine vertices, normal et triangles
    int cylinderVertexCount = vertices.size();
    for (unsigned int i = 0; i < cone.vertices.size(); i++)
    {
        vertices.push_back(cone.vertices[i]);
    }

    for (unsigned int i = 0; i < cone.triangles.size(); i++)
    {
        Triangle triangle = cone.triangles[i];
        triangles.push_back(Triangle(
            triangle.getVertex(0) + cylinderVertexCount, 
            triangle.getVertex(1) + cylinderVertexCount, 
            triangle.getVertex(2) + cylinderVertexCount));
    }

     // Normales
    recomputeSmoothVertexNormals(1);
}

float supershape(float phi, float a, float b, float m, float n1, float n2, float n3)
{
    float t1 = pow(abs((1 / a) * cos(m * phi / 4)), n2);
    float t2 = pow(abs((1 / b) * sin(m * phi / 4)), n3);
    float t3 = pow(t1 + t2, 1 / n1);
    return 1 / t3;
}

void Mesh::setUnitSupershape(int nX, int nY, float a, float b, float m, float n1, float n2, float n3)
{
    vertices.clear();
    triangles.clear();

    // Vertices
    for (int i = 0; i <= nX; i++)
    {
        float phi = i * ((M_PI * 2) / nX);
        for (int j = 0; j <= nY; j++)
        {
            float theta = j * (M_PI / nY) - (M_PI / 2);
            float r = supershape(phi, a, b, m, n1, n2, n3);
            float x = r * cos(theta) * cos(phi);
            float y = r * cos(theta) * sin(phi);
            float z = r * sin(theta);
            vertices.push_back(Vec3(x, y, z));
        }
    }

    // Triangles
    for (int i = 0; i < nX; i++)
    {
        int index = i * (nY + 1);
        for (int j = 0; j < nY; j++)
        {
            int a = index + j;
            int b = a + 1;
            int c = a + (nY + 1);
            int d = c + 1;
            triangles.push_back(Triangle(a, c, b));
            triangles.push_back(Triangle(b, c, d));
        }
    }

    // Normales
    recomputeSmoothVertexNormals(1);
}

void Mesh::setUnitDodecahedron(int nX, int nY)
{
    vertices.clear();
    triangles.clear();

    vertices.clear();
    triangles.clear();

    float phi = (1 + sqrt(5)) / 2; // nombre d'or

    // Vertices
    vertices.push_back(Vec3(-1, -1, -1));
    vertices.push_back(Vec3(-1, -1, 1));
    vertices.push_back(Vec3(-1, 1, -1));
    vertices.push_back(Vec3(-1, 1, 1));
    vertices.push_back(Vec3(1, -1, -1));
    vertices.push_back(Vec3(1, -1, 1));
    vertices.push_back(Vec3(1, 1, -1));
    vertices.push_back(Vec3(1, 1, 1));
    vertices.push_back(Vec3(0, -1 / phi, -phi));
    vertices.push_back(Vec3(0, -1 / phi, phi));
    vertices.push_back(Vec3(0, 1 / phi, -phi));
    vertices.push_back(Vec3(0, 1 / phi, phi));
    vertices.push_back(Vec3(-phi, 0, -1 / phi));
    vertices.push_back(Vec3(-phi, 0, 1 / phi));
    vertices.push_back(Vec3(phi, 0, -1 / phi));
    vertices.push_back(Vec3(phi, 0, 1 / phi));
    vertices.push_back(Vec3(-1 / phi, -phi, 0));
    vertices.push_back(Vec3(-1 / phi, phi, 0));
    vertices.push_back(Vec3(1 / phi, -phi, 0));
    vertices.push_back(Vec3(1 / phi, phi, 0));

    // Triangles
    
    
    // Normales
    recomputeSmoothVertexNormals(1);
}


