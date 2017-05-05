#ifndef SHADERSCENE_H
#define SHADERSCENE_H

// The parent class for this scene

#include "scene.h"

#include <ngl/Obj.h>

class ShaderScene : public Scene
{
public:

        /// An enumerated type to allow the user to select an alternative blur function
        typedef enum {BLUR_GAUSSIAN, BLUR_POISSON} BlurFilter;

    ShaderScene();

    /// Called when the scene needs to be painted
    void paintGL() noexcept;

    /// Called when the scene is to be initialised
    void initGL() noexcept;

    void depthMap();

    void backgroundCube();

    void setShaderSubroutine();



private:

    GLuint m_envTex, m_glossMapTex, m_normalTexBody, m_normalTexCover,m_floorTex, m_testTex;

    void initEnvironment();

    void initTexture(const GLuint&, GLuint &, const char *);

    void initEnvironmentSide(GLenum /*target*/, const char* /*filename*/);

    /// Keep track of whether the FBO needs to be recreated
    bool m_isFBODirty = true;



    GLuint m_frameBufferName, m_depthTexture, tmp;

    std::unique_ptr<ngl::Obj> m_meshPlastic;
    std::unique_ptr<ngl::Obj> m_meshMetal;
    std::unique_ptr<ngl::Obj> m_meshAdaptor;
    std::unique_ptr<ngl::Obj> m_meshFloor;



    /// The focal depth
    GLfloat m_focalDepth = 1.0f;

    /// A weighting of the blur radius (bit like inverse focal length)
    GLfloat m_blurRadius = 0.01f;

    /// Id's used for the frame buffer object and associated textures
    GLuint m_fboId, m_fboTextureId, m_fboDepthId;

    /// The default blur filter to use
    BlurFilter m_blurFilter = BLUR_GAUSSIAN;
};

#endif // SHADERSCENE_H
