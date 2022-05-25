#include "CgSceneGraphEntity.h"
#include "CgAppearance.h"

CgSceneGraphEntity::CgSceneGraphEntity()
{
    setCurrentTransformation(glm::mat4(1.));
    setObjectTransformation(glm::mat4(1.));
}
CgSceneGraphEntity::CgSceneGraphEntity(CgBaseRenderableObject* object) : m_object(object) {
    setCurrentTransformation(glm::mat4(1.));
    setObjectTransformation(glm::mat4(1.));
}

CgBaseRenderableObject* CgSceneGraphEntity::getObject() {
    return m_object;
}
void CgSceneGraphEntity::setObject(CgBaseRenderableObject* object) {
    this->m_object = object;
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
