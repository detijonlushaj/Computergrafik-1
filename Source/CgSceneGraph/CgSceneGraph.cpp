#include "CgSceneGraph.h"

CgSceneGraph::CgSceneGraph()
{
    m_triangle   =nullptr;
    m_cube       =nullptr;
    m_rotation   =nullptr;
    m_loadFile   =nullptr;
    m_polyline   =nullptr;

    m_sun       =nullptr;
    m_planet1   =nullptr;
    m_planet2   =nullptr;
    m_moon1     =nullptr;
    m_moon2     =nullptr;
    coord_system=nullptr;

    m_index_of_selected_gui_elem = -1;
    std::vector<CgBaseRenderableObject*> objects;

    m_cube = new CgUnityCube(Functions::getId());
    objects.push_back(m_cube);

    // Initialize sun
    m_sun = new CgSceneGraphEntity(objects);
    m_sun->setAppearance(new CgAppearance());
    m_sun->getAppearance().setObjectColor(glm::vec4(255.0, 255.0, 255.0, 1.0));
    m_modelview_matrix_stack.push(m_sun->getCurrentTransformation());

    // Initialize planet 1
    m_planet1 = new CgSceneGraphEntity(objects);
    m_planet1->setAppearance(new CgAppearance());
    m_planet1->getAppearance().setObjectColor(glm::vec4(255.0, 255.0, 255.0, 1.0));
    m_planet1->setCurrentTransformation(glm::mat4(glm::vec4(0.5, 0.0, 0.0, 0.0),
                                                  glm::vec4(0.0, 0.5, 0.0, 0.0),
                                                  glm::vec4(0.0, 0.0, 0.5, 0.0),
                                                  glm::vec4(1.0, 1.0, 0.0, 1.0)));
    m_planet1->setParent(m_sun);

    // Initialize planet 2
    m_planet2 = new CgSceneGraphEntity(objects);
    m_planet2->setCurrentTransformation(glm::mat4(glm::vec4(0.5, 0.0, 0.0, 0.0),
                                                  glm::vec4(0.0, 0.5, 0.0, 0.0),
                                                  glm::vec4(0.0, 0.0, 0.5, 0.0),
                                                  glm::vec4(0.0, -1.0, -1.0, 1.0)));
    m_planet2->setAppearance(new CgAppearance());
    m_planet2->getAppearance().setObjectColor(glm::vec4(255.0, 255.0, 255.0, 1.0));
    m_planet2->setParent(m_sun);

    // Initialize moon 1
    m_moon1 = new CgSceneGraphEntity(objects);
    m_moon1->setCurrentTransformation(glm::mat4(glm::vec4(0.3, 0.0, 0.0, 0.0),
                                                glm::vec4(0.0, 0.3, 0.0, 0.0),
                                                glm::vec4(0.0, 0.0, 0.3, 0.0),
                                                glm::vec4(0.0, 1.0, 0.0, 1.0)));
    m_moon1->setAppearance(new CgAppearance());
    m_moon1->getAppearance().setObjectColor(glm::vec4(255.0, 255.0, 255.0, 1.0));
    m_moon1->setParent(m_planet1);

    // Initialize moon 2
    m_moon2 = new CgSceneGraphEntity(objects);
    m_moon2->setCurrentTransformation(glm::mat4(glm::vec4(0.4, 0.0, 0.0, 0.0),
                                                glm::vec4(0.0, 0.4, 0.0, 0.0),
                                                glm::vec4(0.0, 0.0, 0.4, 0.0),
                                                glm::vec4(0.0, -1.0, 0.0, 1.0)));
    m_moon2->setAppearance(new CgAppearance());
    m_moon2->getAppearance().setObjectColor(glm::vec4(255.0, 255.0, 255.0, 1.0));
    m_moon2->setParent(m_planet2);

    // Children of planet 1 and planet 2
    m_planet1->pushChildren(m_moon1);
    m_planet2->pushChildren(m_moon2);

    // Children of sun
    m_sun->pushChildren(m_planet1);
    m_sun->pushChildren(m_planet2);

    this->setRootNode(m_sun);

    m_sun = new CgSceneGraphEntity(objects);


   initializeInorderList(m_root_node);

   // coordination system
   std::vector<CgPolyline*> polylines;
   std::vector<glm::vec3> vertices;
   vertices.push_back(glm::vec3(0.0, 0.0, 0.0));
   vertices.push_back(glm::vec3(1.0, 0.0, 0.0));
   polylines.push_back(new CgPolyline(Functions::getId(), vertices));

   vertices.pop_back();
   vertices.push_back(glm::vec3(0.0, 1.0, 0.0));
   polylines.push_back(new CgPolyline(Functions::getId(), vertices));

   vertices.pop_back();
   vertices.push_back(glm::vec3(0.0, 0.0, 1.0));
   polylines.push_back(new CgPolyline(Functions::getId(), vertices));
   coord_system = new CgCoordSystem(polylines);
}

