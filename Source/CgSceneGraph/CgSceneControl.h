#ifndef CGSCENECONTROL_H
#define CGSCENECONTROL_H

#include "CgBase/CgObserver.h"
#include "CgBase/CgBaseSceneControl.h"
#include <glm/glm.hpp>
#include "CgPolyline.h"
#include "CgPolyline.h"


class CgBaseEvent;
class CgBaseRenderer;
class CgExampleTriangle;
class CgUnityCube;

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

    CgUnityCube* m_cube;
    std::vector<CgPolyline*> m_polylines;

    CgPolyline* m_polyline;
    std::vector<glm::vec3> curve;

    glm::mat4 m_current_transformation;
    glm::mat4 m_trackball_rotation;
    glm::mat4 m_lookAt_matrix;
    glm::mat4 m_proj_matrix;
};

#endif // CGSCENECONTROL_H
