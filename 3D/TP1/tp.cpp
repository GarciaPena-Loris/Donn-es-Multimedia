// -------------------------------------------
// gMini : a minimal OpenGL/GLUT application
// for 3D graphics.
// Copyright (C) 2006-2008 Tamy Boubekeur
// All rights reserved.
// -------------------------------------------

// -------------------------------------------
// Disclaimer: this code is dirty in the
// meaning that there is no attention paid to
// proper class attribute access, memory
// management or optimisation of any kind. It
// is designed for quick-and-dirty testing
// purpose.
// -------------------------------------------

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <GL/glut.h>
#include <float.h>
#include "src/Vec3.h"
#include "src/Camera.h"

int valfX = 10;
int valfY = 10;
float valfR = 0.7;
float valfr = 0.3;
float valRadius = 0.5;
float valHeight = 1;
float valm = 6.0;
float vala = 1.0; // Supershape
float valb = 1.0; // Supershape
float valn1 = 1.0; // Supershape
float valn2 = 1.0; // Supershape
float valn3 = 1.0; // Supershape

enum DisplayMode
{
    WIRE = 0,
    SOLID = 1,
    LIGHTED_WIRE = 2,
    LIGHTED = 3
};

struct Triangle
{
    inline Triangle()
    {
        v[0] = v[1] = v[2] = 0;
    }
    inline Triangle(const Triangle &t)
    {
        v[0] = t.v[0];
        v[1] = t.v[1];
        v[2] = t.v[2];
    }
    inline Triangle(unsigned int v0, unsigned int v1, unsigned int v2)
    {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
    }
    unsigned int &operator[](unsigned int iv) { return v[iv]; }
    unsigned int operator[](unsigned int iv) const { return v[iv]; }
    inline virtual ~Triangle() {}
    inline Triangle &operator=(const Triangle &t)
    {
        v[0] = t.v[0];
        v[1] = t.v[1];
        v[2] = t.v[2];
        return (*this);
    }
    // membres :
    unsigned int v[3];
};

struct Mesh
{
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Triangle> triangles;
};

Mesh mesh;

// Mesh to generate
Mesh unit_sphere;
Mesh unit_cube;
Mesh unit_torus;
Mesh unit_cylinder;
Mesh unit_cone;
Mesh unit_cylinder_cone;
Mesh unit_supershape;

bool display_normals;
bool display_loaded_mesh;
bool display_unit_sphere;
bool display_unit_cube;
bool display_unit_torus;
bool display_unit_cylinder;
bool display_unit_cone;
bool display_unit_cylinder_cone;
bool display_unit_supershape;
DisplayMode displayMode;

// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;
static unsigned int SCREENWIDTH = 1600;
static unsigned int SCREENHEIGHT = 900;
static Camera camera;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX = 0, lastY = 0, lastZoom = 0;
static bool fullScreen = false;

// To complete
void setUnitSphere(Mesh &o_mesh, int nX, int nY)
{
    o_mesh.vertices.clear();
    o_mesh.normals.clear();
    o_mesh.triangles.clear();

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
            o_mesh.vertices.push_back(Vec3(x, y, z));
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
            o_mesh.triangles.push_back(Triangle(a, b, c));
            o_mesh.triangles.push_back(Triangle(b, d, c));
        }
    }

    // Normales
    for (const Vec3 &v : o_mesh.vertices)
    {
        o_mesh.normals.push_back(v / 1);
    }
}


// Bonus
void setUnitCube(Mesh &o_mesh, int nX, int nY)
{
    o_mesh.vertices.clear();
    o_mesh.normals.clear();
    o_mesh.triangles.clear();

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

                o_mesh.vertices.push_back(Vec3(x, y, z)); // Ajout du vertex à la liste
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
                    o_mesh.triangles.push_back(Triangle(a, b, c));
                    o_mesh.triangles.push_back(Triangle(b, d, c));
                }
                else // autres faces
                {
                    o_mesh.triangles.push_back(Triangle(a, c, b));
                    o_mesh.triangles.push_back(Triangle(b, c, d));
                }
            }
        }
    }

    // Normales
    for (const Vec3 &v : o_mesh.vertices)
    {
        o_mesh.normals.push_back(v / 1);
    }
}

void setUnitTorus(Mesh &o_mesh, float R, float r, int nX, int nY)
{
    o_mesh.vertices.clear();
    o_mesh.normals.clear();
    o_mesh.triangles.clear();

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
            o_mesh.vertices.push_back(Vec3(x, y, z));
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
            o_mesh.triangles.push_back(Triangle(a, c, b));
            o_mesh.triangles.push_back(Triangle(b, c, d));
        }
    }

    // Normales
    for (const Vec3 &v : o_mesh.vertices)
    {
        o_mesh.normals.push_back(v / 1);
    }
}

