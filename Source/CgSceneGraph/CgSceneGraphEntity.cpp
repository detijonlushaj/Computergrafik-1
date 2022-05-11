#include "CgSceneGraphEntity.h"
#include "CgAppearance.h"

CgSceneGraphEntity::CgSceneGraphEntity()
{

}
CgSceneGraphEntity::CgSceneGraphEntity(std::vector<CgBaseRenderableObject*> objects) : m_list_of_objects(objects) {
    setCurrentTransformation(glm::mat4(1.));
    setObjectTransformation(glm::mat4(1.));
}

const std::vector<CgBaseRenderableObject*>& CgSceneGraphEntity::getListOfObjects() const {
    return m_list_of_objects;
}
void CgSceneGraphEntity::setListOfObjects(std::vector<CgBaseRenderableObject*> list_of_objects) {
    m_list_of_objects.clear();
    for (unsigned int i = 0; list_of_objects.size(); ++i) {
        m_list_of_objects.push_back(list_of_objects[i]);
    }
}

glm::mat4 CgSceneGraphEntity::getCurrentTransformation() {
    return m_current_transformation;
}
void CgSceneGraphEntity::setCurrentTransformation(glm::mat4 curren_transformation) {
    m_current_transformation = curren_transformation;
}


CgAppearance& CgSceneGraphEntity::getAppearance() {
    return m_appearance;
}
void CgSceneGraphEntity::setAppearance(CgAppearance* appearance) {
    m_appearance = *appearance;
}


const CgSceneGraphEntity& CgSceneGraphEntity::getParent() const {
    return *m_parent;
}
void CgSceneGraphEntity::setParent(CgSceneGraphEntity* parent) {
    m_parent = parent;
}


const std::vector<CgSceneGraphEntity*> CgSceneGraphEntity::getChildren() const {
    return m_children;
}

void CgSceneGraphEntity::pushChildren(CgSceneGraphEntity* child) {
    m_children.push_back(child);
}

void CgSceneGraphEntity::removeLastChild() {
    m_children.pop_back();
}

glm::mat4 CgSceneGraphEntity::getObjectTransformation() const
{
    return m_object_transformation;
}

void CgSceneGraphEntity::setObjectTransformation(const glm::mat4 &object_transformation)
{
    m_object_transformation = object_transformation;
}
