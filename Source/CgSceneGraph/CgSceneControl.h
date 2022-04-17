
#ifndef CGSCENECONTROL_H
#define CGSCENECONTROL_H

#include "CgBase/CgObserver.h"
#include "CgBase/CgBaseSceneControl.h"
#include <glm/glm.hpp>
#include "CgPolyline.h"
#include "CgLoadObjFile.h"

class CgBaseEvent;
class CgBaseRenderer;
class CgExampleTriangle;
class CgUnityCube;
class CgRotation;

class CgSceneControl : public CgObserver, public CgBaseSceneControl
{
public:
    CgSceneControl();
    ~CgSceneControl();
    void handleEvent(CgBaseEvent* e);
    void setRenderer(CgBaseRenderer* r);

    void renderObjects();

private:

    CgBaseRenderer* m_renderer;

    CgExampleTriangle* m_triangle;
    CgUnityCube* m_cube;
    std::vector<CgPolyline*> m_polylines;
    CgPolyline* m_polyline;
    std::vector<glm::vec3> curve;
    CgRotation* m_rotation;
    CgLoadObjFile* m_loadObj;

    glm::mat4 m_current_transformation;
    glm::mat4 m_trackball_rotation;
    glm::mat4 m_lookAt_matrix;
    glm::mat4 m_proj_matrix;
};

#endif // CGSCENECONTROL_H