/*void setUnitCylinder(Mesh &o_mesh, float radius, float height, int nX, int nY) // Cylindre creux
{
    o_mesh.vertices.clear();
    o_mesh.normals.clear();
    o_mesh.triangles.clear();

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
            o_mesh.vertices.push_back(Vec3(x, y, z));
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
            o_mesh.triangles.push_back(Triangle(a, b, c));
            o_mesh.triangles.push_back(Triangle(b, d, c));
        }
    }
    

    // Normales
    for (const Vec3 v : o_mesh.vertices)
    {
        o_mesh.normals.push_back(v / 1);
    }
}*/

void setUnitCylinder(Mesh &o_mesh, float radius, float height, int nX, int nY) {
    o_mesh.vertices.clear();
    o_mesh.normals.clear();
    o_mesh.triangles.clear();

    float half_height = height / 2;

    // Vertices
    for (int i = 0; i <= nX; i++) {
        float u = 2 * M_PI * i / nX;
        for (int j = 0; j <= nY; j++) {
            float v = height * j / nY - half_height;
            float x = radius * cos(u);
            float y = radius * sin(u);
            float z = v;
            o_mesh.vertices.push_back(Vec3(x, y, z));
        }
    }

    // Pole Nord
    int top_vertex_index = o_mesh.vertices.size();
    o_mesh.vertices.push_back(Vec3(0, 0, half_height));

    // Pole Sud
    int bottom_vertex_index = o_mesh.vertices.size();
    o_mesh.vertices.push_back(Vec3(0, 0, -half_height));

    // Triangles
    for (int i = 0; i < nX; i++) {
        int index = i * (nY + 1);
        for (int j = 0; j < nY; j++) {
            int a = index + j;
            int b = a + 1;
            int c = a + (nY + 1);
            int d = c + 1;
            o_mesh.triangles.push_back(Triangle(a, c, b));
            o_mesh.triangles.push_back(Triangle(b, c, d));
        }
    }
    // Triangles Pole Nord
    for (int i = 0; i < nX; i++) {
        int a = bottom_vertex_index;
        int b = i * (nY + 1);
        int c = (i + 1) * (nY + 1);
        o_mesh.triangles.push_back(Triangle(a, c, b));
    }
    // Triangle Pole Sud
    for (int i = 0; i < nX; i++) {
        int a = top_vertex_index;
        int b = (nX - i - 1) * (nY + 1) + nY;
        int c = (nX - i) * (nY + 1) + nY;
        o_mesh.triangles.push_back(Triangle(a, b, c));
    }

    // Normales
    for (const Vec3 &v : o_mesh.vertices)
    {
        o_mesh.normals.push_back(v / 1);
    }
}

void setUnitCone(Mesh &o_mesh, float radius, float height, int nX, int nY)
{
    o_mesh.vertices.clear();
    o_mesh.normals.clear();
    o_mesh.triangles.clear();

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
            o_mesh.vertices.push_back(Vec3(x, y, z));
        }
    }

    // Triangles
    // Sous le cone
    for (int i = 1; i <= nX; i++)
    {
        int a = 0;
        int b = i * (nY + 1);
        int c = (i % nX + 1) * (nY + 1);
        o_mesh.triangles.push_back(Triangle(a, b, c));
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
            o_mesh.triangles.push_back(Triangle(a, b, c));
            o_mesh.triangles.push_back(Triangle(b, d, c));
        }
    }

    // Normales
    for (const Vec3 &v : o_mesh.vertices)
    {
        o_mesh.normals.push_back(v / 1);
    }
}

void setUnitCylinderCone(Mesh &o_mesh, float radius, float height, int nX, int nY)
{
    o_mesh.vertices.clear();
    o_mesh.normals.clear();
    o_mesh.triangles.clear();

    // Cylinder
    setUnitCylinder(o_mesh, radius, height, nX, nY);

    // Cone
    Mesh cone;
    setUnitCone(cone, radius, height, nX, nY);

    // Translate cone to end of cylinder
    for (unsigned int i = 0; i < cone.vertices.size(); i++)
    {
        cone.vertices[i][2] -= height/2;
    }

    // Combine vertices, normals, and triangles from cylinder and cone
    int cylinderVertexCount = o_mesh.vertices.size();
    for (unsigned int i = 0; i < cone.vertices.size(); i++)
    {
        o_mesh.vertices.push_back(cone.vertices[i]);
    }

    for (unsigned int i = 0; i < cone.triangles.size(); i++)
    {
        Triangle triangle = cone.triangles[i];
        o_mesh.triangles.push_back(Triangle(triangle[0] + cylinderVertexCount, triangle[1] + cylinderVertexCount, triangle[2] + cylinderVertexCount));
    }

     // Normales
    for (const Vec3 &v : o_mesh.vertices)
    {
        o_mesh.normals.push_back(v / 1);
    }
}

