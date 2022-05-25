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
    glm::vec4 getCurrentEntityOldColor();
    CgSceneGraphEntity* getCurrentEntity();
    CgSceneGraphEntity* getNextEntity();
    CgCoordSystem* getCoordSystem();

    void initializeInorderList(CgSceneGraphEntity* entity);

    void pushMatrix(glm::mat4);
    void popMatrix();

    void render(CgSceneControl* scene_control, CgSceneGraphEntity* entity);
private:
    void pushMatrix();
    void applyTransform(glm::mat4 arg);

    CgSceneGraphEntity* m_root_node;
    std::stack<glm::mat4> m_modelview_matrix_stack;

    int m_index_of_selected_gui_elem;
    glm::vec4 m_current_entity_old_color;
    std::vector<CgSceneGraphEntity*> m_inorder_scene_entities;

    CgCoordSystem* coord_system;

    std::vector<CgBaseRenderableObject*> o_all_objects;

    // all entities
    CgSceneGraphEntity* m_world;
    CgSceneGraphEntity* m_stuhlbein_ul;
        CgSceneGraphEntity* m_stuhlbein_ur;
        CgSceneGraphEntity* m_stuhlbein_ol;
        CgSceneGraphEntity* m_stuhlbein_or;
        CgSceneGraphEntity* m_stuhlplate;
        CgSceneGraphEntity* m_lehne;
        CgSceneGraphEntity* m_man;
    CgSceneGraphEntity* m_tischbein_ul;
        CgSceneGraphEntity* m_tischbein_ur;
        CgSceneGraphEntity* m_tischbein_ol;
        CgSceneGraphEntity* m_tischbein_or;
        CgSceneGraphEntity* m_tischplatte;
        CgSceneGraphEntity* m_checkerboard;
            CgSceneGraphEntity* m_b_king;
            CgSceneGraphEntity* m_b_queen;
            CgSceneGraphEntity* m_b_bishop_1;
            CgSceneGraphEntity* m_b_bishop_2;
            CgSceneGraphEntity* m_b_knight_1;
            CgSceneGraphEntity* m_b_knight_2;
            CgSceneGraphEntity* m_b_rook_1;
            CgSceneGraphEntity* m_b_rook_2;
            CgSceneGraphEntity* m_b_pawn_1;
            CgSceneGraphEntity* m_b_pawn_2;
            CgSceneGraphEntity* m_b_pawn_3;
            CgSceneGraphEntity* m_b_pawn_4;
            CgSceneGraphEntity* m_b_pawn_5;
            CgSceneGraphEntity* m_b_pawn_6;
            CgSceneGraphEntity* m_b_pawn_7;
            CgSceneGraphEntity* m_b_pawn_8;

            CgSceneGraphEntity* m_w_king;
            CgSceneGraphEntity* m_w_queen;
            CgSceneGraphEntity* m_w_bishop_1;
            CgSceneGraphEntity* m_w_bishop_2;
            CgSceneGraphEntity* m_w_knight_1;
            CgSceneGraphEntity* m_w_knight_2;
            CgSceneGraphEntity* m_w_rook_1;
            CgSceneGraphEntity* m_w_rook_2;
            CgSceneGraphEntity* m_w_pawn_1;
            CgSceneGraphEntity* m_w_pawn_2;
            CgSceneGraphEntity* m_w_pawn_3;
            CgSceneGraphEntity* m_w_pawn_4;
            CgSceneGraphEntity* m_w_pawn_5;
            CgSceneGraphEntity* m_w_pawn_6;
            CgSceneGraphEntity* m_w_pawn_7;
            CgSceneGraphEntity* m_w_pawn_8;
        CgSceneGraphEntity* m_box_plate;
            CgSceneGraphEntity* m_box_wand1;
            CgSceneGraphEntity* m_box_wand2;
            CgSceneGraphEntity* m_box_wand3;
            CgSceneGraphEntity* m_box_wand4;


    //all objs
    CgLoadObjFile*  obj_man;
    CgLoadObjFile*  obj_king;
    CgLoadObjFile*  obj_queen;
    CgLoadObjFile*  obj_bishop;
    CgLoadObjFile*  obj_knight;
    CgLoadObjFile*  obj_rook;
    CgUnityCube*    obj_cube;
    CgRotation*     obj_pawn;
};
#endif // SCENEGRAPH_H
