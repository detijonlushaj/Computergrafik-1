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
    CgSceneGraphEntity(std::vector<CgBaseRenderableObject*> objects);


    const std::vector<CgBaseRenderableObject*>& getListOfObjects() const;
    void setListOfObjects(std::vector<CgBaseRenderableObject*> list_of_objects);
    glm::mat4 getCurrentTransformation();
    void setCurrentTransformation(glm::mat4 curren_transformation);

    CgAppearance& getAppearance();
    void setAppearance(CgAppearance* appearance);

    const CgSceneGraphEntity& getParent() const;
    void setParent(CgSceneGraphEntity* parent);

    const std::vector<CgSceneGraphEntity*> getChildren() const;
    void pushChildren(CgSceneGraphEntity* child);
    void removeLastChild();

private:
    std::vector<CgBaseRenderableObject*> m_list_of_objects;
    glm::mat4 m_current_transformation;

    CgAppearance m_appearance;

    CgSceneGraphEntity* m_parent;
    std::vector<CgSceneGraphEntity*> m_children;
};

#endif // CGSCENEGRAPH_H
