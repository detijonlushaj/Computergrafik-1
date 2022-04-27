#include "CgSceneGraphEntity.h"


CgSceneGraphEntity::CgSceneGraphEntity()
{

}


void CgSceneGraphEntity::iterateAllChildren_DFS(CgSceneGraph* sceneGraph, CgBaseRenderer* renderer)
{

    //push
    sceneGraph->pushMatrix(this->getCurrentTransformation());

    //apply transformation

    // include a scenegraph into rendering
    m_curren_transformation = sceneGraph->getModelviewMatrixStack().top();


    // applyTransormation
    sceneGraph->renderObjects();


    // zeichne das aktuelle Entity
    for(unsigned int i = 0; i < this->getListOfObject().size(); i++) {
        renderer->render(this->getListOfObject().at(i));
    }

    for(unsigned int i=0;i<m_children.size();i++)
    {
        m_children[i]->iterateAllChildren_DFS(sceneGraph, renderer);
    }
    //pop
    sceneGraph->popMatrix();
}

std::vector<CgBaseRenderableObject*> CgSceneGraphEntity::getListOfObject() {return m_list_of_objects;}
glm::mat4 CgSceneGraphEntity::getCurrentTransformation() {return m_curren_transformation;}
CgAppearance CgSceneGraphEntity::getAppearance() {return m_appearance;}
CgSceneGraphEntity* CgSceneGraphEntity::getParent() {return m_parent;}
std::vector<CgSceneGraphEntity*> CgSceneGraphEntity::getChildren() {return m_children;}

void CgSceneGraphEntity::setListOfObjects(std::vector<CgBaseRenderableObject*> list_of_objects) {
    m_list_of_objects.clear();
    for (unsigned int i = 0; list_of_objects.size(); ++i) {
        m_list_of_objects.push_back(list_of_objects[i]);
    }
}

void CgSceneGraphEntity::setCurrentTransformation(glm::mat4 curren_transformation) {m_curren_transformation = curren_transformation;}
void CgSceneGraphEntity::setAppearance(CgAppearance appearance) {m_appearance = appearance;}
void CgSceneGraphEntity::setParent(CgSceneGraphEntity* parent) {m_parent = parent;}

void CgSceneGraphEntity::setChildren(std::vector<CgSceneGraphEntity*> children) {
    m_children.clear();
    for (unsigned int i = 0; children.size(); ++i) {
        m_children.push_back(children[i]);
    }
}

void CgSceneGraphEntity::pushObject(CgBaseRenderableObject* object) {m_list_of_objects.push_back(object);}
void CgSceneGraphEntity::pushChildren(CgSceneGraphEntity* child) {m_children.push_back(child);}
