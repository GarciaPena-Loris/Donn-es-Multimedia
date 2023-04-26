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

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glut.h>

#include "src/Shader.h"
#include "src/Vec3D.h"
#include "src/Vertex.h"
#include "src/Triangle.h"
#include "src/Mesh.h"
#include "src/Camera.h"

using namespace std;

class PhongShader : public Shader {
public:
    PhongShader () { init ("shader.vert", "shader.frag"); }
    inline virtual ~PhongShader () {}

    void setAmbientRef (float s) {
        glUniform1fARB (ambientRefLocation, s);
    }

    void setDiffuseRef (float s) {
        glUniform1fARB (diffuseRefLocation, s);
    }

    void setSpecularRef (float s) {
        glUniform1fARB (specularRefLocation, s);
    }

    void setShininess (float s) {
        glUniform1fARB (shininessLocation, s);
    }

    void setLevels (int l) {
        glUniform1iARB (levelsLocation, l);
    }

private:
    void init (const std::string & vertexShaderFilename,
               const std::string & fragmentShaderFilename) {
        loadFromFile (vertexShaderFilename, fragmentShaderFilename);
        bind ();
        ambientRefLocation = getUniLoc ("ambientRef");
        diffuseRefLocation = getUniLoc ("diffuseRef");
        specularRefLocation = getUniLoc ("specularRef");
        shininessLocation = getUniLoc ("shininess");
        levelsLocation = getUniLoc ("levels");
    }
    GLint ambientRefLocation;
    GLint diffuseRefLocation;
    GLint specularRefLocation;
    GLint shininessLocation;
    GLint levelsLocation;
};

static GLint window;
static unsigned int SCREENWIDTH = 1024;
static unsigned int SCREENHEIGHT = 768;
static Camera camera;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX=0, lastY=0, lastZoom=0;
static unsigned int FPS = 0;
static bool fullScreen = false;

static PhongShader * phongShader;

static Mesh current_mesh;
static Mesh mesh_pose_0;
static Mesh mesh_pose_1;
static Mesh mesh_pose_2;
static GLuint glID;

static int levels = 4;
static float ambientRef = 0.2f;
static float diffuseRef = 0.8f;
static float specularRef = 0.5f;
static float shininess = 16.0f;
static float interpolant0 = 1.f;
static float interpolant1 = 0.f;
static float interpolant2 = 0.f;
static float anglex = 0.f;
static float angley = 0.f;
static float anglez = 0.f;
static float offsetx = 0.f;
static float offsety = 0.f;
static float offsetz = 0.f;
bool show_animation = false;
bool increment1 = true;
bool increment2 = true;

float nX = 15;
float nY = 15;
float R = 0.7;
float r = 0.3;
float radius = 0.5;
float height = 1;
float m = 6;
float a = 1;
float b = 1;
float n1 = 1;
float n2 = 1;
float n3 = 1;

bool display_normals;
int compteurChameaux = 0;

typedef enum {Wire, Phong, Solid} RenderingMode;
static RenderingMode mode = Phong;
void initGLList ();
void openOFF (const std::string filename, Mesh &mesh, unsigned int normWeight) {
    vector<Vertex> V;
    vector<Triangle> T;

    ifstream in (filename.c_str ());
    if (!in)
        exit (EXIT_FAILURE);
    string offString;
    unsigned int sizeV, sizeT, tmp;
    in >> offString >> sizeV >> sizeT >> tmp;
    for (unsigned int i = 0; i < sizeV; i++) {
        Vec3 p;
        in >> p;
        V.push_back (Vertex (p));
    }
    int s;
    for (unsigned int i = 0; i < sizeT; i++) {
        in >> s;
        unsigned int v[3];
        for (unsigned int j = 0; j < 3; j++)
            in >> v[j];
        T.push_back (Triangle (v[0], v[1], v[2]));
    }
    in.close ();

    Vec3 center;
    float radius;
    Vertex::scaleToUnitBox (V, center, radius);
    mesh = Mesh (V, T);
    mesh.recomputeSmoothVertexNormals (normWeight);

}

inline void glVertexVec3Df (const Vec3 & v) {
    glVertex3f (v[0], v[1], v[2]);
}

inline void glNormalVec3Df (const Vec3 & n) {
    glNormal3f (n[0], n[1], n[2]);
}

