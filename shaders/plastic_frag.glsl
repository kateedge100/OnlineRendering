#version 150                                      // Keeping you on the bleeding edge!
#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

// Attributes passed on from the vertex shader
smooth in vec3 WSVertexPosition;
smooth in vec3 WSVertexNormal;
smooth in vec2 WSTexCoord;

smooth in vec4 ShadowCoord;

uniform sampler2D NormalTexture;
uniform sampler2D ColourTexture;

// A texture unit for storing the 3D texture
uniform samplerCube envMap;

// Set the maximum environment level of detail (cannot be queried from GLSL apparently)
uniform int envMaxLOD = 8;

// Set our gloss map texture
uniform sampler2D glossMap;

uniform sampler2D shadowMap;
uniform sampler2D depthMap;

//uniform sampler2D m_depthTexture;

// The inverse View matrix
uniform mat4 invV;

// A toggle allowing you to set it to reflect or refract the light
uniform bool isReflect = true;

// Refractic index quiried from https://refractiveindex.info/?shelf=3d&book=plastics&page=pmma
// Specify the refractive index for refractions
uniform float refractiveIndex = 1.4906;

// The output colour which will be output to the framebuffer
layout (location=0) out vec4 FragColor;




uniform vec2 u_resolution = vec2(768, 1024);


vec2 random2(vec2 st){
    st = vec2( dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}

// Value Noise by Inigo Quilez - iq/2013
// https://www.shadertoy.com/view/lsf3WH
float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(3.0-2.0*f);

    return mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),
                     dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),
                     dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
}





// Structure for holding light parameters
struct LightInfo {
    vec4 Position; // Light position in eye coords.
    vec3 La; // Ambient light intensity
    vec3 Ld; // Diffuse light intensity
    vec3 Ls; // Specular light intensity
};

// We'll have a single light in the scene with some default values
uniform LightInfo Light = LightInfo(
            vec4(100.0, 100.0, 1.0, 1.0),   // position
            vec3(0.5, 0.5, 0.5),        // La
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
            vec3(0.1, 0.1, 0.1),    // Ka
            vec3(0.5, 0.5, 0.5),    // Kd
            vec3(0.7, 0.6,0.6),    // Ks
           0.1                    // Shininess
            );


// colour of material
vec4 materialColor= vec4(0.38f*1.2,0.07f*1.2,0.5686f*1.2,0.99f);

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

vec3 halfVector(vec3 v, vec3 s)
{
    vec3 norm = normalize(v + s);
   vec3 h = (v + s)/norm;
   return h;
}

float BeckmannDist(float m, vec3 n, vec3 h)
{
    float r1 = 1.0 / ( 4.0 * (m*m) * pow(max(dot(n,h),0.0), 4.0));
    float r2 = (max(dot(n,h),0.0) * max(dot(n,h),0.0) - 1.0) / ((m*m) * max(dot(n,h),0.0) * max(dot(n,h),0.0));
    float roughness = r1 * exp(r2);
    return roughness;

}

float GeoAtten(vec3 n, vec3 h, vec3 v, vec3 l)
{
    // geometric attenuation
    float NH2 = 2.0 * max(dot(n,h),0.0);
    float invVdotH = 1.0 / max(dot(v,h), 0.0);
    float g1 = (NH2 * max(dot(n,v),0.0)) * invVdotH;
    float g2 = (NH2 * max(dot(n,l),0.0)) * invVdotH;
    float geoAtt = min(1.0, min(g1, g2));
    return geoAtt;
}

float Schlick(vec3 v, vec3 h)
{
    // Schlick approximation
    float F0 = 0.8; // Fresnel reflectance at normal incidence
    float fresnel = pow(1.0 - max(dot(v,h),0.0), 5.0) * (1.0 - F0) + F0;
    return fresnel;
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

    // calculate half vector
   vec3 h =  halfVector(v,s);

   // roughness value (between 0 and 1)
   float m = 0.9;

   // fraction of diffuse reflection (specular reflection = 1 - k)
   float k = 0.9;

   // Beckmann distribution value
   float Beckmann = BeckmannDist(m,n,h);

   // Geometric Attentuation
   float Geo = GeoAtten(n,h,v,s);

   float Schlicks = Schlick(v,h);

   vec3 MicroFacet = vec3(Beckmann*Geo*Schlicks)/ max(dot(n,v),0.0);



    // Compute the light from the ambient, diffuse and specular components
    vec3 lightColor = (
            Light.La * Material.Ka +
            Light.Ld * Material.Kd * max( dot(s, n), 0.0 )  +
            Light.Ls * Material.Ks * pow( max( dot(r,v), 0.0 ), Material.Shininess ) * MicroFacet);






    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;
    float Noisecolor = 0.0;

    vec2 pos = vec2(st*100.0);

    Noisecolor = vec3( noise(pos)*.5+.5 ).r;

    // Determine the gloss value from our input texture, and scale it by our LOD resolution
    float gloss = (1.0 - Noisecolor) * float(envMaxLOD);

    // This call determines the current LOD value for the texture map
    vec4 colour = textureLod(envMap, lookup, gloss);

    vec3 texColor = texture(ColourTexture, WSTexCoord).rgb;

    // calculate shadows
//    float visibility = 1.0;
//    if ( texture( shadowMap, ShadowCoord.xy ).z  <  ShadowCoord.z){
//        visibility = 0.5;
//    }

//    float depth = texture( shadowMap, ShadowCoord.xy ).z;


    float
     bias = 0.005;
    float
     shade = 1.0;
    float
     depth = texture(shadowMap, ShadowCoord.xy).z;
    if
     (depth < (ShadowCoord.z - bias)) {
    shade = 0.5;
    }




     vec3 finalValue = Light.Ld * max(dot(n,s), 0.0) *  max( dot(r,v), 0.0 ) * (k + MicroFacet * (1.0 - k));


   // Set the output color of our current pixel
   //FragColor = vec4(texture(shadowMap, ShadowCoord.xy).z,0,0,1); // vec4(colorC,0,0,1);// vec4(lightColor, 1.0) * materialColor * colour;
   FragColor =  vec4(finalValue, 1.0) * materialColor;//' * colour;
//vec4(Noisecolor,Noisecolor,Noisecolor,1);
   //FragColor = vec4(gl_FragCoord.z);



}