CgSceneGraph::~CgSceneGraph() {
    if (m_sun!=NULL) {
        for (unsigned int i=0; i<m_sun->getListOfObjects().size(); ++i) {
            delete m_sun->getListOfObjects()[i];
        }
    }
    if (m_planet1!=NULL) {
        for ( unsigned int i=0; i<m_planet1->getListOfObjects().size(); ++i) {
            delete m_planet1->getListOfObjects()[i];
        }
    }
    if (m_planet2!=NULL) {
        for (unsigned int i=0; i<m_planet2->getListOfObjects().size(); ++i) {
            delete m_planet2->getListOfObjects()[i];
        }
    }
    if (m_moon1!=NULL) {
        for ( unsigned int i=0; i<m_moon1->getListOfObjects().size(); ++i) {
            delete m_moon1->getListOfObjects()[i];
        }
    }
    if (m_moon2!=NULL) {
        for ( unsigned int i=0; i<m_moon2->getListOfObjects().size(); ++i) {
            delete m_moon2->getListOfObjects()[i];
        }
    }
}

CgSceneGraphEntity* CgSceneGraph::getCurrentEntity() {
    if (m_index_of_selected_gui_elem != -1) {
        return m_inorder_scene_entities[m_index_of_selected_gui_elem];
    }
    return NULL;
}
glm::vec4 CgSceneGraph::getCurrentEntityOldColor() {
    return m_current_entity_old_color;
}

CgSceneGraphEntity* CgSceneGraph::getNextEntity() {
    // if iterated trough all gui elem then reset index
    if ((int)m_index_of_selected_gui_elem == (int)(m_inorder_scene_entities.size()-1)) {
        m_index_of_selected_gui_elem = -1;
    }
    m_index_of_selected_gui_elem++;
    m_current_entity_old_color = m_inorder_scene_entities[m_index_of_selected_gui_elem]->
            getAppearance().getObjectColor();
    return m_inorder_scene_entities[m_index_of_selected_gui_elem];
}

void CgSceneGraph::setRenderer(CgBaseRenderer* renderer) {
    if (m_sun!=NULL) {
        for (unsigned int i=0; i<m_sun->getListOfObjects().size(); ++i)  {
            renderer->init(m_sun->getListOfObjects()[i]);
        }
        for (unsigned int i=0; i<coord_system->getCoordSystem().size(); ++i) {
            renderer->init(coord_system->getCoordSystem()[i]);
        }
    }

}

void CgSceneGraph::pushMatrix() {
    m_modelview_matrix_stack.push(m_modelview_matrix_stack.top());
}

void CgSceneGraph::popMatrix() {
    m_modelview_matrix_stack.pop();
}

void CgSceneGraph::applyTransform(glm::mat4 arg) {
    m_modelview_matrix_stack.top()*=arg;
}

void CgSceneGraph::setRootNode(CgSceneGraphEntity* root) {
    m_root_node = root;
}

CgSceneGraphEntity* CgSceneGraph::getRootNode() {
    return m_root_node;
}

void CgSceneGraph::initializeInorderList(CgSceneGraphEntity* entity) {
    m_inorder_scene_entities.push_back(entity);
    for (unsigned int i=0; i<entity->getChildren().size(); ++i) {
        initializeInorderList(entity->getChildren()[i]);
    }
}

void CgSceneGraph::render(CgSceneControl* scene_control, CgSceneGraphEntity* entity) {
    scene_control->setCurrentTransformation(m_modelview_matrix_stack.top()*entity->getObjectTransformation());
    scene_control->getRenderer()->setUniformValue("mycolor", entity->getAppearance().getObjectColor());

    // render objects of the group
    for (unsigned int i=0; i < entity->getListOfObjects().size(); ++i) {
        scene_control->getRenderer()->render(entity->getListOfObjects()[i]);
    }

    // iterate through children recursive
    for (unsigned int i=0; i < entity->getChildren().size(); ++i) {
        pushMatrix();
        applyTransform(entity->getChildren()[i]->getCurrentTransformation());
        render(scene_control, entity->getChildren()[i]);
        m_modelview_matrix_stack.pop();
    }

}

CgCoordSystem* CgSceneGraph::getCoordSystem() {
    return coord_system;
}