inline void glDrawPoint (const Vec3 & pos, const Vec3 & normal) {
    glNormalVec3Df (normal);
    glVertexVec3Df (pos);
}

inline void glDrawPoint (const Vertex & v) {
    glDrawPoint (v.getPosition (), v.getNormal ());
}




// ########################################
// ########################################
// ########################################
// ########################################


//A completer
void updateAnimation (){
    //Récupérer la position des sommets du maillage courant à mettre à jour
    vector<Vertex> & V = current_mesh.getVertices ();

    //(1) Faire l'interpolation linéaire entre les positions de mesh_pose_0 et mesh_pose_1
    //Affecter le résultat aux positions de current_mesh

    const vector<Vertex> & V0 = mesh_pose_0.getVertices ();
    const vector<Vertex> & V1 = mesh_pose_1.getVertices ();
    const vector<Vertex> & V2 = mesh_pose_2.getVertices ();

    //Les variables interpolant0, interpolant1 et interpolant3 permettent de changer la pondération
    //Calule les poids w0 tel que la somme de w0, w1, w2 soit égale à 1
    float w0 = interpolant0;
    float w1 = interpolant1;
    float w2 = interpolant2;
    // Normaliser les poids : i.e. diviser chaque poids par la somme des poids
    //A completer

    //(2) Faire la moyenne pondérée entre les positions de mesh_pose_0, mesh_pose_1 et Mesh_pose_2
    w0 /= (w0 + w1 + w2);
    w1 /= (w0 + w1 + w2);
    w2 /= (w0 + w1 + w2);

    /* (1)
    for (int i = 0; i < V.size(); i++) {
        V[i].setPosition((1-w1) * V0[i].getPosition() + w1 * V1[i].getPosition());
    } */

    // (2)
    //Affecter le résultat aux positions de current_mesh
    
    for (unsigned int i = 0; i < V.size(); i++) {
        V[i].setPosition(((1-w1) * V0[i].getPosition() + w1 * V1[i].getPosition() + (1-w2) * V0[i].getPosition() + w2 * V2[i].getPosition()) / 2);
    }

    //Ajouter des transformation

    //Matrices de rotation
    Mat3 Rx, Ry, Rz;

    //Mettre a jour Rx pour avoir une rotation atour de l'axe x de angle
    // A completer
    Rx(0,0) = 1;                Rx(0,1) = 0;                Rx(0,2) = 0;
    Rx(1,0) = 0;                Rx(1,1) = cos(anglex);       Rx(1,2) = -sin(anglex);
    Rx(2,0) = 0;                Rx(2,1) = sin(anglex);       Rx(2,2) = cos(anglex);

    //Mettre a jour Ry pour avoir une rotation atour de l'axe y de angle
    Ry(0,0) = cos(angley);       Ry(0,1) = 0;                Ry(0,2) = sin(angley);
    Ry(1,0) = 0;                Ry(1,1) = 1;                Ry(1,2) = 0;
    Ry(2,0) = -sin(angley);      Ry(2,1) = 0;                Ry(2,2) = cos(angley);

    //Mettre a jour Rz pour avoir une rotation atour de l'axe z de angle
    Rz(0,0) = cos(anglez);       Rz(0,1) = -sin(anglez);      Rz(0,2) = 0;
    Rz(1,0) = sin(anglez);       Rz(1,1) = cos(anglez);       Rz(1,2) = 0;
    Rz(2,0) = 0;                Rz(2,1) = 0;                Rz(2,2) = 1;

    //Matrice rotation appliquer au resulat
    //tester votre matrices en utilisant la matrice model
    Mat3 rotation = Rx * Ry * Rz;

    for (unsigned int i = 0; i < V.size(); i++) {
        V[i].setPosition(rotation * V[i].getPosition());
    }

    //Translation a mettre a jour en utilisant la variable offset
    //Définir un vecteur de translation en utilisant la matrice offset
    Vec3 translation(offsetx, offsety, offsetz);

    //Appliquer la translation à chaque sommet
    for (unsigned int i = 0; i < V.size(); i++) {
        V[i].setPosition(V[i].getPosition() + translation);
    }

    //Recalcule des normales et mise à jour de l'affichage
    current_mesh.recomputeSmoothVertexNormals(0);
    initGLList ();
}

