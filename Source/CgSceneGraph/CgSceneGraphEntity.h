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
class CgBaseRenderer;

class CgSceneGraphEntity
{
public:
    CgSceneGraphEntity();

    std::vector<CgBaseRenderableObject*> getListOfObject();
    glm::mat4 getCurrentTransformation();
    CgAppearance getAppearance();
    CgSceneGraphEntity* getParent();
    std::vector<CgSceneGraphEntity*> getChildren();
    void setListOfObjects(std::vector<CgBaseRenderableObject*> list_of_objects);
    void setCurrentTransformation(glm::mat4 curren_transformation);
    void setAppearance(CgAppearance appearance);
    void setParent(CgSceneGraphEntity* parent);
    void setChildren(std::vector<CgSceneGraphEntity*> children);
    void pushObject(CgBaseRenderableObject* object);
    void pushChildren(CgSceneGraphEntity* child);

    void iterateAllChildren_DFS(CgSceneGraph*, CgBaseRenderer*);

private:
    std::vector<CgBaseRenderableObject*> m_list_of_objects;
    glm::mat4 m_curren_transformation;
    CgAppearance m_appearance;
    CgSceneGraphEntity* m_parent;
    std::vector<CgSceneGraphEntity*> m_children;
};

#endif // CGSCENEGRAPH_H
