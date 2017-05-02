#include "shaderscene.h"

#include <glm/gtc/type_ptr.hpp>
#include <ngl/Obj.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>

/// THINGS TO DO
/// get transparency working - DONE!
/// adjust refraction for plastic - DONE!
/// add shadow map
/// add roughness
/// add noise - simplex
/// add depth of field
/// add groundplane - done!
/// add more lights
/// fix model
/// do report
/// DONE


ShaderScene::ShaderScene() : Scene() {}

/**
 * @brief ObjLoaderScene::initGL
 */
void ShaderScene::initGL() noexcept {
    // Fire up the NGL machinary (not doing this will make it crash)
    ngl::NGLInit::instance();

    // Enable 2D texturing
    glEnable(GL_TEXTURE_2D);

    // Set background colour
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);

    // enable multisampling for smoother drawing
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);

    // Allows transparency
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create and compile all of our shaders
    ngl::ShaderLib *plasticShader=ngl::ShaderLib::instance();
    plasticShader->loadShader("PlasticProgram","shaders/plastic_vert.glsl","shaders/plastic_frag.glsl");


    ngl::ShaderLib *metalShader=ngl::ShaderLib::instance();
    metalShader->loadShader("MetalProgram","shaders/metal_vert.glsl","shaders/metal_frag.glsl");


    ngl::ShaderLib *shadowShader=ngl::ShaderLib::instance();
    shadowShader->loadShader("ShadowProgram","shaders/shadow_vert.glsl","shaders/shadow_frag.glsl");


    ngl::ShaderLib *floorShader=ngl::ShaderLib::instance();
    floorShader->loadShader("FloorProgram","shaders/floor_vert.glsl","shaders/floor_frag.glsl");






    initTexture(1, m_normalTexBody, "images/normalMap.png");
    // Initialize Normal Map
    initTexture(2, m_normalTexCover, "images/CoverNormalMap.png");
    // Initialise our gloss texture map here
    initTexture(3, m_glossMapTex, "images/gloss.png");
    // Initialise our foor textur here
    initTexture(4, m_floorTex, "images/floor.jpg");



    // Initialise our environment map here at texture unit 0
    initEnvironment();

    plasticShader->use("PlasticProgram");
    // Set the active texture unit on the GPU
    GLint pid = plasticShader->getProgramID("PlasticProgram");
    glUniform1i(glGetUniformLocation(pid, "envMap"), //location of uniform
                       0); // texture unit for norma
    glUniform1i(glGetUniformLocation(pid, "NormalTexture"), //location of unifor
                       1); // texture unit for normas
    glUniform1i(glGetUniformLocation(pid, "glossMap"), //location of uniform
                                   3); // texture unit for normas




    metalShader->use("MetalProgram");

    GLint pid2 = metalShader->getProgramID("MetalProgram");
    glUniform1i(glGetUniformLocation(pid2, "envMap"), //location of uniform
                       0); // texture unit for norma
    glUniform1i(glGetUniformLocation(pid2, "NormalTexture"), //location of uniform
                       2); // texture unit for normas
    glUniform1i(glGetUniformLocation(pid2, "glossMap"), //location of uniform
                       3); // texture unit for normas


    floorShader->use("FloorProgram");
    GLint pid3 = floorShader->getProgramID("FloorProgram");
    glUniform1i(glGetUniformLocation(pid3, "floorTex"), //location of uniform
                       4); // texture unit for floor texture


    /// LOAD IN OBJECTS

    // Load the Obj file and create a Vertex Array Object
    m_meshPlastic.reset(new ngl::Obj("models/memoryStickPlastic.obj"));
    m_meshPlastic->createVAO();

    // Load the Obj file and create a Vertex Array Object
    m_meshMetal.reset(new ngl::Obj("models/memoryStickMetal.obj"));
    m_meshMetal->createVAO();

    // Load the Obj file and create a Vertex Array Object
    m_meshAdaptor.reset(new ngl::Obj("models/memoryStickAdaptor.obj"));
    m_meshAdaptor->createVAO();

    // Load the Obj file and create a Vertex Array Object
    m_meshFloor.reset(new ngl::Obj("models/memoryStickFloor.obj"));
    m_meshFloor->createVAO();

    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    prim->createTrianglePlane("plane",2,2,1,1,ngl::Vec3(0,1,0));

}



