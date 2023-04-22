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

uniform float ambientRef;
uniform float diffuseRef;
uniform float specularRef;
uniform float shininess;
uniform float levels;

varying vec4 p;
varying vec3 n;

void main (void) {
    vec3 P = vec3 (gl_ModelViewMatrix * p); //Position du point à éclairer
    vec3 N = normalize (gl_NormalMatrix * n); //Normal en ce point
    vec3 V = normalize (-P); //Vecteur de vue
    
    vec4 Isa = gl_LightModel.ambient; // couleur ambiante 
    vec4 Ka = gl_FrontMaterial.ambient;
    vec4 Ia = Isa * Ka;

    vec4 I = ambientRef * Ia;

    ////////////////////////////////////////////////
    //Eclairage de Phong à calculer en utilisant -- couleur diffuse et spéculaire
    ///////////////////////////////////////////////
    // gl_LightSource[i].position.xyz Position de la lumière i
    // gl_LightSource[i].diffuse Couleur diffuse de la lumière i
    // gl_LightSource[i].specular Couleur speculaire de la lumière i
    // gl_FrontMaterial.diffuse Matériaux diffus de l'objet
    // gl_FrontMaterial.specular Matériaux speculaire de l'objet

    vec4 Kd = gl_FrontMaterial.diffuse;
    vec4 Ks = gl_FrontMaterial.specular;
    
    for (int i = 0; i < 3; i++) {
        // diffuse
        vec4 Ild = gl_LightSource[i].diffuse;
        vec4 Ils = gl_LightSource[i].specular;


        vec3 Ll = normalize(gl_LightSource[i].position.xyz - P);
        vec3 Rl = 2.0 * dot(Ll, N) * N - Ll; // 𝑅 = 2 𝑁. 𝐿 𝑁 − 𝐿
        
        float dotLN = dot(Ll, N);

        for (float l = 1.0f; l < levels; l++) {
            if (l == 1.0f) {
                if (dotLN > 1.0f && dotLN < l/levels) {
                    dotLN = l/levels;
                }
            }
            else {
                if (dotLN > (l-1.0f)/levels && dotLN < l/levels) {
                    dotLN = l/levels;
                }
            }
        }

        vec4 Id = Ild * Kd * dotLN; // 𝐼𝑑 = 𝐼𝑠𝑑 ∗ 𝐾𝑑 ∗ (𝐿. 𝑁)
        vec4 Is = Ils * Ks * pow(max(0.0, dot(Rl, V)), shininess); 

        I += diffuseRef * Id;
        I += specularRef * Is;
    }

    // Ajouter un bord noir a la forme en affichant une couleur noir lorque le produit scalaire entre la normal N est la
    // direction de vue V est inferieur a un seuil (example 0.3).

    gl_FragColor = vec4 (I.xyz, 1);

}

