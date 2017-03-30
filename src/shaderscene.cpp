#include "shaderscene.h"

#include <glm/gtc/type_ptr.hpp>
#include <ngl/Obj.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>

ShaderScene::ShaderScene() : Scene() {}

/**
 * @brief ObjLoaderScene::initGL
 */
void ShaderScene::initGL() noexcept {
    // Fire up the NGL machinary (not doing this will make it crash)
    ngl::NGLInit::instance();

    // Set background colour
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);

    // enable multisampling for smoother drawing
    glEnable(GL_MULTISAMPLE);

    // Create and compile all of our shaders
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    shader->loadShader("PhongProgram","shaders/plastic_vert.glsl","shaders/plastic_frag.glsl");

    shader->loadShader("ToonProgram","shaders/metal_vert.glsl","shaders/metal_frag.glsl");

    // Load the Obj file and create a Vertex Array Object
    m_mesh.reset(new ngl::Obj("models/memoryStickPlastic.obj"));
    m_mesh->createVAO();

    // Load the Obj file and create a Vertex Array Object
    m_meshMetal.reset(new ngl::Obj("models/memoryStickMetal.obj"));
    m_meshMetal->createVAO();

    // Load the Obj file and create a Vertex Array Object
    m_meshAdaptor.reset(new ngl::Obj("models/memoryStickAdaptor.obj"));
    m_meshAdaptor->createVAO();

    // Load the Obj file and create a Vertex Array Object
    m_meshFloor.reset(new ngl::Obj("models/memoryStickFloor.obj"));
    m_meshFloor->createVAO();

}

void ShaderScene::paintGL() noexcept {
    // Clear the screen (fill with our glClearColor)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the viewport
    glViewport(0,0,m_width,m_height);

    // Use plastic shader for this draw
    ngl::ShaderLib *plasticShader=ngl::ShaderLib::instance();
    GLint pid;

    (*plasticShader)["PhongProgram"]->use();
    pid = plasticShader->getProgramID("PhongProgram");


    // Our MVP matrices
    glm::mat4 M = glm::mat4(1.0f);
    glm::mat4 MVP, MV;
    glm::mat3 N;

    // Note the matrix multiplication order as we are in COLUMN MAJOR storage
    MV = m_V * M;
    N = glm::inverse(glm::mat3(MV));
    MVP = m_P * MV;

    // Set this MVP on the GPU
    glUniformMatrix4fv(glGetUniformLocation(pid, "MVP"), //location of uniform
                       1, // how many matrices to transfer
                       false, // whether to transpose matrix
                       glm::value_ptr(MVP)); // a raw pointer to the data
    glUniformMatrix4fv(glGetUniformLocation(pid, "MV"), //location of uniform
                       1, // how many matrices to transfer
                       false, // whether to transpose matrix
                       glm::value_ptr(MV)); // a raw pointer to the data
    glUniformMatrix3fv(glGetUniformLocation(pid, "N"), //location of uniform
                       1, // how many matrices to transfer
                       true, // whether to transpose matrix
                       glm::value_ptr(N)); // a raw pointer to the data

    m_mesh->draw();


    // Use metal shader for this draw
    ngl::ShaderLib *metalShader2=ngl::ShaderLib::instance();
    GLint pid2;


    (*metalShader2)["ToonProgram"]->use();
    pid2 = metalShader2->getProgramID("ToonProgram");

    // Set this MVP on the GPU
    glUniformMatrix4fv(glGetUniformLocation(pid2, "MVP"), //location of uniform
                       1, // how many matrices to transfer
                       false, // whether to transpose matrix
                       glm::value_ptr(MVP)); // a raw pointer to the data
    glUniformMatrix4fv(glGetUniformLocation(pid2, "MV"), //location of uniform
                       1, // how many matrices to transfer
                       false, // whether to transpose matrix
                       glm::value_ptr(MV)); // a raw pointer to the data
    glUniformMatrix3fv(glGetUniformLocation(pid2, "N"), //location of uniform
                       1, // how many matrices to transfer
                       true, // whether to transpose matrix
                       glm::value_ptr(N)); // a raw pointer to the data


    m_meshMetal->draw();
    m_meshAdaptor->draw();
    //m_meshFloor->draw();




}