// ########################################
// ########################################
// ########################################
// ########################################

void smooth(float factor){
    current_mesh.smooth(factor);

    //Recalcule des normales et mise à jour de l'affichage
    current_mesh.recomputeSmoothVertexNormals(0);
    initGLList ();
}

void smoothTaubin(int ite, float lambda, float mu){
    current_mesh.smoothTaubin(lambda, mu, ite);
    //Recalcule des normales et mise à jour de l'affichage
    current_mesh.recomputeSmoothVertexNormals(0);
    initGLList ();
}

void swell(float factor){
    current_mesh.swell(factor);

    //Recalcule des normales et mise à jour de l'affichage
    current_mesh.recomputeSmoothVertexNormals(0);
    initGLList ();
}

void scale(int Axe, float factor){
    current_mesh.scale(Axe, factor);

    //Recalcule des normales et mise à jour de l'affichage
    current_mesh.recomputeSmoothVertexNormals(0);
    initGLList ();
}

// ########################################
// ########################################

void animation() {
    if (increment1) {
        interpolant1 += 0.1;
        if (interpolant1 >= 1.0) {
            increment1 = false;
            increment2 = true;
        }
    }
    else if (increment2) {
        interpolant2 += 0.1;
        if (interpolant2 >= 1.0) {
            increment2 = false;
        }
    }
    else if (interpolant2 >= 0.0) {
        interpolant2 -= 0.1;
    }
    else {
        interpolant1 -= 0.1;
        if (interpolant1 <= 0.0) {
            increment1 = true;
        }
    }

    if (interpolant1 >= 0 && interpolant1 <= 0.5) {
        anglex += 0.1f;
        angley += 0.1f;
        anglez += 0.1f;
        if( anglex >= (float)M_PI*2.f ) anglex = 0.;
        if( angley >= (float)M_PI*2.f ) angley = 0.;
        if( anglez >= (float)M_PI*2.f ) anglez = 0.;
    }

    if (compteurChameaux <= 5) {
        offsetx += 0.1;
        offsety += 0.1;
    }
    else if (compteurChameaux >= 100 && compteurChameaux <= 105) {
        offsetx = offsetx - 0.3; 
    }
    else if (compteurChameaux >= 200 &&  compteurChameaux <= 205) {
        offsety = offsety - 0.2; 
    }
    else if (compteurChameaux >= 300 &&  compteurChameaux <= 305) {
        offsetx = offsetx + 0.3; 
    }
    else if (compteurChameaux >= 400 &&  compteurChameaux <= 405) {
        offsetx -= 0.1;
        offsety += 0.1;
    }
    
    if (interpolant1 >= 0 && interpolant1 <= 0.3)
        glClearColor (1.0, 0.0, 0.0, 1.0);
    if (interpolant1 >= 0.3 && interpolant1 <= 0.6)
        glClearColor (0.0, 1.0, 0.0, 1.0);
    if (interpolant1 >= 0.6 && interpolant1 <= 1)
        glClearColor (0.0, 0.0, 1.0, 1.0);
    if (interpolant2 >= 0 && interpolant2 <= 0.3)
        glClearColor (1.0, 1.0, 0.0, 1.0);
    if (interpolant2 >= 0.3 && interpolant2 <= 0.6)
        glClearColor (0.0, 1.0, 1.0, 1.0);
    if (interpolant2 >= 0.6 && interpolant2 <= 1)
        glClearColor (1.0, 0.0, 1.0, 1.0);

    updateAnimation();
    compteurChameaux = (compteurChameaux + 1) % 500;
}

// ########################################
// ########################################

void displayUnitCamel() {
    openOFF(std::string("./data/camel.off"), current_mesh, 0);
    initGLList();
}

void displayUnitSphere() {
    GLfloat material_color[4] = {1.0, 0.0, 1.0, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);

    current_mesh.setUnitSphere(nX, nY);
    initGLList();
}