void ShaderScene::depthMap()
{
    // Generate a texture to write the FBO result to
    glGenTextures(1, &tmp);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D,tmp);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 m_width,
                 m_height,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // The depth buffer is rendered to a texture buffer too

    glGenTextures(1, &m_depthTexture);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D,m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_DEPTH_COMPONENT,
                 m_width,
                 m_height,
                 0,
                 GL_DEPTH_COMPONENT,
                 GL_UNSIGNED_BYTE,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create the frame buffer
    glGenFramebuffers(1, &m_frameBufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferName);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_depthTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tmp, 0);

    // Set the fragment shader output targets (DEPTH_ATTACHMENT is done automatically)
    GLenum drawBufs[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBufs);

    // Check it is ready to rock and roll
    CheckFrameBuffer();

    // Unbind the framebuffer to revert to default render pipeline
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

//    glGenFramebuffers(1, &m_frameBufferName);
//    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferName);

//     // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
//     glGenTextures(1, &m_depthTexture);
//     glBindTexture(GL_TEXTURE_2D, m_depthTexture);

//     // give empty image to openGL
//     glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, m_width, m_height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//     //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//     glBindTexture(GL_TEXTURE_2D, 0);



//     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_depthTexture, 0);

//     GLuint buffs[] = {GL_COLOR_ATTACHMENT0};

//     glDrawBuffers(1,buffs); // No color buffer is drawn to.

//     glBindFramebuffer(GL_FRAMEBUFFER,0);

//    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//    {
//        std::cout<<"FBO NOT WORKING!";
//    }


}


void ShaderScene::paintGL() noexcept {

    // Clear the screen (fill with our glClearColor)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Allows transparency
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up the viewport
    glViewport(0,0,m_width,m_height);






    // Our MVP matrices
    glm::mat4 M = glm::mat4(1.0f);
    glm::mat4 MVP, MV;
    glm::mat3 N;

    // Note the matrix multiplication order as we are in COLUMN MAJOR storage
    MV = m_V * M;
    N = glm::inverse(glm::mat3(MV));
    MVP = m_P * MV;

    // light position
    glm::vec3 lightDir = glm::vec3(1,1,1);
    // Centre (target) vector
    glm::vec3 target = glm::vec3(0,0,0);
    // UP vector
    glm::vec3 up = glm::vec3(0,1,0);

    //glm::mat4 depthProjection = m_P;//glm::ortho <float>(-10,10,-10,10,1,100);
    glm::mat4 depthProjection = glm::ortho <float>(-10,10,-10,10,-5,5);
    glm::mat4 depthView = glm::lookAt(lightDir, target, up);
    glm::mat4 depthModel = glm::mat4(1.0);
    glm::mat4 depthMVP = depthProjection * depthView * depthModel;

    //Bind the frame buffer (future renders go here)
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferName);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    ngl::ShaderLib *shadowShader=ngl::ShaderLib::instance();
    GLint pid;

    (*shadowShader)["ShadowProgram"]->use();
    pid = shadowShader->getProgramID("ShadowProgram");

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
    glUniformMatrix4fv(glGetUniformLocation(pid, "depthMVP"), //location of uniform
                       1, // how many matrices to transfer
                       false, // whether to transpose matrix
                       glm::value_ptr(depthMVP)); // a raw pointer to the data

    m_meshPlastic->draw();
    m_meshFloor->draw();
    m_meshMetal->draw();
    m_meshAdaptor->draw();





    //Unbind our frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,m_width,m_height);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D,tmp);

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D,m_depthTexture);


    // Use plastic shader for this draw
    ngl::ShaderLib *plasticShader=ngl::ShaderLib::instance();
    GLint pid2;

    (*plasticShader)["PlasticProgram"]->use();
    pid2 = plasticShader->getProgramID("PlasticProgram");


