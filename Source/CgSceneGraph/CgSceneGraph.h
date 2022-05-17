#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <vector>
#include <glm/glm.hpp>
#include <stack>
#include <iostream>
#include <string>
#include "CgBase/CgBaseRenderer.h"
#include "CgSceneGraphEntity.h"
#include "CgSceneGraph.h"
#include "CgSceneControl.h"

#include "CgExampleTriangle.h"
#include "CgUnityCube.h"
#include "CgRotation.h"
#include "CgLoadObjFile.h"
#include "CgPolyline.h"
#include "CgCoordSystem.h"

#include"../CgUtils/Functions.h"

class CgSceneGraphEntity;
class CgSceneControl;
class CgBaseRenderer;

class CgSceneGraph {
public:
    CgSceneGraph();
    ~CgSceneGraph();

    void setRenderer(CgBaseRenderer* renderer);
    void setRootNode(CgSceneGraphEntity* root);
    CgSceneGraphEntity* getRootNode();
    void initializeInorderList(CgSceneGraphEntity* entity);
    glm::vec4 getCurrentEntityOldColor();
    CgSceneGraphEntity* getCurrentEntity();
    CgSceneGraphEntity* getNextEntity();
    void render(CgSceneControl* scene_control, CgSceneGraphEntity* entity);

    CgCoordSystem* getCoordSystem();

private:
    void pushMatrix();
    void popMatrix();
    void applyTransform(glm::mat4 arg);

private:
    CgSceneGraphEntity* m_root_node;
    std::stack<glm::mat4> m_modelview_matrix_stack;

    int m_index_of_selected_gui_elem;
    glm::vec4 m_current_entity_old_color;
    std::vector<CgSceneGraphEntity*> m_inorder_scene_entities;

    CgExampleTriangle* m_triangle;
    CgUnityCube* m_cube;
    CgRotation* m_rotation;
    CgLoadObjFile* m_loadFile;
    CgPolyline* m_polyline;

    CgSceneGraphEntity* m_sun;
    CgSceneGraphEntity* m_planet1;
    CgSceneGraphEntity* m_planet2;
    CgSceneGraphEntity* m_moon1;
    CgSceneGraphEntity* m_moon2;

    CgCoordSystem* coord_system;

    //die Welt
    CgSceneGraphEntity* m_world;

    //stuhl
    CgSceneGraphEntity* m_chair;
        //human
        CgLoadObjFile* m_human;

    //tisch
    CgSceneGraphEntity* m_table;

        //Schachfigurenbrett
        CgSceneGraphEntity* m_checkerboard;
        CgLoadObjFile* m_king;
        CgLoadObjFile* m_queen;
        CgLoadObjFile* m_bishop;
        CgLoadObjFile* m_knight;
        CgLoadObjFile* m_rock;
        CgLoadObjFile* m_pawn;


};

#endif // SCENEGRAPH_H
