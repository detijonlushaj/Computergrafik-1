#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <stack>
#include "CgSceneGraphEntity.h"
#include "../CgBase/CgBaseRenderableObject.h"
#include "CgBase/CgBaseRenderer.h"
#include "CgBase/CgObserver.h"
#include "CgBase/CgBaseSceneControl.h"

#include "CgExampleTriangle.h"
#include "CgUnityCube.h"



class CgSceneGraphEntity;


class CgSceneGraph : public CgObserver, public CgBaseSceneControl
{
public:
    CgSceneGraph();

    void render(CgBaseRenderer* renderer);
    CgSceneGraphEntity* getRootNode();
    std::stack<glm::mat4> getModelviewMatrixStack();
    void setRootNode(CgSceneGraphEntity* root_node);
    void setModelviewMatrixStack(std::stack<glm::mat4> modelview_matrix_stack);
    void addModelviewMatrixStack(glm::mat4 modelview_matrix);
    void pushMatrix(glm::mat4 matrix);
    void popMatrix();
    void applyTransform(glm::mat4 arg);

    glm::mat4 getCurrent_transformation() const;
    void setCurrent_transformation(const glm::mat4 &current_transformation);
    glm::mat4 getTrackball_rotation() const;
    void setTrackball_rotation(const glm::mat4 &trackball_rotation);
    glm::mat4 getLookAt_matrix() const;
    void setLookAt_matrix(const glm::mat4 &lookAt_matrix);
    glm::mat4 getProj_matrix() const;
    void setProj_matrix(const glm::mat4 &proj_matrix);

    void handleEvent(CgBaseEvent* e);
    void setRenderer(CgBaseRenderer* r);
    void renderObjects();

private:
    CgSceneGraphEntity* m_root_node;
    std::stack<glm::mat4> m_modelview_matrix_stack;

    CgBaseRenderer* m_renderer;

    glm::mat4 m_current_transformation;
    glm::mat4 m_trackball_rotation;
    glm::mat4 m_lookAt_matrix;
    glm::mat4 m_proj_matrix;

    CgExampleTriangle* m_triangle;
    CgUnityCube* m_cube;


};

#endif // SCENEGRAPH_H