//    // Our MVP matrices
//    glm::mat4 M = glm::mat4(1.0f);
//    glm::mat4 MVP, MV;
//    glm::mat3 N;

    glm::mat4 B = glm::mat4(0.5,0,0,0.5,0,0.5,0,0.5,0,0,0.5,0.5,0,0,0,1);
//    glm::mat4 B = glm::mat4(
//                                0.5, 0.0, 0.0, 0.0,
//                                0.0, 0.5, 0.0, 0.0,
//                                0.0, 0.0, 0.5, 0.0,
//                                0.5, 0.5, 0.5, 1.0
//                                );

    glm::mat4 depthTransMVP = B * depthMVP;

    // Note the matrix multiplication order as we are in COLUMN MAJOR storage
    //MV = m_V * M;
    //N = glm::inverse(glm::mat3(MV));
    //MVP = m_P * MV;

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
    glUniformMatrix4fv(glGetUniformLocation(pid2, "depthTransMVP"), //location of uniform
                       1, // how many matrices to transfer
                       false, // whether to transpose matrix
                       glm::value_ptr(depthTransMVP)); // a raw pointer to the data




    glUniform1i(glGetUniformLocation(pid2, "shadowMap"), //location of uniform
                           5); // texture unit for floor texture
    glUniform1i(glGetUniformLocation(pid2, "depthMap"), //location of uniform
                           6); // texture unit for floor texture


    m_meshPlastic->draw();




    // Use metal shader for this draw
    ngl::ShaderLib *metalShader=ngl::ShaderLib::instance();
    GLint pid3;
    (*metalShader)["MetalProgram"]->use();
    pid3 = metalShader->getProgramID("MetalProgram");

    // Set this MVP on the GPU
    glUniformMatrix4fv(glGetUniformLocation(pid3, "MVP"), //location of uniform
                       1, // how many matrices to transfer
                       false, // whether to transpose matrix
                       glm::value_ptr(MVP)); // a raw pointer to the data
    glUniformMatrix4fv(glGetUniformLocation(pid3, "MV"), //location of uniform
                       1, // how many matrices to transfer
                       false, // whether to transpose matrix
                       glm::value_ptr(MV)); // a raw pointer to the data
    glUniformMatrix3fv(glGetUniformLocation(pid3, "N"), //location of uniform
                       1, // how many matrices to transfer
                       true, // whether to transpose matrix
                       glm::value_ptr(N)); // a raw pointer to the data
    glUniformMatrix4fv(glGetUniformLocation(pid3, "depthTransMVP"), //location of uniform
                       1, // how many matrices to transfer
                       false, // whether to transpose matrix
                       glm::value_ptr(depthTransMVP)); // a raw pointer to the data//m_meshPlastic->draw();

    glUniform1i(glGetUniformLocation(pid3, "shadowMap"), //location of uniform
                           5); // texture unit for floor texture
    glUniform1i(glGetUniformLocation(pid3, "depthMap"), //location of uniform
                           6); // texture unit for floor texture
  //  m_meshMetal->draw();
   // m_meshAdaptor->draw();

    // Use flooor shader for this draw
    ngl::ShaderLib *floorShader=ngl::ShaderLib::instance();
    GLint pid4;
    (*floorShader)["FloorProgram"]->use();
    pid4 = floorShader->getProgramID("FloorProgram");

    // Set this MVP on the GPU
    glUniformMatrix4fv(glGetUniformLocation(pid4, "MVP"), //location of uniform
                       1, // how many matrices to transfer
                       false, // whether to transpose matrix
                       glm::value_ptr(MVP)); // a raw pointer to the data
    glUniformMatrix4fv(glGetUniformLocation(pid4, "MV"), //location of uniform
                       1, // how many matrices to transfer
                       false, // whether to transpose matrix
                       glm::value_ptr(MV)); // a raw pointer to the data
    glUniformMatrix3fv(glGetUniformLocation(pid4, "N"), //location of uniform
                       1, // how many matrices to transfer
                       true, // whether to transpose matrix
                       glm::value_ptr(N)); // a raw pointer to the data
    glUniformMatrix4fv(glGetUniformLocation(pid4, "depthTransMVP"), //location of uniform
                       1, // how many matrices to transfer
                       false, // whether to transpose matrix
                       glm::value_ptr(depthTransMVP)); // a raw pointer to the data//m_meshPlastic->draw();


    glUniform1i(glGetUniformLocation(pid4, "shadowMap"), //location of uniform
                           5); // texture unit for floor texture
    glUniform1i(glGetUniformLocation(pid4, "depthMap"), //location of uniform
                           6); // texture unit for floor texture

