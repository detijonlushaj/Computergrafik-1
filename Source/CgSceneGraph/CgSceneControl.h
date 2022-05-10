
#ifndef CGSCENECONTROL_H
#define CGSCENECONTROL_H

#include "CgBase/CgObserver.h"
#include "CgBase/CgBaseSceneControl.h"
#include <glm/glm.hpp>
#include "CgPolyline.h"
#include "CgLoadObjFile.h"
#include "CgSceneGraph.h"
#include "CgSceneGraphEntity.h"


class CgSceneGraph;
class CgSceneGraphEntity;
class CgBaseEvent;
class CgBaseRenderer;


class CgSceneControl : public CgObserver, public CgBaseSceneControl
{
public:
    CgSceneControl();
    ~CgSceneControl();
    void handleEvent(CgBaseEvent* e);
    void setRenderer(CgBaseRenderer* r);
    CgBaseRenderer*& getRenderer();
    void renderObjects();
    void setScene(CgSceneGraph* scene);
    void setCurrentTransformation(glm::mat4);

private:
    CgSceneGraph* m_scene;

    CgSceneGraphEntity* selected_entity;
    bool entity_selected;

    CgBaseRenderer* m_renderer;

    glm::mat4 m_current_transformation;
    glm::mat4 m_trackball_rotation;
    glm::mat4 m_lookAt_matrix;
    glm::mat4 m_proj_matrix;
    glm::mat4 m_scalemat;
};

#endif // CGSCENECONTROL_H
