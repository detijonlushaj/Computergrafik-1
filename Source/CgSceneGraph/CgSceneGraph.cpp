#include "CgSceneGraph.h"

#include <glm/gtc/matrix_transform.hpp>


CgSceneGraph::CgSceneGraph()
{
    m_current_transformation    =glm::mat4(1.);
    m_lookAt_matrix             =glm::lookAt(glm::vec3(0.0,0.0,1.0),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));
    m_proj_matrix               =glm::mat4x4(glm::vec4(1.792591, 0.0, 0.0, 0.0), glm::vec4(0.0, 1.792591, 0.0, 0.0), glm::vec4(0.0, 0.0, -1.0002, -1.0), glm::vec4(0.0, 0.0, -0.020002, 0.0));
    m_trackball_rotation        =glm::mat4(1.);
}

void CgSceneGraph::setRenderer(CgBaseRenderer *r)
{
    m_renderer=r;
    m_renderer->setSceneControl(this);

    //set Color in the beginn of the Rendering - removed form rederObjects()!
    m_renderer->setUniformValue("mycolor",glm::vec4(0.0,1.0,0.0,1.0));
}

void CgSceneGraph::renderObjects()
{
    glm::mat4 mv_matrix = m_lookAt_matrix * m_trackball_rotation* m_current_transformation ;
    glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(mv_matrix)));

    m_renderer->setUniformValue("projMatrix"        ,m_proj_matrix);
    m_renderer->setUniformValue("modelviewMatrix"   ,mv_matrix);    //top of stack in case of scenegraph
    m_renderer->setUniformValue("normalMatrix"      ,normal_matrix);
}

void CgSceneGraph::handleEvent(CgBaseEvent *e)
{

}



void CgSceneGraph::render(CgBaseRenderer *renderer) {
    m_root_node->iterateAllChildren_DFS(this, renderer);
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
