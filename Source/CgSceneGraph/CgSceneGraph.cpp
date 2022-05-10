#include "CgSceneGraph.h"

#include "CgSceneControl.h"
#include "CgBase/CgEnums.h"
#include "CgEvents/CgMouseEvent.h"
#include "CgEvents/CgKeyEvent.h"
#include "CgEvents/CgWindowResizeEvent.h"
#include "CgEvents/CgLoadObjFileEvent.h"
#include "CgEvents/CgTrackballEvent.h"
#include "CgBase/CgBaseRenderer.h"
#include "CgEvents/CgColorChangeEvent.h"
#include "CgEvents/CgLaneRiesenfeldEvent.h"
#include "CgEvents/CgRotationEvent.h"
#include "CgExampleTriangle.h"
#include "CgUnityCube.h"
#include "CgPolyline.h"
#include "CgRotation.h"
#include "CgLoadObjFile.h"
#include "CgAppearance.h"
#include "../CgUtils/Functions.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "CgUtils/ObjLoader.h"
#include <string>
#include <cmath>

CgSceneGraph::CgSceneGraph()
{
    m_triangle      =nullptr;
    m_cube          =nullptr;

    m_current_transformation    =glm::mat4(1.);
    m_lookAt_matrix             =glm::lookAt(glm::vec3(0.0,0.0,1.0),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));
    m_proj_matrix               =glm::mat4x4(glm::vec4(1.792591, 0.0, 0.0, 0.0), glm::vec4(0.0, 1.792591, 0.0, 0.0), glm::vec4(0.0, 0.0, -1.0002, -1.0), glm::vec4(0.0, 0.0, -0.020002, 0.0));
    m_trackball_rotation        =glm::mat4(1.);

    m_triangle = new CgExampleTriangle(Functions::getId());
    m_cube= new CgUnityCube(Functions::getId());

    float a = 1.5;
    glm::mat4 matrix_cube = { glm::vec4( 1.0,  0.0,  0.0,  0.0),
                              glm::vec4( 0.0,  1.0,  0.0,  0.0),
                              glm::vec4( 0.0,  0.0,  1.0,  0.0),
                              glm::vec4(   a,    a,    a,  1.0)};

    float b = 1.0;
    glm::mat4 matrix_triangle1 = { glm::vec4(   b,  0.0,  0.0,  0.0),
                                   glm::vec4( 0.0,    b,  0.0,  0.0),
                                   glm::vec4( 0.0,  0.0,    b,  0.0),
                                   glm::vec4( 0.0,  0.0,  0.0,  1.0)};
    float c = -1.5;
    glm::mat4 matrix_triangle2 = { glm::vec4( 1.0,  0.0,  0.0,  0.0),
                                   glm::vec4( 0.0,  1.0,  0.0,  0.0),
                                   glm::vec4( 0.0,  0.0,  1.0,  0.0),
                                   glm::vec4(   c,    c,    c,  1.0)};

    m_root_node = new CgSceneGraphEntity();
    m_root_node->pushObject(m_cube);
    m_root_node->setCurrentTransformation(glm::mat4(1.0));

    m_root_node->pushChildren(new CgSceneGraphEntity());
    m_root_node->getChildren().at(0)->pushObject(m_triangle);
    m_root_node->getChildren().at(0)->setCurrentTransformation(matrix_triangle1);

    m_root_node->pushChildren(new CgSceneGraphEntity());
    m_root_node->getChildren().at(1)->pushObject(m_triangle);
    m_root_node->getChildren().at(1)->setCurrentTransformation(matrix_triangle2);

    m_root_node->pushChildren(new CgSceneGraphEntity());
    m_root_node->getChildren().at(2)->pushObject(m_cube);
    m_root_node->getChildren().at(2)->setCurrentTransformation(matrix_cube);

    m_root_node->getChildren().at(2)->setAppearance(new CgAppearance());
    m_root_node->getChildren().at(2)->getApperance().setObjectColor(glm::vec4(1.0, 66.0, 200.0, 1.0));
}


void CgSceneGraph::setRenderer(CgBaseRenderer *r)
{
    m_renderer=r;
    m_renderer->setSceneControl(this);

    //set Color in the beginn of the Rendering - removed form rederObjects()!
    m_renderer->setUniformValue("mycolor",glm::vec4(0.0,1.0,0.0,1.0));

    if(m_triangle!=NULL)
        m_renderer->init(m_triangle);

    if(m_cube!=NULL)
        m_renderer->init(m_cube);
}

void CgSceneGraph::renderObjects()
{
    // Materialeigenschaften setzen
    // sollte ja eigentlich pro Objekt unterschiedlich sein können, naja bekommen sie schon hin....

    m_renderer->setUniformValue("matDiffuseColor"   ,glm::vec4(0.35,0.31,0.09,1.0));
    m_renderer->setUniformValue("lightDiffuseColor" ,glm::vec4(1.0,1.0,1.0,1.0));

    m_renderer->setUniformValue("matAmbientColor"   ,glm::vec4(0.25,0.22,0.06,1.0));
    m_renderer->setUniformValue("lightAmbientColor" ,glm::vec4(1.0,1.0,1.0,1.0));

    m_renderer->setUniformValue("matSpecularColor"  ,glm::vec4(0.8,0.72,0.21,1.0));
    m_renderer->setUniformValue("lightSpecularColor",glm::vec4(1.0,1.0,1.0,1.0));

    // include a scenegraph into rendering
    // m_current_transformation = scenegrap

    glm::mat4 mv_matrix = m_lookAt_matrix * m_trackball_rotation* m_current_transformation ;
    glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(mv_matrix)));

    m_renderer->setUniformValue("projMatrix"        ,m_proj_matrix);
    m_renderer->setUniformValue("modelviewMatrix"   ,mv_matrix); //top of stack in case of scenegraph
    m_renderer->setUniformValue("normalMatrix"      ,normal_matrix);

    iterateAllChildren_DFS(m_root_node);
}