float supershape(float phi, float a, float b, float m, float n1, float n2, float n3)
{
    float t1 = pow(abs((1 / a) * cos(m * phi / 4)), n2);
    float t2 = pow(abs((1 / b) * sin(m * phi / 4)), n3);
    float t3 = pow(t1 + t2, 1 / n1);
    return 1 / t3;
}

void setUnitSupershape(Mesh &o_mesh, int nX, int nY, float a, float b, float m, float n1, float n2, float n3)
{
    o_mesh.vertices.clear();
    o_mesh.normals.clear();
    o_mesh.triangles.clear();

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
            o_mesh.vertices.push_back(Vec3(x, y, z));
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
            o_mesh.triangles.push_back(Triangle(a, c, b));
            o_mesh.triangles.push_back(Triangle(b, c, d));
        }
    }

    // Normales
    for (const Vec3 &v : o_mesh.vertices)
    {
        o_mesh.normals.push_back(v / 1);
    }
}

void smooth(Mesh &o_mesh, float factor){
}

void scale(Mesh &o_mesh,int Axe, float factor)
{
    for (long unsigned int i = 0; i < o_mesh.vertices.size(); i++)
    {
        o_mesh.vertices[i][Axe] *= factor;
    }
}

void swell(Mesh &o_mesh, float factor)
{
    for (long unsigned int i = 0; i < o_mesh.vertices.size(); ++i) {
        Vec3 normal = o_mesh.normals[i];
        o_mesh.vertices[i] += factor * normal;
    }
}

bool saveOFF(const std::string &filename,
             std::vector<Vec3> &i_vertices,
             std::vector<Vec3> &i_normals,
             std::vector<Triangle> &i_triangles,
             bool save_normals = true)
{
    std::ofstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open())
    {
        std::cout << filename << " cannot be opened" << std::endl;
        return false;
    }

    myfile << "OFF" << std::endl;

    unsigned int n_vertices = i_vertices.size(), n_triangles = i_triangles.size();
    myfile << n_vertices << " " << n_triangles << " 0" << std::endl;

    for (unsigned int v = 0; v < n_vertices; ++v)
    {
        myfile << i_vertices[v][0] << " " << i_vertices[v][1] << " " << i_vertices[v][2] << " ";
        if (save_normals)
            myfile << i_normals[v][0] << " " << i_normals[v][1] << " " << i_normals[v][2] << std::endl;
        else
            myfile << std::endl;
    }
    for (unsigned int f = 0; f < n_triangles; ++f)
    {
        myfile << 3 << " " << i_triangles[f][0] << " " << i_triangles[f][1] << " " << i_triangles[f][2];
        myfile << std::endl;
    }
    myfile.close();
    return true;
}

void openOFF(std::string const &filename,
             std::vector<Vec3> &o_vertices,
             std::vector<Vec3> &o_normals,
             std::vector<Triangle> &o_triangles,
             bool load_normals = true)
{
    std::ifstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open())
    {
        std::cout << filename << " cannot be opened" << std::endl;
        return;
    }

    std::string magic_s;

    myfile >> magic_s;

    if (magic_s != "OFF")
    {
        std::cout << magic_s << " != OFF :   We handle ONLY *.off files." << std::endl;
        myfile.close();
        exit(1);
    }

    int n_vertices, n_faces, dummy_int;
    myfile >> n_vertices >> n_faces >> dummy_int;

    o_vertices.clear();
    o_normals.clear();

    for (int v = 0; v < n_vertices; ++v)
    {
        float x, y, z;

        myfile >> x >> y >> z;
        o_vertices.push_back(Vec3(x, y, z));

        if (load_normals)
        {
            myfile >> x >> y >> z;
            o_normals.push_back(Vec3(x, y, z));
        }
    }

    o_triangles.clear();
    for (int f = 0; f < n_faces; ++f)
    {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;

        if (n_vertices_on_face == 3)
        {
            unsigned int _v1, _v2, _v3;
            myfile >> _v1 >> _v2 >> _v3;

            o_triangles.push_back(Triangle(_v1, _v2, _v3));
        }
        else if (n_vertices_on_face == 4)
        {
            unsigned int _v1, _v2, _v3, _v4;
            myfile >> _v1 >> _v2 >> _v3 >> _v4;

            o_triangles.push_back(Triangle(_v1, _v2, _v3));
            o_triangles.push_back(Triangle(_v1, _v3, _v4));
        }
        else
        {
            std::cout << "We handle ONLY *.off files with 3 or 4 vertices per face" << std::endl;
            myfile.close();
            exit(1);
        }
    }
}

