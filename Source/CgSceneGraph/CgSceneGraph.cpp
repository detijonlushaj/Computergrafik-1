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
    m_renderer->setUniformValue("matDiffuseColor"   ,glm::vec4(0.35,0.31,0.09,1.0));
    m_renderer->setUniformValue("lightDiffuseColor" ,glm::vec4(1.0,1.0,1.0,1.0));

    m_renderer->setUniformValue("matAmbientColor"   ,glm::vec4(0.25,0.22,0.06,1.0));
    m_renderer->setUniformValue("lightAmbientColor" ,glm::vec4(1.0,1.0,1.0,1.0));

    m_renderer->setUniformValue("matSpecularColor"  ,glm::vec4(0.8,0.72,0.21,1.0));
    m_renderer->setUniformValue("lightSpecularColor",glm::vec4(1.0,1.0,1.0,1.0));

    glm::mat4 mv_matrix = m_lookAt_matrix * m_trackball_rotation* m_current_transformation ;
    glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(mv_matrix)));

    m_renderer->setUniformValue("projMatrix"        ,m_proj_matrix);
    m_renderer->setUniformValue("modelviewMatrix"   ,mv_matrix);    //top of stack in case of scenegraph
    m_renderer->setUniformValue("normalMatrix"      ,normal_matrix);
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

    if(e->getType() & Cg::LoadObjFileEvent)
    {

        CgLoadObjFileEvent* ev = (CgLoadObjFileEvent*)e;


        ObjLoader* loader= new ObjLoader();
        loader->load(ev->FileName());

        std::cout << ev->FileName() << std::endl;

        std::vector<glm::vec3> pos;
        loader->getPositionData(pos);

        std::vector<glm::vec3> norm;
        loader->getNormalData(norm);

        std::vector<unsigned int> indx;
        loader->getFaceIndexData(indx);

        //m_loadObj = new CgLoadObjFile(Functions::getId(),pos,norm,indx);
        //m_loadObj->init(pos,norm,indx);

        m_loadObj = new CgLoadObjFile(Functions::getId(),pos,indx);

        m_renderer->init(m_loadObj);
        m_renderer->redraw();

        for(unsigned int i = 0; i < m_loadObj->getVertexNormals().size() ; ++i) {
            std::vector<glm::vec3> vertices;
            vertices.push_back( m_loadObj->getVertices()[i] );
            vertices.push_back( (m_loadObj->getVertices()[i] + (m_loadObj->getVertexNormals()[i]/100.0f) ) );
            m_polylines.push_back(new CgPolyline(Functions::getId(), vertices));
        }

        for(unsigned int i = 0; i < m_polylines.size() ; ++i) {
            m_renderer->render(m_polylines[i]);
            m_renderer->init(m_polylines[i]);
        }
        m_renderer->redraw();

    }


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