//   m_meshFloor->draw();
    //MVP = glm::rotate(glm::mat4(1.0f), glm::pi<float>() * 0.5f, glm::vec3(1.0f,0.0f,0.0f));
    //glUniformMatrix4fv(glGetUniformLocation(pid, "MVP"), 1, false, glm::value_ptr(MVP));
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    //prim->draw("plane");



 glBindTexture(GL_TEXTURE_2D, 0);

}

void ShaderScene::initTexture(const GLuint& texUnit, GLuint &texId, const char *filename) {
    // Set our active texture unit
    glActiveTexture(GL_TEXTURE0 + texUnit);

    // Load up the image using NGL routine
    ngl::Image img(filename);

    // Create storage for our new texture
    glGenTextures(1, &texId);

    // Bind the current texture
    glBindTexture(GL_TEXTURE_2D, texId);

    // Transfer image data onto the GPU using the teximage2D call
    glTexImage2D (
                GL_TEXTURE_2D,    // The target (in this case, which side of the cube)
                0,                // Level of mipmap to load
                img.format(),     // Internal format (number of colour components)
                img.width(),      // Width in pixels
                img.height(),     // Height in pixels
                0,                // Border
                GL_RGB,          // Format of the pixel data
                GL_UNSIGNED_BYTE, // Data type of pixel data
                img.getPixels()); // Pointer to image data in memory

    // Set up parameters for our texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

/**
 * @brief Scene::initEnvironment in texture unit 0
 */
void ShaderScene::initEnvironment() {
    // Enable seamless cube mapping
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Placing our environment map texture in texture unit 0
    glActiveTexture (GL_TEXTURE0);

    // Generate storage and a reference for our environment map texture
    glGenTextures (1, &m_envTex);


    // Bind this texture to the active texture unit
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_envTex);

    // Now load up the sides of the cube
    initEnvironmentSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "images/sky_zneg.png");
    initEnvironmentSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "images/sky_zpos.png");
    initEnvironmentSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "images/sky_ypos.png");
    initEnvironmentSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "images/sky_yneg.png");
    initEnvironmentSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "images/sky_xneg.png");
    initEnvironmentSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X, "images/sky_xpos.png");

    // Generate mipmap levels
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // Set the texture parameters for the cube map
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLfloat anisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

//    // Set our cube map texture to on the shader so we can use it
//    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
//    shader->use(program);
//    shader->setUniform("envMap", 0);
//


}

/**
 * @brief Scene::initEnvironmentSide
 * @param texture
 * @param target
 * @param filename
 * This function should only be called when we have the environment texture bound already
 * copy image data into 'target' side of cube map
 */
void ShaderScene::initEnvironmentSide(GLenum target, const char *filename) {
    // Load up the image using NGL routine
    ngl::Image img(filename);

    // Transfer image data onto the GPU using the teximage2D call
    glTexImage2D (
      target,           // The target (in this case, which side of the cube)
      0,                // Level of mipmap to load
      img.format(),     // Internal format (number of colour components)
      img.width(),      // Width in pixels
      img.height(),     // Height in pixels
      0,                // Border
      GL_RGBA,          // Format of the pixel data
      GL_UNSIGNED_BYTE, // Data type of pixel data
      img.getPixels()   // Pointer to image data in memory
    );
}

