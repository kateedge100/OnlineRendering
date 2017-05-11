#include "shaderScene.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <ngl/Obj.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>

/**
 * @brief DofScene::DofScene
 */
DofScene::DofScene() : Scene() {
}

void DofScene::resizeGL(GLint width, GLint height) noexcept {
    Scene::resizeGL(width,height);
    m_isFBODirty = true;
}

/**
 * @brief DofScene::initGL
 */
void DofScene::initGL() noexcept {
    // Fire up the NGL machinary (not doing this will make it crash)
    ngl::NGLInit::instance();

    // Set background colour
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

    // Enable 2D texturing
    glEnable(GL_TEXTURE_2D);

    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);

    // enable multisampling for smoother drawing
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);

    // Allows transparency
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create and compile the vertex and fragment shader
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    // Create the basic shader used to render the scene with Gouraud shading
    shader->loadShader("GouraudProgram",              // Name of program
                       "../common/shaders/gouraud_vert.glsl",   // Vertex shader
                       "../common/shaders/gouraud_frag.glsl");  // Fragment shader

    // Create the depth of field shader program, which combines pixels in the fragment shader
    shader->loadShader("DofProgram",                // Name of program
                       "shaders/dof_vert.glsl",     // Vertex shader
                       "shaders/dof_frag.glsl");    // Fragment shader

    shader->loadShader("PlasticProgram",                // Name of program
                       "shaders/plastic_vert.glsl",     // Vertex shader
                       "shaders/plastic_frag.glsl");    // Fragment shader


    shader->loadShader("MetalProgram",
                       "shaders/metal_vert.glsl",
                       "shaders/metal_frag.glsl");

    shader->loadShader("FloorProgram",
                       "shaders/floor_vert.glsl",
                       "shaders/floor_frag.glsl");

    shader->loadShader("ShadowProgram",
                       "shaders/shadow_vert.glsl",
                       "shaders/shadow_frag.glsl");

    initTexture(3, m_normalTexBody, "images/normalMap.png");
    // Initialize Normal Map
    initTexture(4, m_normalTexCover, "images/CoverNormalMap.png");
    // Initialise our gloss texture map here
    initTexture(5, m_glossMapTex, "images/gloss.png");
    // Initialise our foor textur here
    initTexture(6, m_floorTex, "images/woodTexture.jpg");

    // Initialise our environment map here at texture unit 0
    initEnvironment();

    shader->use("PlasticProgram");
    // Set the active texture unit on the GPU
    GLint pid = shader->getProgramID("PlasticProgram");
    glUniform1i(glGetUniformLocation(pid, "envMap"), //location of uniform
                       0); // texture unit for norma
    glUniform1i(glGetUniformLocation(pid, "NormalTexture"), //location of unifor
                       3); // texture unit for normas
    glUniform1i(glGetUniformLocation(pid, "glossMap"), //location of uniform
                                   5); // texture unit for normas




    shader->use("MetalProgram");
    GLint pid2 = shader->getProgramID("MetalProgram");
    glUniform1i(glGetUniformLocation(pid2, "envMap"), //location of uniform
                       0); // texture unit for norma
    glUniform1i(glGetUniformLocation(pid2, "NormalTexture"), //location of uniform
                       4); // texture unit for normas
    glUniform1i(glGetUniformLocation(pid2, "glossMap"), //location of uniform
                       5); // texture unit for normas


    shader->use("FloorProgram");
    GLint pid3 = shader->getProgramID("FloorProgram");
    glUniform1i(glGetUniformLocation(pid3, "floorTex"), //location of uniform
                       6); // texture unit for normas
    glUniform1i(glGetUniformLocation(pid3, "shadowMap"), //location of uniform
                       7); // texture unit for normas


    shader->use("DofProgram");
    GLint pid4 = shader->getProgramID("DofProgram");
    glUniform1i(glGetUniformLocation(pid4, "colourTex"), 1);
    glUniform1i(glGetUniformLocation(pid4, "depthTex"), 2);



    // Load the Obj file and create a Vertex Array Object
    m_meshPlastic.reset(new ngl::Obj("model/memoryStickPlastic.obj"));
    m_meshPlastic->createVAO();

    // Load the Obj file and create a Vertex Array Object
    m_meshMetal.reset(new ngl::Obj("model/memoryStickMetal.obj"));
    m_meshMetal->createVAO();

    // Load the Obj file and create a Vertex Array Object
    m_meshAdaptor.reset(new ngl::Obj("model/memoryStickAdaptor.obj"));
    m_meshAdaptor->createVAO();

    // Load the Obj file and create a Vertex Array Object
    m_meshFloor.reset(new ngl::Obj("model/memoryStickFloor.obj"));
    m_meshFloor->createVAO();



    // Create a screen oriented plane
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    prim->createTrianglePlane("plane",2,2,1,1,ngl::Vec3(0,1,0));   
}

/**
 * @brief DofScene::initFBO
 */
