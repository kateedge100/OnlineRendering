#ifndef DOFSCENE_H
#define DOFSCENE_H

// The parent class for this scene
#include "scene.h"

#include <ngl/Obj.h>

class DofScene : public Scene
{
public:
    /// An enumerated type to allow the user to select an alternative blur function
    typedef enum {BLUR_GAUSSIAN, BLUR_POISSON} BlurFilter;

    /// Construct our scene
    DofScene();

    /// Called when the scene needs to be painted
    void paintGL() noexcept;

    /// Called when the scene is to be initialised
    void initGL() noexcept;

    /// Set the depth for this DOF shader
    void setFocalDepth(const GLfloat &d) {m_focalDepth = d;}

    /// Toggle which blurring filter is active
    void toggleBlurFilter() {m_blurFilter = (m_blurFilter==BLUR_GAUSSIAN)?BLUR_POISSON:BLUR_GAUSSIAN;}

    /// Resize the window (must resize the FBO)
    void resizeGL(GLint /*width*/, GLint /*height*/) noexcept;

    void drawScene(std::string _pass);

private:
    /// Initialise the FBO
    void initFBO();

    /// Keep track of whether the FBO needs to be recreated
    bool m_isFBODirty = true;

    /// The number of objects to draw
    int m_numObjects = 5;

    /// The focal depth
    GLfloat m_focalDepth = 1.0f;

    /// A weighting of the blur radius (bit like inverse focal length)
    GLfloat m_blurRadius = 0.01f;

    /// Id's used for the frame buffer object and associated textures
    GLuint m_fboId, m_fboTextureId, m_fboDepthId;

    GLuint m_envTex, m_glossMapTex, m_normalTexBody, m_normalTexCover,m_floorTex, m_testTex;

    void initEnvironment();

    void initTexture(const GLuint&, GLuint &, const char *);

    void initEnvironmentSide(GLenum /*target*/, const char* /*filename*/);

    /// The default blur filter to use
    BlurFilter m_blurFilter = BLUR_GAUSSIAN;

    /// Set the currently desired shader subroutine on the shader (from m_blurFilter)
    void setShaderSubroutine();

    GLuint m_frameBufferName, m_depthTexture, tmp;

    std::unique_ptr<ngl::Obj> m_meshPlastic;
    std::unique_ptr<ngl::Obj> m_meshMetal;
    std::unique_ptr<ngl::Obj> m_meshAdaptor;
    std::unique_ptr<ngl::Obj> m_meshFloor;
};

#endif // DOFSCENE_H
