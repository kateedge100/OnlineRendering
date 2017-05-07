#version 150                                       // Keeping you on the bleeding edge!
//#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

// The modelview and projection matrices are no longer given in OpenGL 4.2

uniform mat4 depthMVP;

uniform mat4 MVP;

// The vertex position attribute
layout (location=0) in vec3 VertexPosition;



void main() {  	      

    // Compute the position of the vertex
    gl_Position = depthMVP * vec4(VertexPosition,1.0);
}
