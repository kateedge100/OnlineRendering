#version 150                                       // Keeping you on the bleeding edge!
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

#define MAX_LIGHTS 2
uniform sampler2D NormalTexture;
uniform sampler2D ColourTexture;

uniform sampler2D shadowMap;

// Attributes passed on from the vertex shader
smooth in vec3 WSVertexPosition;
smooth in vec3 WSVertexNormal;
smooth in vec2 WSTexCoord;

smooth in vec4 ShadowCoord;

// A texture unit for storing the 3D texture
uniform samplerCube envMap;

// Set the maximum environment level of detail (cannot be queried from GLSL apparently)
uniform int envMaxLOD = 8;

// Set our gloss map texture
uniform sampler2D glossMap;

// The inverse View matrix
uniform mat4 invV;

// A toggle allowing you to set it to reflect or refract the light
uniform bool isReflect = true;

// Specify the refractive index for refractions
uniform float refractiveIndex = 2.9304;
// This is no longer a built-in variable
layout (location=0) out vec4 FragColor;

// Structure for holding light parameters
struct LightInfo {
    vec4 Position; // Light position in eye coords.
    vec3 La; // Ambient light intensity
    vec3 Ld; // Diffuse light intensity
    vec3 Ls; // Specular light intensity
};

// We'll have a single light in the scene with some default values
uniform LightInfo Light = LightInfo(
            vec4(1.0,1.0, 1.0, 1.0),   // position
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
           0.9                    // Shininess
            );


// colour of material
vec4 materialColor= vec4(0.9f,0.9f,0.9f,1.0f);

/** From http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/
  */
mat4 rotationMatrix(vec3 axis, float angle)
{
    //axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

/**
  * Rotate a vector vec by using the rotation that transforms from src to tgt.
  */
vec3 rotateVector(vec3 src, vec3 tgt, vec3 vec) {
    float angle = acos(dot(src,tgt));

    // Check for the case when src and tgt are the same vector, in which case
    // the cross product will be ill defined.
    if (angle == 0.0) {
        return vec;
    }
    vec3 axis = normalize(cross(src,tgt));
    mat4 R = rotationMatrix(axis,angle);

    // Rotate the vec by this rotation matrix
    vec4 _norm = R*vec4(vec,1.0);
    return _norm.xyz / _norm.w;
}

void main() {
    // Calculate the normal (this is the expensive bit in Phong)
    vec3 n = normalize( WSVertexNormal );

    // Calculate the view vector
    vec3 v = normalize(vec3(-WSVertexPosition));

    vec3 lookup;

    if (isReflect) {
        lookup = reflect(v,n);
    } else {
        lookup = refract(v,n,refractiveIndex);
    }

    // Extract the normal from the normal map (rescale to [-1,1]
    vec3 tgt = normalize(texture(NormalTexture, WSTexCoord).rgb * 2.0 - 1.0);

    // The source is just up in the Z-direction
    vec3 src = vec3(0.0, 0.0, 1.0);

    // Perturb the normal according to the target
    vec3 np = rotateVector(src, tgt, n);


    // Calculate the light vector
    vec3 s = normalize( vec3(Light.Position) - WSVertexPosition );


    // Reflect the light about the surface normal
    vec3 r = reflect( -s, np );

    // Compute the light from the ambient, diffuse and specular components
    vec3 lightColor = (
            Light.La * Material.Ka +
            Light.Ld * Material.Kd * max( dot(s, n), 0.0 ) +
            Light.Ls * Material.Ks * pow( max( dot(r,v), 0.0 ), Material.Shininess ));

    float
     bias = 0.005;
    float
     shade = 1.0;
    float
     depth =texture(shadowMap, ShadowCoord.xy).z;
    if
     (depth < (ShadowCoord.z - bias)) {
    shade = 0.5;
    }

    // Determine the gloss value from our input texture, and scale it by our LOD resolution
    float gloss = (1.0 - texture(glossMap, WSTexCoord*2).r) * float(envMaxLOD);

    // This call determines the current LOD value for the texture map
    vec4 colour = textureLod(envMap, lookup, gloss);

    vec3 texColor = texture(ColourTexture, WSTexCoord).rgb;

    // Set the output color of our current pixel
    FragColor = vec4(lightColor, 1.0)*materialColor*colour;

    //FragColor = vec4(gl_FragCoord.z);

}
