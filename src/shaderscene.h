#ifndef SHADERSCENE_H
#define SHADERSCENE_H

// The parent class for this scene
#include "scene.h"

#include <ngl/Obj.h>

class ShaderScene : public Scene
{
public:


    ShaderScene();

    /// Called when the scene needs to be painted
    void paintGL() noexcept;

    /// Called when the scene is to be initialised
    void initGL() noexcept;

    void depthMap();



private:

    GLuint m_envTex, m_glossMapTex, m_normalTexBody, m_normalTexCover,m_floorTex, shadowMap;

    void initEnvironment(std::string program);

    void initTexture(const GLuint&, GLuint &, const char *);

    void initEnvironmentSide(GLenum /*target*/, const char* /*filename*/);



    GLuint m_frameBufferName, m_depthTexture;

    std::unique_ptr<ngl::Obj> m_meshPlastic;
    std::unique_ptr<ngl::Obj> m_meshMetal;
    std::unique_ptr<ngl::Obj> m_meshAdaptor;
    std::unique_ptr<ngl::Obj> m_meshFloor;
};

#endif // SHADERSCENE_H