// ------------------------------------

void initLight()
{
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f, -16.0f, -50.0f};
    GLfloat color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};

    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
}

void init()
{
    camera.resize(SCREENWIDTH, SCREENHEIGHT);
    initLight();
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);

    displayMode = LIGHTED;
    display_normals = false;
    display_unit_sphere = false;
    display_unit_cube = false;
    display_unit_torus = false;
    display_unit_cylinder = false;
    display_unit_cone = false;
    display_unit_cylinder_cone = false;
    display_unit_supershape = false;
    display_loaded_mesh = true;

    glLineWidth(1.);
    glPointSize(4.);
}

// ------------------------------------
// rendering.
// ------------------------------------

void drawVector(Vec3 const &i_from, Vec3 const &i_to)
{

    glBegin(GL_LINES);
    glVertex3f(i_from[0], i_from[1], i_from[2]);
    glVertex3f(i_to[0], i_to[1], i_to[2]);
    glEnd();
}

void drawVertices(Mesh const &i_mesh)
{
    glBegin(GL_POINTS); // Fonction OpenGL de dessin de points
    for (unsigned int vIt = 0; vIt < i_mesh.vertices.size(); ++vIt)
    {
        Vec3 p = i_mesh.vertices[vIt];

        glVertex3f(p[0], p[1], p[2]);
    }
    glEnd();
}

void drawTriangleMesh(Mesh const &i_mesh)
{

    if (i_mesh.triangles.size() > 0)
    {
        if (i_mesh.normals.size() > 0)
        {
            // Fonction de dessin en utilisant les normales au sommet
            glBegin(GL_TRIANGLES); // Fonction OpenGL de dessin de triangles
            for (unsigned int tIt = 0; tIt < i_mesh.triangles.size(); ++tIt)
            {
                Vec3 p0 = i_mesh.vertices[i_mesh.triangles[tIt][0]];
                Vec3 n0 = i_mesh.normals[i_mesh.triangles[tIt][0]];

                Vec3 p1 = i_mesh.vertices[i_mesh.triangles[tIt][1]];
                Vec3 n1 = i_mesh.normals[i_mesh.triangles[tIt][1]];

                Vec3 p2 = i_mesh.vertices[i_mesh.triangles[tIt][2]];
                Vec3 n2 = i_mesh.normals[i_mesh.triangles[tIt][2]];

                glNormal3f(n0[0], n0[1], n0[2]);
                glVertex3f(p0[0], p0[1], p0[2]);
                glNormal3f(n1[0], n1[1], n1[2]);
                glVertex3f(p1[0], p1[1], p1[2]);
                glNormal3f(n2[0], n2[1], n2[2]);
                glVertex3f(p2[0], p2[1], p2[2]);
            }
            glEnd();
        }
        else
        {
            // Fonction de dessin en utilisant sans normales
            glBegin(GL_TRIANGLES); // Fonction OpenGL de dessin de triangles
            for (unsigned int tIt = 0; tIt < i_mesh.triangles.size(); ++tIt)
            {
                Vec3 p0 = i_mesh.vertices[i_mesh.triangles[tIt][0]];
                Vec3 p1 = i_mesh.vertices[i_mesh.triangles[tIt][1]];
                Vec3 p2 = i_mesh.vertices[i_mesh.triangles[tIt][2]];

                // Dessin des trois sommets formant le triangle
                glVertex3f(p0[0], p0[1], p0[2]);
                glVertex3f(p1[0], p1[1], p1[2]);
                glVertex3f(p2[0], p2[1], p2[2]);
            }
            glEnd();
        }
    }
    else
    {
        drawVertices(i_mesh);
    }

    if (display_normals)
    {

        glColor3f(1., 0., 0.);
        for (unsigned int pIt = 0; pIt < i_mesh.normals.size(); ++pIt)
        {
            Vec3 to = i_mesh.vertices[pIt] + 0.02 * i_mesh.normals[pIt];
            drawVector(i_mesh.vertices[pIt], to);
        }
    }

    drawVertices(i_mesh); // new
}