void CgSceneGraph::render(CgBaseRenderer *renderer) {
    iterateAllChildren_DFS(m_root_node);
}

void CgSceneGraph::iterateAllChildren_DFS(CgSceneGraphEntity* node)
{
    //push
      pushMatrix(getCurrent_transformation());
    //apply transformation & include a scenegraph into rendering
      applyTransform(node->getCurrentTransformation());
     // m_current_transformation = getModelviewMatrixStack().top();

    // applyTransormation
    m_renderer->setUniformValue("modelviewMatrix", m_lookAt_matrix * m_trackball_rotation * getModelviewMatrixStack().top());    //top of stack in case of scenegraph

    // zeichne das aktuelle Entity
    for(unsigned int i = 0; i < node->getListOfObjects().size(); i++) {
//        renderer->init(this->getListOfObject().at(i));
        m_renderer->render(node->getListOfObjects().at(i));
    }

    for(unsigned int i=0;i<node->getChildren().size();i++)
    {
        iterateAllChildren_DFS(node->getChildren()[i]);
    }
    //pop
    popMatrix();
}

void CgSceneGraph::handleEvent(CgBaseEvent *e)
{
    // die Enums sind so gebaut, dass man alle Arten von MausEvents über CgEvent::CgMouseEvent abprüfen kann,
    // siehe dazu die CgEvent enums im CgEnums.h

    if(e->getType() & Cg::CgMouseEvent)
    {
        CgMouseEvent* ev = (CgMouseEvent*)e;
        std::cout << *ev << std::endl;

         // hier kommt jetzt die Abarbeitung des Events hin...
    }

    if(e->getType() & Cg::CgTrackballEvent)
    {
        CgTrackballEvent* ev = (CgTrackballEvent*)e;

        m_trackball_rotation=ev->getRotationMatrix();
        m_renderer->redraw();

    }

    // die Enums sind so gebaut, dass man alle Arten von KeyEvents über CgEvent::CgKeyEvent abprüfen kann,
    // siehe dazu die CgEvent enums im CgEnums.h
    // momentan werden nur KeyPressEvents gefangen.

    if(e->getType() & Cg::CgKeyEvent)
    {
        CgKeyEvent* ev = (CgKeyEvent*)e;
        std::cout << *ev <<std::endl;

        if(ev->text()=="+")
        {
            glm::mat4 scalemat = glm::mat4(1.);
            scalemat = glm::scale(scalemat,glm::vec3(1.1,1.1,1.1));
            m_current_transformation=m_current_transformation*scalemat;
            m_renderer->redraw();
        }
        if(ev->text()=="-")
        {
            glm::mat4 scalemat = glm::mat4(1.);
            scalemat = glm::scale(scalemat,glm::vec3(0.9,0.9,0.9));

            m_current_transformation=m_current_transformation*scalemat;

            m_renderer->redraw();
        }
        // hier kommt jetzt die Abarbeitung des Events hin...
    }

    if(e->getType() & Cg::WindowResizeEvent)
    {
         CgWindowResizeEvent* ev = (CgWindowResizeEvent*)e;
         std::cout << *ev <<std::endl;
         m_proj_matrix=glm::perspective(45.0f, (float)(ev->w()) / ev->h(), 0.01f, 100.0f);
    }
}

void CgSceneGraph::pushMatrix(glm::mat4 matrix) {m_modelview_matrix_stack.push(matrix);}
void CgSceneGraph::popMatrix() {m_modelview_matrix_stack.pop();}
void CgSceneGraph::applyTransform(glm::mat4 arg) {m_modelview_matrix_stack.top() *= arg;}

glm::mat4 CgSceneGraph::getCurrent_transformation() const {return m_current_transformation;}
void CgSceneGraph::setCurrent_transformation(const glm::mat4 &current_transformation) {m_current_transformation = current_transformation;}

glm::mat4 CgSceneGraph::getTrackball_rotation() const {return m_trackball_rotation;}
void CgSceneGraph::setTrackball_rotation(const glm::mat4 &trackball_rotation){m_trackball_rotation = trackball_rotation;}

glm::mat4 CgSceneGraph::getLookAt_matrix() const {return m_lookAt_matrix;}
void CgSceneGraph::setLookAt_matrix(const glm::mat4 &lookAt_matrix) {m_lookAt_matrix = lookAt_matrix;}

glm::mat4 CgSceneGraph::getProj_matrix() const {return m_proj_matrix;}
void CgSceneGraph::setProj_matrix(const glm::mat4 &proj_matrix){m_proj_matrix = proj_matrix;}

CgSceneGraphEntity* CgSceneGraph::getRootNode() {return m_root_node;}
std::stack<glm::mat4> CgSceneGraph::getModelviewMatrixStack() {return m_modelview_matrix_stack;}

void CgSceneGraph::setModelviewMatrixStack(std::stack<glm::mat4> modelview_matrix_stack) {
    while (!m_modelview_matrix_stack.empty()) m_modelview_matrix_stack.pop();
    std::stack<glm::mat4> temp_stack;
    int stack_size = modelview_matrix_stack.size();
    for (int i = 0; i < stack_size; ++i) {
        temp_stack.push(modelview_matrix_stack.top());
        modelview_matrix_stack.pop();
    }
    for (int i = 0; i < stack_size; ++i) {
        m_modelview_matrix_stack.push(temp_stack.top());
        temp_stack.pop();
    }
}
void CgSceneGraph::addModelviewMatrixStack(glm::mat4 modelview_matrix) {
    m_modelview_matrix_stack.push(modelview_matrix);
}
