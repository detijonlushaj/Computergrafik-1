#ifndef CGSCENEGRAPH_H
#define CGSCENEGRAPH_H

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "../CgBase/CgBaseRenderableObject.h"
#include "CgAppearance.h"
#include "CgSceneGraph.h"
#include "CgBase/CgBaseRenderer.h"
#include "../CgBase/CgBaseTriangleMesh.h"
#include "CgUnityCube.h"
#include "CgRay.h"

class CgSceneGraph;

class CgSceneGraphEntity
{
public:
    CgSceneGraphEntity();
    CgSceneGraphEntity(CgBaseTriangleMesh* objects);
    ~CgSceneGraphEntity();

    CgBaseTriangleMesh* getObject();
    glm::mat4 getCurrentTransformation();
    CgAppearance& getAppearance();
    CgSceneGraphEntity* getParent();
    const std::vector<CgSceneGraphEntity*> getChildren() const;
    glm::mat4 getObjectTransformation();
    CgUnityCube* getAABB();
    CgUnityCube* getAABBAxisAlligned();

    void setObject(CgBaseTriangleMesh* object);
    void setCurrentTransformation(glm::mat4 curren_transformation);
    void setAppearance(CgAppearance* appearance);
    void setParent(CgSceneGraphEntity* parent);
    void setObjectTransformation(const glm::mat4 &object_transformation);
    void setAABB(CgUnityCube*);
    void setAABB(float, float, float, float, float, float);
    void setAABBAxisAlligned(float, float, float, float, float, float);

    void pushChildren(CgSceneGraphEntity* child);
    void removeLastChild();

private:
    CgUnityCube* m_aabb_axis_allgined;
    CgUnityCube* m_aabb;

    CgBaseTriangleMesh* m_object;
    glm::mat4 m_current_transformation;
    glm::mat4 m_object_transformation;

    CgAppearance m_appearance;

    CgSceneGraphEntity* m_parent;
    std::vector<CgSceneGraphEntity*> m_children;
};
#endif // CGSCENEGRAPH_H
