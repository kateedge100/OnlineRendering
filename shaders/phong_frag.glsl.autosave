#version 150                                       // Keeping you on the bleeding edge!
#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

// Attributes passed on from the vertex shader
smooth in vec3 WSVertexPosition;
smooth in vec3 WSVertexNormal;
smooth in vec2 WSTexCoord;

// Structure for holding light parameters
struct LightInfo {
    vec4 Position; // Light position in eye coords.
    vec3 La; // Ambient light intensity
    vec3 Ld; // Diffuse light intensity
    vec3 Ls; // Specular light intensity
};

// We'll have a single light in the scene with some default values
uniform LightInfo Light = LightInfo(
            vec4(2.0, 2.0, 10.0, 1.0),   // position
            vec3(0.2, 0.2, 0.2),        // La
            vec3(1.0, 1.0, 1.0),        // Ld
            vec3(1.0, 1.0, 1.0)         // Ls
            );

// The material properties of our object
struct MaterialInfo {
    vec3 Ka; // Ambient reflectivity
    vec3 Kd; // Diffuse reflectivity
    vec3 Ks; // Specular reflectivity
    float Shininess; // Specular shininess factor
};

// The object has a material
// chrome values from devernay.free.fr/cours/opengl/materials.html

uniform MaterialInfo Material = MaterialInfo(
            vec3(0.25, 0.25, 0.25),    // Ka
            vec3(0.4, 0.4, 0.4),    // Kd
            vec3(0.774597, 0.774597,0.774597),    // Ks
           0.6                    // Shininess
            );

// This is no longer a built-in variable
out vec4 FragColor;

// colour of material
vec4 materialColor= vec4(0.38f,0.07f,0.5686f,1.0f);

void main() {
    // Calculate the normal (this is the expensive bit in Phong)
    vec3 n = normalize( WSVertexNormal );

    // Calculate the light vector
    vec3 s = normalize( vec3(Light.Position) - WSVertexPosition );

    // Calculate the view vector
    vec3 v = normalize(vec3(-WSVertexPosition));

    // Reflect the light about the surface normal
    vec3 r = reflect( -s, n );

    // Compute the light from the ambient, diffuse and specular components
    vec3 lightColor = (
            Light.La * Material.Ka +
            Light.Ld * Material.Kd * max( dot(s, n), 0.0 ) +
            Light.Ls * Material.Ks * pow( max( dot(r,v), 0.0 ), Material.Shininess ));

    // Set the output color of our current pixel
    FragColor = vec4(lightColor, 1.0) * materialColor;

}
