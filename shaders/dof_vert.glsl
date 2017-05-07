#version 150
//#extension GL_ARB_explicit_attribute_location : require
// The vertex position attribute
layout (location=0) in vec3 VertexPosition;

// We need an MVP because the plane needs to be rotated
uniform mat4 MVP;

void main() {
    // Set the position of the current vertex
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}

