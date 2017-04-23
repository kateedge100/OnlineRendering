#version 150                                       // Keeping you on the bleeding edge!
#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_separate_shader_objects : require


// The output colour which will be output to the framebuffer
layout (location=0) out float fragmentdepth;
uniform sampler2D depthTexture;


void main() {

    // Use the following shader for the correct value
    fragmentdepth = gl_FragCoord.z;
}