void draw()
{

    if (displayMode == LIGHTED || displayMode == LIGHTED_WIRE)
    {

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LIGHTING);
    }
    else if (displayMode == WIRE)
    {

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_LIGHTING);
    }
    else if (displayMode == SOLID)
    {
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (display_unit_sphere)
    {
        glColor3f(0.6, 1, 0.5);
        drawTriangleMesh(unit_sphere);
    }

    if (display_unit_cube)
    {
        glColor3f(1.0, 0.1, 0.1);
        drawTriangleMesh(unit_cube);
    }

    if (display_unit_torus)
    {
        glColor3f(0.4, 0.4, 1.0);
        drawTriangleMesh(unit_torus);
    }

    if (display_unit_cylinder)
    {
        glColor3f(0.7, 0.0, 1.0);
        drawTriangleMesh(unit_cylinder);
    }

    if (display_unit_cone)
    {
        glColor3f(1.0, 0.5, 0.0);
        drawTriangleMesh(unit_cone);
    }

    if (display_unit_cylinder_cone)
    {
        glColor3f(1.0, 0.5, 0.5);
        drawTriangleMesh(unit_cylinder_cone);
    }

    if (display_unit_supershape)
    {
        glColor3f(1.0, 1.0, 0);
        drawTriangleMesh(unit_supershape);
    }

    if (display_loaded_mesh)
    {
        glColor3f(0.96, 0.66, 0.86);
        drawTriangleMesh(mesh);
    }

    if (displayMode == SOLID || displayMode == LIGHTED_WIRE)
    {
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1.0f);
        glPolygonOffset(-2.0, 1.0);

        glColor3f(0., 0., 0.);
        if (display_unit_sphere)
            drawTriangleMesh(unit_sphere);
        if (display_unit_cube)
            drawTriangleMesh(unit_cube);
        if (display_unit_torus)
            drawTriangleMesh(unit_torus);
        if (display_unit_cylinder)
            drawTriangleMesh(unit_cylinder);
        if (display_unit_cone)
            drawTriangleMesh(unit_cone);
        if (display_unit_cylinder_cone)
            drawTriangleMesh(unit_cylinder_cone);
        if (display_unit_supershape)
            drawTriangleMesh(unit_supershape);
        if (display_loaded_mesh)
            drawTriangleMesh(mesh);

        glDisable(GL_POLYGON_OFFSET_LINE);
        glEnable(GL_LIGHTING);
    }
}

void changeDisplayMode()
{
    if (displayMode == LIGHTED)
        displayMode = LIGHTED_WIRE;
    else if (displayMode == LIGHTED_WIRE)
        displayMode = SOLID;
    else if (displayMode == SOLID)
        displayMode = WIRE;
    else
        displayMode = LIGHTED;
}

void display()
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply();
    draw();
    glFlush();

    glutSwapBuffers();
}

void idle()
{
    glutPostRedisplay();
}