void DofScene::initFBO() {
    // First delete the FBO if it has been created previously
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)==GL_FRAMEBUFFER_COMPLETE) {
        glDeleteTextures(1, &m_fboTextureId);
        glDeleteTextures(1, &m_fboDepthId);
        glDeleteFramebuffers(1, &m_fboId);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Generate a texture to write the FBO result to
    glGenTextures(1, &m_fboTextureId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_fboTextureId);
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
    glGenTextures(1, &m_fboDepthId);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_fboDepthId);
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
    glGenFramebuffers(1, &m_fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboTextureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_fboDepthId, 0);

    // Set the fragment shader output targets (DEPTH_ATTACHMENT is done automatically)
    GLenum drawBufs[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBufs);

    // Check it is ready to rock and roll
    CheckFrameBuffer();

    // Unbind the framebuffer to revert to default render pipeline
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void DofScene::drawScene(std::string _pass)
{
    /// SHADERS GO HERE

    // Use our shader for this draw
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();


    // Our MVP matrices
    glm::mat4 M = glm::mat4(1.0f);
    glm::mat4 MVP, MV;
    glm::mat3 N;

    // Translate in depth and x to get a cascade effect
    //glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(float(0), 0.0f, float(0)));



    // Note the matrix multiplication order as we are in COLUMN MAJOR storage
    MVP = m_P * m_V * M;
    MV = m_V * M;
    N = glm::inverse(glm::mat3(MV));

    //    // light position
        glm::vec3 lightDir = glm::vec3(1,4,2);
        // Centre (target) vector
        glm::vec3 target = glm::vec3(0,0,0);
        // UP vector
        glm::vec3 up = glm::vec3(0,1,0);

        glm::mat4 depthProjection = glm::ortho <float>(-10,10,-10,10,-10,10);
        //glm::mat4 depthProjection = glm::perspective<float>(45.0f, 1.0f, 1.0f, 100.0f);
        glm::mat4 depthView = glm::lookAt(lightDir, target, up);
        glm::mat4 depthModel = glm::mat4(1.0);
        glm::mat4 depthMVP =  depthProjection *  depthView * depthModel;

        // if shadow pass use shadow shader
    if(_pass == "shadow")
    {


            ngl::ShaderLib *shadowShader=ngl::ShaderLib::instance();
            GLint pid;

            (*shadowShader)["ShadowProgram"]->use();
            pid = shadowShader->getProgramID("ShadowProgram");

            // Set this MVP on the GPU
            glUniformMatrix4fv(glGetUniformLocation(pid, "depthMVP"), //location of uniform
                               1, // how many matrices to transfer
                               false, // whether to transpose matrix
                               glm::value_ptr(depthMVP)); // a raw pointer to the data

            m_meshPlastic->draw();
            m_meshFloor->draw();


            m_meshMetal->draw();
            m_meshAdaptor->draw();


    }



    // else render scene as normal
    else
    {
//        glm::mat4 B = glm::mat4(0.5,0,0,0.5,
//                                0,0.5,0,0.5,
//                                0,0,0.5,0.5
//                                ,0,0,0,1);

            glm::mat4 B = glm::mat4(
                                        0.5, 0.0, 0.0, 0.0,
                                        0.0, 0.5, 0.0, 0.0,
                                        0.0, 0.0, 0.5, 0.0,
                                        0.5, 0.5, 0.5, 1.0
                                        );

        glm::mat4 depthTransMVP = B * depthMVP;

   (*shader)["PlasticProgram"]->use();

    // Store the program id
    GLint pid = shader->getProgramID("PlasticProgram");


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
        glUniformMatrix4fv(glGetUniformLocation(pid, "depthTransMVP"), //location of uniform
                           1, // how many matrices to transfer
                           false, // whether to transpose matrix
                           glm::value_ptr(depthTransMVP)); // a raw pointer to the data//m_meshPlastic->draw();

        //glUniform3fv(glGetUniformLocation(pid, "Material.Kd"), 1, glm::value_ptr(rgb));

        // Draw a teapot primitive
        //prim->draw("teapot");
        m_meshPlastic->draw();

        GLint pid2;
        (*shader)["MetalProgram"]->use();
        pid2 = shader->getProgramID("MetalProgram");

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
                           glm::value_ptr(depthTransMVP)); // a raw pointer to the data//m_meshPlastic->draw();


        m_meshMetal->draw();
        m_meshAdaptor->draw();

        (*shader)["FloorProgram"]->use();
        GLint pid3;
        pid3 = shader->getProgramID("FloorProgram");

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
        glUniformMatrix4fv(glGetUniformLocation(pid3, "depthTransMVP"), //location of uniform
                           1, // how many matrices to transfer
                           false, // whether to transpose matrix
                           glm::value_ptr(depthTransMVP)); // a raw pointer to the data//m_meshPlastic->draw();
        glUniformMatrix4fv(glGetUniformLocation(pid3, "depthMVP"), //location of uniform
                           1, // how many matrices to transfer
                           false, // whether to transpose matrix
                           glm::value_ptr(depthMVP)); // a raw pointer to the data//m_meshPlastic->draw();



       m_meshFloor->draw();
    }

}

