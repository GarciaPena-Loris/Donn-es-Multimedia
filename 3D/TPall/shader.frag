uniform float ambientRef;
uniform float diffuseRef;
uniform float specularRef;
uniform float shininess;
uniform float levels;
// Fog
uniform float fog;

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

    // Fog
    vec3 fogColor = vec3(0.5, 0.5, 0.5); // couleur du brouillard (gris)
    vec3 finalColor = mix(I.xyz, fogColor, fog);

    // Affichage du fragment
    gl_FragColor = vec4(finalColor, 1.0);

    //gl_FragColor = vec4 (I.xyz, 1);

}

