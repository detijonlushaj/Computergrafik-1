#ifndef CGSCENEGRAPH_H
#define CGSCENEGRAPH_H

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "../CgBase/CgBaseRenderableObject.h"
#include "CgAppearance.h"
#include "CgSceneGraph.h"
#include "CgBase/CgBaseRenderer.h"

class CgSceneGraph;

class CgSceneGraphEntity
{
public:
    CgSceneGraphEntity();
    CgSceneGraphEntity(CgBaseRenderableObject* objects);


    CgBaseRenderableObject* getObject();
    glm::mat4 getCurrentTransformation();
    CgAppearance& getAppearance();
    const CgSceneGraphEntity& getParent() const;
    const std::vector<CgSceneGraphEntity*> getChildren() const;
    glm::mat4 getObjectTransformation() const;

    void setObject(CgBaseRenderableObject* object);
    void setCurrentTransformation(glm::mat4 curren_transformation);
    void setAppearance(CgAppearance* appearance);
    void setParent(CgSceneGraphEntity* parent);
    void setObjectTransformation(const glm::mat4 &object_transformation);

    void pushChildren(CgSceneGraphEntity* child);
    void removeLastChild();
private:
    CgBaseRenderableObject* m_object;
    glm::mat4 m_current_transformation;
    glm::mat4 m_object_transformation;

    CgAppearance m_appearance;

    CgSceneGraphEntity* m_parent;
    std::vector<CgSceneGraphEntity*> m_children;
};
#endif // CGSCENEGRAPH_H