/**
 * @brief DofScene::paintGL
 * This is a two stage render process: firstly we render a bunch of teapots into our FBO, which
 * writes the colour and depth buffer areas to the textures m_fboTextureId and m_fboDepthId.
 * In the second render pass we bind these two textures and draw a screen aligned plane with
 * these textures applied. The depth of field effect is made by combining these two stages on
 * the fragment shader.
 */
void DofScene::paintGL() noexcept {
    // Check if the FBO needs to be recreated. This occurs after a resize.
    if (m_isFBODirty) {
        initFBO();
        m_isFBODirty = false;
    }
glViewport(0,0,m_width,m_height);
    // SHADOW MAP PASS
// ----------------------------------------------------------------------------------------

//        // Bind the FBO to specify an alternative render target
//        glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);

//        // Set up the viewport
//        glViewport(0,0,m_width,m_height);

//        // Clear the screen (fill with our glClearColor)
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        // SHADOW MAP PASS
//        drawScene("shadow");



//        // Unbind our FBO
//       glBindFramebuffer(GL_FRAMEBUFFER,0);

//        // Find the depth of field shader
//        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        glViewport(0,0,m_width,m_height);

//        // Now bind our rendered image which should be in the frame buffer for the next render pass
//        glActiveTexture(GL_TEXTURE7);
//        glBindTexture(GL_TEXTURE_2D, m_fboDepthId);



// ----------------------------------------------------------------------------------------


    // DEPTH MAP PASS
//------------------------------------------------------------------------------------------
    // Bind the FBO to specify an alternative render target
    //glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);

     //Set up the viewport
    glViewport(0,0,m_width,m_height);

    // Clear the screen (fill with our glClearColor)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // DEPTH MAP PASS
    drawScene("depth");

    // SHADERS END!!

    // Unbind our FBO
    //glBindFramebuffer(GL_FRAMEBUFFER,0);

    // Find the depth of field shader
    //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,m_width,m_height);

    // Now bind our rendered image which should be in the frame buffer for the next render pass
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_fboTextureId);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_fboDepthId);


//-----------------------------------------------------------------------------------------------------







    // DOF PASS
//------------------------------------------------------------------------------------------


//    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
//    (*shader)["DofProgram"]->use();
//    GLuint pid = shader->getProgramID("DofProgram");

////    glUniform1i(glGetUniformLocation(pid, "colourTex"), 1);
////    glUniform1i(glGetUniformLocation(pid, "depthTex"), 2);
//    glUniform1f(glGetUniformLocation(pid, "focalDepth"), m_focalDepth);
//    glUniform1f(glGetUniformLocation(pid, "blurRadius"), m_blurRadius);
//    glUniform2f(glGetUniformLocation(pid, "windowSize"), m_width, m_height);
//    setShaderSubroutine();

//    glm::mat4 MVP;

//    // Translate in depth and x to get a cascade effect
//    glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(float(0), 0.0f, float(0)));

//    // Note the matrix multiplication order as we are in COLUMN MAJOR storage
//    MVP = m_P * m_V * M;

//    MVP = glm::rotate(glm::mat4(1.0f), glm::pi<float>() * 0.5f, glm::vec3(1.0f,0.0f,0.0f));
//    glUniformMatrix4fv(glGetUniformLocation(pid, "MVP"), 1, false, glm::value_ptr(MVP));

//    // Grab and instance of the VAO primitives path
//    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
//    prim->draw("plane");

    //-----------------------------------------------------------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * @brief DofScene::setShaderSubroutine
 * GLSL handles subroutines very badly. Each shader can have many subroutines, and these
 * are all set at once using an array with the indices of each of the subroutines to use.
 */
void DofScene::setShaderSubroutine() {
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["DofProgram"]->use();
    GLint pid = shader->getProgramID("DofProgram");

    // Determine the different subroutine indices for our blur filters
    GLuint GaussianFilter = glGetSubroutineIndex(pid, GL_FRAGMENT_SHADER, "GaussianFilter");
    GLuint PoissonFilter = glGetSubroutineIndex(pid, GL_FRAGMENT_SHADER, "PoissonFilter");

    // Technically these must all be set, but we should only have 1
    GLuint blurFilter;
    switch(m_blurFilter) {
    case BLUR_POISSON:
        blurFilter = PoissonFilter;
        break;
    default:
        blurFilter = GaussianFilter;
        break;
    }

    // Set the subroutine on the shader
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &blurFilter);
}

void DofScene::initTexture(const GLuint& texUnit, GLuint &texId, const char *filename) {
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
void DofScene::initEnvironment() {
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
void DofScene::initEnvironmentSide(GLenum target, const char *filename) {
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