void key(unsigned char keyPressed, int x, int y)
{
    switch (keyPressed)
    {
    case 'p':
        if (fullScreen == true)
        {
            glutReshapeWindow(SCREENWIDTH, SCREENHEIGHT);
            fullScreen = false;
        }
        else
        {
            glutFullScreen();
            fullScreen = true;
        }
        break;
    case 'w': // Change le mode d'affichage
        changeDisplayMode();
        break;
    case 'n': // Press n key to display normals
        display_normals = !display_normals;
        break;
    case '1': // Toggle loaded mesh display
        display_loaded_mesh = !display_loaded_mesh;
        break;
    case '2': // Toggle unit sphere mesh display
        display_unit_sphere = !display_unit_sphere;
        break;
    case '3': // Toggle unit cube mesh display
        display_unit_cube = !display_unit_cube;
        break;
    case '4': // Toggle unit torus mesh display
        display_unit_torus = !display_unit_torus;
        break;
    case '5': // Toggle unit cylinder mesh display
        display_unit_cylinder = !display_unit_cylinder;
        break;
    case '6': // Toggle unit cone mesh display
        display_unit_cone = !display_unit_cone;
        break;
    case '7': // Toggle unit cylinder cone mesh display
        display_unit_cylinder_cone = !display_unit_cylinder_cone;
        break;
    case '8': // Toggle unit supershape mesh display
        display_unit_supershape = !display_unit_supershape;
        break;
    case '-':
        valfX = std::max(0, valfX - 1);
        valfY = std::max(0, valfY - 1);
        setUnitSphere(unit_sphere, valfX, valfY);
        setUnitCube(unit_cube, valfX, valfY);
        setUnitTorus(unit_torus, valfR, valfr, valfX, valfY);
        setUnitCylinder(unit_cylinder, valRadius, valHeight, valfX, valfY);
        setUnitCone(unit_cone, valRadius, valHeight, valfX, valfY);
        setUnitCylinderCone(unit_cylinder_cone, valRadius, valHeight, valfX, valfY);
        setUnitSupershape(unit_supershape, valfX, valfY, vala, valb, valm, valn1, valn2, valn3);
        break;
    case '+':
        valfX += 1;
        valfY += 1;
        setUnitSphere(unit_sphere, valfX, valfY);
        setUnitCube(unit_cube, valfX, valfY);
        setUnitTorus(unit_torus, valfR, valfr, valfX, valfY);
        setUnitCylinder(unit_cylinder, valRadius, valHeight, valfX, valfY);
        setUnitCone(unit_cone, valRadius, valHeight, valfX, valfY);
        setUnitCylinderCone(unit_cylinder_cone, valRadius, valHeight, valfX, valfY);
        setUnitSupershape(unit_supershape, valfX, valfY, vala, valb, valm, valn1, valn2, valn3);
        break;
    // Torus
    case 'q':
        valfr += 0.1;
        setUnitTorus(unit_torus, valfR, valfr, valfX, valfY);
        break;
    case 'Q':
        valfr = std::max(0.0f, valfr - 0.1f);
        setUnitTorus(unit_torus, valfR, valfr, valfX, valfY);
        break;
    case 's':
        valfR += 0.1;
        setUnitTorus(unit_torus, valfR, valfr, valfX, valfY);
        break;
    case 'S':
        valfR = std::max(0.0f, valfR - 0.1f);
        setUnitTorus(unit_torus, valfR, valfr, valfX, valfY);
        break;
    // Cylinder
    case 'd':
        valHeight += 0.1;
        setUnitCylinder(unit_cylinder, valRadius, valHeight, valfX, valfY);
        setUnitCone(unit_cone, valRadius, valHeight, valfX, valfY);
        setUnitCylinderCone(unit_cylinder_cone, valRadius, valHeight, valfX, valfY);
        break;
    case 'D':
        valHeight = std::max(0.0f, valHeight - 0.1f);
        setUnitCylinder(unit_cylinder, valRadius, valHeight, valfX, valfY);
        setUnitCone(unit_cone, valRadius, valHeight, valfX, valfY);
        setUnitCylinderCone(unit_cylinder_cone, valRadius, valHeight, valfX, valfY);
        break;
    case 'f':
        valRadius += 0.1;
        setUnitCylinder(unit_cylinder, valRadius, valHeight, valfX, valfY);
        setUnitCone(unit_cone, valRadius, valHeight, valfX, valfY);
        setUnitCylinderCone(unit_cylinder_cone, valRadius, valHeight, valfX, valfY);
        break;
    case 'F':
        valRadius = std::max(0.0f, valRadius - 0.1f);
        setUnitCylinder(unit_cylinder, valRadius, valHeight, valfX, valfY);
        setUnitCone(unit_cone, valRadius, valHeight, valfX, valfY);
        setUnitCylinderCone(unit_cylinder_cone, valRadius, valHeight, valfX, valfY);
        break;
    // Supershape
    case 'g':
        vala += 0.1;
        setUnitSupershape(unit_supershape, valfX, valfY, vala, valb, valm, valn1, valn2, valn3);
        break;
    case 'G':
        vala = std::max(0.0f, vala - 0.1f);
        setUnitSupershape(unit_supershape, valfX, valfY, vala, valb, valm, valn1, valn2, valn3);
        break;
    case 'h':
        valb += 0.1;
        setUnitSupershape(unit_supershape, valfX, valfY, vala, valb, valm, valn1, valn2, valn3);
        break;
    case 'H':
        valb = std::max(0.0f, valb - 0.1f);
        setUnitSupershape(unit_supershape, valfX, valfY, vala, valb, valm, valn1, valn2, valn3);
        break;
    case 'j':
        valm += 0.1;
        setUnitSupershape(unit_supershape, valfX, valfY, vala, valb, valm, valn1, valn2, valn3);
        break;
    case 'J':
        valm = std::max(0.0f, valm - 0.1f);
        setUnitSupershape(unit_supershape, valfX, valfY, vala, valb, valm, valn1, valn2, valn3);
        break;
    case 'k':
        valn1 += 0.1;
        setUnitSupershape(unit_supershape, valfX, valfY, vala, valb, valm, valn1, valn2, valn3);
        break;
    case 'K':
        valn1 = std::max(0.0f, valn1 - 0.1f);
        setUnitSupershape(unit_supershape, valfX, valfY, vala, valb, valm, valn1, valn2, valn3);
        break;
    case 'l':
        valn2 += 0.1;
        setUnitSupershape(unit_supershape, valfX, valfY, vala, valb, valm, valn1, valn2, valn3);
        break;
    case 'L':
        valn2 = std::max(0.0f, valn2 - 0.1f);
        setUnitSupershape(unit_supershape, valfX, valfY, vala, valb, valm, valn1, valn2, valn3);
        break;
    case 'm':
        valn3 += 0.1;
        setUnitSupershape(unit_supershape, valfX, valfY, vala, valb, valm, valn1, valn2, valn3);
        break;
    case 'M':
        valn3 = std::max(0.0f, valn3 - 0.1f);
        setUnitSupershape(unit_supershape, valfX, valfY, vala, valb, valm, valn1, valn2, valn3);
        break;
    case 'Z': // Etire le cube selon l'axe z
        if(display_unit_sphere)
            scale(unit_sphere,2, 1.1); // Augmente la taille selon l'axe z
        if(display_unit_cube)
            scale(unit_cube,2, 1.1); // Augmente la taille selon l'axe z
        if(display_unit_cylinder)
            scale(unit_cylinder,2, 1.1); // Augmente la taille selon l'axe z
        if(display_unit_torus)
            scale(unit_torus,2, 1.1); // Augmente la taille selon l'axe z
        if(display_unit_cone)
            scale(unit_cone,2, 1.1); // Augmente la taille selon l'axe z
        if(display_unit_cylinder_cone)
            scale(unit_cylinder_cone,2, 1.1); // Augmente la taille selon l'axe z
        if(display_unit_supershape)
            scale(unit_supershape,2, 1.1); // Augmente la taille selon l'axe z
        break;
    case 'z': // Etire le cube selon l'axe z
        if(display_unit_sphere)
            scale(unit_sphere,2, 0.9); // Diminue la taille selon l'axe z
        if(display_unit_cube)
            scale(unit_cube,2, 0.9); // Diminue la taille selon l'axe z
        if(display_unit_cylinder)
            scale(unit_cylinder,2, 0.9); // Diminue la taille selon l'axe z
        if(display_unit_torus)
            scale(unit_torus,2, 0.9); // Diminue la taille selon l'axe z
        if(display_unit_cone)
            scale(unit_cone,2, 0.9); // Diminue la taille selon l'axe z
        if(display_unit_cylinder_cone)
            scale(unit_cylinder_cone,2, 0.9); // Diminue la taille selon l'axe z
        if(display_unit_supershape)
            scale(unit_supershape,2, 0.9); // Diminue la taille selon l'axe z
        break;
    case 'A': // Etire le cube selon l'axe x
        if(display_unit_sphere)
            scale(unit_sphere,0, 1.1); // Augmente la taille selon l'axe x
        if(display_unit_cube)
            scale(unit_cube,0, 1.1); // Augmente la taille selon l'axe x
        if(display_unit_cylinder)
            scale(unit_cylinder,0, 1.1); // Augmente la taille selon l'axe x
        if(display_unit_torus)
            scale(unit_torus,0, 1.1); // Augmente la taille selon l'axe x
        if(display_unit_cone)
            scale(unit_cone,0, 1.1); // Augmente la taille selon l'axe x
        if(display_unit_cylinder_cone)
            scale(unit_cylinder_cone,0, 1.1); // Augmente la taille selon l'axe x
        if(display_unit_supershape)
            scale(unit_supershape,0, 1.1); // Augmente la taille selon l'axe x
        break;
    case 'a': // Etire le cube selon l'axe x
        if(display_unit_sphere)
            scale(unit_sphere,0, 0.9); // Diminue la taille selon l'axe x
        if(display_unit_cube)
            scale(unit_cube,0, 0.9); // Diminue la taille selon l'axe x
        if(display_unit_cylinder)
            scale(unit_cylinder,0, 0.9); // Diminue la taille selon l'axe x
        if(display_unit_torus)
            scale(unit_torus,0, 0.9); // Diminue la taille selon l'axe x
        if(display_unit_cone)
            scale(unit_cone,0, 0.9); // Diminue la taille selon l'axe x
        if(display_unit_cylinder_cone)
            scale(unit_cylinder_cone,0, 0.9); // Diminue la taille selon l'axe x
        if(display_unit_supershape)
            scale(unit_supershape,0, 0.9); // Diminue la taille selon l'axe x
        break;
    case 'O':  // Etire le cube selon l'axe y
        if(display_unit_sphere)
            scale(unit_sphere,1, 1.1); // Augmente la taille selon l'axe y
        if(display_unit_cube)
            scale(unit_cube,1, 1.1); // Augmente la taille selon l'axe y
        if(display_unit_cylinder)
            scale(unit_cylinder,1, 1.1); // Augmente la taille selon l'axe y
        if(display_unit_torus)
            scale(unit_torus,1, 1.1); // Augmente la taille selon l'axe y
        if(display_unit_cone)
            scale(unit_cone,1, 1.1); // Augmente la taille selon l'axe y
        if(display_unit_cylinder_cone)
            scale(unit_cylinder_cone,1, 1.1); // Augmente la taille selon l'axe y
        if(display_unit_supershape)
            scale(unit_supershape,1, 1.1); // Augmente la taille selon l'axe y
        break;
    case 'o': // Etire le cube selon l'axe y
        if(display_unit_sphere)
            scale(unit_sphere,1, 0.9); // Diminue la taille selon l'axe y
        if(display_unit_cube)
            scale(unit_cube,1, 0.9); // Diminue la taille selon l'axe y
        if(display_unit_cylinder)
            scale(unit_cylinder,1, 0.9); // Diminue la taille selon l'axe y
        if(display_unit_torus)
            scale(unit_torus,1, 0.9); // Diminue la taille selon l'axe y
        if(display_unit_cone)
            scale(unit_cone,1, 0.9); // Diminue la taille selon l'axe y
        if(display_unit_cylinder_cone)
            scale(unit_cylinder_cone,1, 0.9); // Diminue la taille selon l'axe y
        if(display_unit_supershape)
            scale(unit_supershape,1, 0.9); // Diminue la taille selon l'axe y
        break;// Etire le cube selon l'axe y
    case 'u':
            swell(mesh,0.01f);
        break;
    case 'U':
            swell(mesh,-0.01f);
        break;
    case 'i':
            smooth(mesh,0.01f);
        break;
    case 'I':
            smooth(mesh,-0.01f);
        break;
    default:
        break;
    }
    idle();
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_UP)
    {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    }
    else
    {
        if (button == GLUT_LEFT_BUTTON)
        {
            camera.beginRotate(x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
        }
        else if (button == GLUT_RIGHT_BUTTON)
        {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        }
        else if (button == GLUT_MIDDLE_BUTTON)
        {
            if (mouseZoomPressed == false)
            {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }
    idle();
}


void motion(int x, int y)
{
    if (mouseRotatePressed == true)
    {
        camera.rotate(x, y);
    }
    else if (mouseMovePressed == true)
    {
        camera.move((x - lastX) / static_cast<float>(SCREENWIDTH), (lastY - y) / static_cast<float>(SCREENHEIGHT), 0.0);
        lastX = x;
        lastY = y;
    }
    else if (mouseZoomPressed == true)
    {
        camera.zoom(float(y - lastZoom) / SCREENHEIGHT);
        lastZoom = y;
    }
}

void reshape(int w, int h)
{
    camera.resize(w, h);
}

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        exit(EXIT_FAILURE);
    }
    glutInit(&argc, argv);                                     // créer une feunetre
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); // couleurs
    glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);             // tailles
    window = glutCreateWindow("TP HAI605I");                   // titre

    init();                   // Quel fichier ouvrir par exemple
    glutIdleFunc(idle);       // Quand il se passe rien appeller la foncton "idle"
    glutDisplayFunc(display); // Quand on affiche quelque chose ...
    glutKeyboardFunc(key);    // Touche clavier
    glutReshapeFunc(reshape); // Resize
    glutMotionFunc(motion);   // Mouvement souris
    glutMouseFunc(mouse);     // Clic souris
    key('?', 0, 0);

    // Unit sphere mesh loaded with precomputed normals
    //openOFF("data/unit_sphere_n.off", mesh.vertices, mesh.normals, mesh.triangles); // Ouvrir le fichier
    openOFF("data/avion_n.off", mesh.vertices, mesh.normals, mesh.triangles); // Ouvrir le fichier

    // Uncomment to see other meshes
    // openOFF("data/elephant_n.off", mesh.vertices, mesh.normals, mesh.triangles);

    setUnitSphere(unit_sphere, valfX, valfY);
    setUnitCube(unit_cube, valfX, valfY);
    setUnitTorus(unit_torus, valfR, valfr, valfX, valfY);
    setUnitCylinder(unit_cylinder, valRadius, valHeight, valfX, valfY);
    setUnitCone(unit_cone, valRadius, valHeight, valfX, valfY);
    setUnitCylinderCone(unit_cylinder_cone, valRadius, valHeight, valfX, valfY);
    setUnitSupershape(unit_supershape, valfX, valfY, vala, valb, valm, valn1, valn2, valn3);

    glutMainLoop(); // Le programme se lance et appelle en boucle les fonctions au dessus
    return EXIT_SUCCESS;
}