void displayUnitCube() {
    GLfloat material_color[4] = {1.0, 0.0, 0.0, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
    current_mesh.setUnitCube(nX, nY);
    initGLList();
}

void displayUnitTorus() {
    GLfloat material_color[4] = {0.2, 0.2, 1.0, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
    current_mesh.setUnitTorus(R, r, nX, nY);
    initGLList();
}

void displayUnitEmptyCylinder() {
    GLfloat material_color[4] = {0.0, 0.0, 0.0, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
    current_mesh.setUnitEmptyCylinder(R, r, nX, nY);
    initGLList();
}

void displayUnitCylinder() {
    GLfloat material_color[4] = {1.0, 1.0, 0.0, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
    current_mesh.setUnitCylinder(R, r, nX, nY);
    initGLList();
}

void displayUnitCone() {
    GLfloat material_color[4] = {1.0, 1.0, 0.9, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
    current_mesh.setUnitCone(radius, height, nX, nY);
    initGLList();
}

void displayUnitCylinderCone() {
    GLfloat material_color[4] = {1.0, 0.2, 0.3, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
    current_mesh.setUnitCylinderCone(radius, height, nX, nY);
    initGLList();
}

void displayUnitSupershape() {
    GLfloat material_color[4] = {0.0, 1.0, 1.0, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
    current_mesh.setUnitSupershape(nX, nY, a, b, m, n1, n2, n3);
    initGLList();
}


// ########################################
// ########################################
// ########################################
// ########################################







void setShaderValues () {
    phongShader->setAmbientRef(ambientRef);
    phongShader->setDiffuseRef (diffuseRef);
    phongShader->setSpecularRef (specularRef);
    phongShader->setShininess (shininess);
    phongShader->setLevels(levels);
}

void drawMesh (bool flat) {
    const vector<Vertex> & V = current_mesh.getVertices ();
    const vector<Triangle> & T = current_mesh.getTriangles ();
    glBegin (GL_TRIANGLES);
    for (unsigned int i = 0; i < T.size (); i++) {
        const Triangle & t = T[i];
        if (flat) {
            Vec3 normal = Vec3::crossProduct (V[t.getVertex (1)].getPosition ()
                    - V[t.getVertex (0)].getPosition (),
                    V[t.getVertex (2)].getPosition ()
                    - V[t.getVertex (0)].getPosition ());
            normal.normalize ();
            glNormalVec3Df (normal);
        }
        for (unsigned int j = 0; j < 3; j++)
            if (!flat) {
                glNormalVec3Df (V[t.getVertex (j)].getNormal ());
                glVertexVec3Df (V[t.getVertex (j)].getPosition ());
            } else
                glVertexVec3Df (V[t.getVertex (j)].getPosition ());
    }
    glEnd ();
}

void drawSolidModel () {
    glEnable (GL_LIGHTING);
    glEnable (GL_COLOR_MATERIAL);
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    glPolygonOffset (1.0, 1.0);
    glEnable (GL_POLYGON_OFFSET_FILL);
    glShadeModel (GL_FLAT);
    phongShader->bind ();
    drawMesh (true);
    glPolygonMode (GL_FRONT, GL_LINE);
    glPolygonMode (GL_BACK, GL_FILL);
    glColor3f (0.0, 0.0, 0.0);
    drawMesh (true);
    glDisable (GL_POLYGON_OFFSET_FILL);
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    glDisable (GL_COLOR_MATERIAL);
    glDisable (GL_LIGHTING);
    glShadeModel (GL_SMOOTH);
}

void drawPhongModel () {
    glCallList (glID);
}

void initLights () {

    GLfloat light_position_0[4] = {42, 374, 161, 0};
    GLfloat light_position_1[4] = {473, -351, -259, 0};
    GLfloat light_position_2[4] = {-438, 167, -48, 0};

    GLfloat direction_0[3] = {-42, -374, -161};
    GLfloat direction_1[3] = {-473, 351, 259};
    GLfloat direction_2[3] = {438, -167, 48};

    GLfloat diffuse_color_0[4] = {1.0, 1.0, 1.0, 1};
    GLfloat diffuse_color_1[4] = {0.28, 0.39, 1.0, 1};
    GLfloat diffuse_color_2[4] = {1.0, 0.69, 0.23, 1};

    GLfloat specular_color_0[4] = {0.5, 0.0, 0.0, 1};
    GLfloat specular_color_1[4] = {0.0, 0.8, 0.0, 1};
    GLfloat specular_color_2[4] = {0.0, 0.0, 0.8, 1};

    GLfloat ambient[4] = {0.4f, 0.4f, 0.4f, 1.f};

    glLightfv (GL_LIGHT0, GL_POSITION, light_position_0);
    glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, direction_0);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse_color_0);
    glLightfv (GL_LIGHT0, GL_SPECULAR, specular_color_0);

    glLightfv (GL_LIGHT1, GL_POSITION, light_position_1);
    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direction_1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, diffuse_color_1);
    glLightfv (GL_LIGHT1, GL_SPECULAR, specular_color_1);

    glLightfv (GL_LIGHT2, GL_POSITION, light_position_2);
    glLightfv (GL_LIGHT2, GL_SPOT_DIRECTION, direction_2);
    glLightfv (GL_LIGHT2, GL_DIFFUSE, diffuse_color_2);
    glLightfv (GL_LIGHT2, GL_SPECULAR, specular_color_2);

    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambient);

    glEnable (GL_LIGHTING);
}

void setSunriseLight () {
    glDisable (GL_LIGHT0);
    glDisable (GL_LIGHT1);
    glDisable (GL_LIGHT2);
}

void setSingleSpotLight () {
    glEnable (GL_LIGHT0);
    glDisable (GL_LIGHT1);
    glDisable (GL_LIGHT2);
}

void setDefaultMaterial () {
    GLfloat material_color[4] = {1.0, 1.0, 1., 1.0f };
    GLfloat material_specular[4] = {0.5, 0.5, 0.5, 1.0 };
    GLfloat material_ambient[4] = {1.0, 0.0, 0.0, 1.0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);

    glDisable (GL_COLOR_MATERIAL);
}

void initGLList () {
    glID = glGenLists (1);
    glNewList (glID, GL_COMPILE);
    drawMesh (false);
    glEndList ();
}

void init () {
    glewInit();
    if (glewGetExtension ("GL_ARB_vertex_shader")        != GL_TRUE ||
            glewGetExtension ("GL_ARB_shader_objects")       != GL_TRUE ||
            glewGetExtension ("GL_ARB_shading_language_100") != GL_TRUE) {
        cerr << "Driver does not support OpenGL Shading Language" << endl;
        exit (EXIT_FAILURE);
    }
    if (glewGetExtension ("GL_ARB_vertex_buffer_object") != GL_TRUE) {
        cerr << "Driver does not support Vertex Buffer Objects" << endl;
        exit (EXIT_FAILURE);
    }

    camera.resize (SCREENWIDTH, SCREENHEIGHT);
    glClearColor (0.5, 0.5, 0.5, 1.0);

    initLights ();
    setSunriseLight ();
    setDefaultMaterial ();
    openOFF(std::string("./data/camel.off"), current_mesh, 0);
    openOFF(std::string("./data/camel.off"), mesh_pose_0, 0);
    openOFF(std::string("./data/camel_pose_1.off"), mesh_pose_1, 0);
    openOFF(std::string("./data/camel_pose_2.off"), mesh_pose_2, 0);
    initGLList ();

    try {
        phongShader = new PhongShader;
        phongShader->bind ();
        setShaderValues ();
    } catch (ShaderException e) {
        cerr << e.getMessage () << endl;
        exit (EXIT_FAILURE);
    }
}

void clear () {
    delete phongShader;
    glDeleteLists (glID, 1);
}

void reshape(int w, int h) {
    camera.resize (w, h);
}

void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    if (mode == Solid)
        drawSolidModel ();
    else if (mode == Phong || mode == Wire )
        drawPhongModel ();
    glFlush ();
    glutSwapBuffers ();
}

void idle () {
    static float lastTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
    static unsigned int counter = 0;
    counter++;
    int nbBoucle = 0;
    float currentTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
    if (currentTime - lastTime >= 1000.0f) {
        FPS = counter;
        counter = 0;
        static char FPSstr [128];
        unsigned int numOfTriangles = current_mesh.getTriangles ().size ();
        if (mode == Solid)
            sprintf (FPSstr, "HAI60I - Examen TP: %d tri. - solid shading - %d FPS.",
                     numOfTriangles, FPS);
        else if (mode == Phong)
            sprintf (FPSstr, "HAI60I - Examen TP: %d tri. - Phong shading - %d FPS.",
                     numOfTriangles, FPS);
        glutSetWindowTitle (FPSstr);
        lastTime = currentTime;
        nbBoucle = nbBoucle + 1 % 2000;
    }
    if (nbBoucle == 0 && show_animation) {
        animation();
    }
    
    glutPostRedisplay ();
}

void printUsage () {
    cerr << endl
         << "--------------------------------------" << endl
         << "TP REVISION" << endl
         << "--------------------------------------" << endl
         << " w: Toggle wireframe Mode" << endl
         << " *: Toggle full screen mode" << endl
         << " a/A : Augmente/Diminue ambiantRef" << endl
         << " z/Z : Augmente/Diminue diffuseRef" << endl
         << " e/E : Augmente/Diminue SpecularRef" << endl
         << " t/T : Augmente/Diminue le poids du modele 0 pour l’interpolation" << endl
         << " y/Y : Augmente/Diminue le poids du modele 1 pour l’interpolation" << endl
         << " u/U : Augmente/Diminue le poids du modele 2 pour l’interpolation" << endl
         << " s/S : Rotation sur x" << endl
         << " d/D : Rotation sur y" << endl
         << " f/F : Rotation sur z" << endl
         << " g/G : Rotation sur x,y,z" << endl
         << " h/H : Translate +/- sur x" << endl
         << " j/J : Translate +/- sur y" << endl
         << " k/K : Translate +/- sur z" << endl
         << " +/- : Augmente/Diminue l’offset, nX et nY" << endl
         << " x/X : Augmente/Diminue smooth" << endl
         << " c : Fait smooth Taubin" << endl
         << " v/V : Augmente/Diminue gonflement" << endl
         << " ','/'?' : Augmente/Diminue l'epaisseur sur x" << endl
         << " ';'/'.' : Augmente/Diminue l'epaisseur sur y" << endl
         << " ':'/'/' : Augmente/Diminue l'epaisseur sur z" << endl
         << " q, <esc>: Quit" << endl << endl
         << "--------------------------------------" << endl;
}

void key (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
    case '*':
        if (fullScreen == true) {
            glutReshapeWindow (SCREENWIDTH, SCREENHEIGHT);
            fullScreen = false;
        } else {
            glutFullScreen ();
            fullScreen = true;
        }
        break;
    case 'q':
    case 27:
        clear ();
        exit (0);
        break;
    case 'w':
        if( mode == Wire ){
            glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
            phongShader->bind ();
            mode = Phong;
        } else {
            glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
            phongShader->bind ();
            mode = Wire;
        }
        break;
    case 'a':
        ambientRef = ambientRef + 0.1f;
        break;
    case 'A':
        ambientRef = ambientRef - 0.1f;
        break;
    case 'z':
        diffuseRef = diffuseRef + 0.1f;
        break;
    case 'Z':
        diffuseRef = diffuseRef - 0.1f;
        break;
    case 'e':
        specularRef = specularRef + 0.1f;
        break;
    case 'E':
        specularRef = specularRef - 0.1f;
        break;
    case 't':
        interpolant0 = std::min( interpolant0 + 0.01f, 1.0f);
        updateAnimation();
        break;
    case 'T':
        interpolant0 = std::max( interpolant0 - 0.01f, 0.0f);
        updateAnimation();
        break;
    case 'y':
        interpolant1 = std::min( interpolant1 + 0.01f, 1.0f);
        updateAnimation();
        break;
    case 'Y':
        interpolant1 = std::max( interpolant1 - 0.01f, 0.0f);
        updateAnimation();
        break;
    case 'u':
        interpolant2 = std::min( interpolant2 + 0.01f, 1.0f);
        updateAnimation();
        break;
    case 'U':
        interpolant2 = std::max( interpolant2 - 0.01f, 0.0f);
        updateAnimation();
        break;
     case 's':
        anglex += 0.1f;
        if( anglex >= (float)M_PI*2.f ) anglex = 0.;
        updateAnimation();
        break;
     case 'S':
         anglex -= 0.1f;
        if( anglex <= 0.f ) anglex = (float)M_PI*2.f;
        updateAnimation();
        break;
    case 'd':
        angley += 0.1f;
        if( angley >= (float)M_PI*2.f ) angley = 0.;
        updateAnimation();
        break;
     case 'D':
         angley -= 0.1f;
        if( angley <= 0.f ) angley = (float)M_PI*2.f;
        updateAnimation();
        break;
    case 'f':
        anglez += 0.1f;
        if( anglez >= (float)M_PI*2.f ) anglez = 0.;
        updateAnimation();
        break;
    case 'F':
         anglez -= 0.1f;
        if( anglez <= 0.f ) anglez = (float)M_PI*2.f;
        updateAnimation();
        break;
    case 'g':
        anglex += 0.1f;
        angley += 0.1f;
        anglez += 0.1f;
        if( anglex >= (float)M_PI*2.f ) anglex = 0.;
        if( angley >= (float)M_PI*2.f ) angley = 0.;
        if( anglez >= (float)M_PI*2.f ) anglez = 0.;
        updateAnimation();
        break;
    case 'G':
        anglex -= 0.1f;
        angley -= 0.1f;
        anglez -= 0.1f;
        if( anglex <= 0.f ) anglex = (float)M_PI*2.f;
        if( angley <= 0.f ) angley = (float)M_PI*2.f;
        if( anglez <= 0.f ) anglez = (float)M_PI*2.f;
        updateAnimation();
        break;
    case 'h':
        offsetx += 0.01f;
        updateAnimation();
        break;
    case 'H':
        offsetx -= 0.01f;
        updateAnimation();
        break;
    case 'j':
        offsety += 0.01f;
        updateAnimation();
        break;
    case 'J':
        offsety -= 0.01f;
        updateAnimation();
        break;
    case 'k':
        offsetz += 0.01f;
        updateAnimation();
        break;
    case 'K':
        offsetz -= 0.01f;
        updateAnimation();
        break;
    case 'm':
        show_animation = !show_animation;
        break;
    case '+':
        nX += 1.0;
        nY += 1.0;
        updateAnimation();
        displayUnitSphere();
        break;
    case '-':
        nX -= 1.0;
        nY -= 1.0;
        updateAnimation();
        displayUnitSphere();
        break;
    case 'x':
        smooth(0.1f);
        break;
    case 'X':
        smooth(-0.1f);
        break;
    case 'c':
        smoothTaubin(2,0.50f,0.51f);
        break;
    case 'v':
        swell(0.01f);
        break;
    case 'V':
        swell(-0.01f);
        break;
    case ',':
        scale(0, 1.1f);
        break;
    case '?':
        scale(0, 0.9f);
        break;
    case ';':
        scale(1, 1.1f);
        break;
    case '.':
        scale(1, 0.9f);
        break;
    case ':':
        scale(2, 1.1f);
        break;
    case '/':
        scale(2, 0.9f);
        break;
    case '1':
        displayUnitCamel();
        break;
    case '2':
        displayUnitSphere();
        break;
    case '3':
        displayUnitCube();
        break;
    case '4':
        displayUnitTorus();
        break;
    case '5':
        displayUnitEmptyCylinder();
        break;
    case '6':
        displayUnitCylinder();
        break;
    case '7':
        displayUnitCone();
        break;
    case '8':
        displayUnitCylinderCone();
        break;
    case '9':
        displayUnitSupershape();
        break;
    default:
        printUsage ();
        break;
    }
    setShaderValues ();
    idle ();
}

void mouse (int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    } else {
        if (button == GLUT_LEFT_BUTTON) {
            camera.beginRotate (x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
        } else if (button == GLUT_RIGHT_BUTTON) {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        } else if (button == GLUT_MIDDLE_BUTTON) {
            if (mouseZoomPressed == false) {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }
    idle ();
}

void motion (int x, int y) {
    if (mouseRotatePressed == true)
        camera.rotate (x, y);
    else if (mouseMovePressed == true) {
        camera.move ((x-lastX)/static_cast<float>(SCREENWIDTH),
                     (lastY-y)/static_cast<float>(SCREENHEIGHT),
                     0.0);
        lastX = x;
        lastY = y;
    }
    else if (mouseZoomPressed == true) {
        camera.zoom (float (y-lastZoom)/SCREENHEIGHT);
        lastZoom = y;
    }
}

void usage () {
    printUsage ();
    exit (EXIT_FAILURE);
}



int main (int argc, char ** argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCREENWIDTH, SCREENHEIGHT);
    window = glutCreateWindow ( "REVISION");


    init ();

    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);

    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);

    phongShader->bind ();
    glutMainLoop ();
    return EXIT_SUCCESS;
}

