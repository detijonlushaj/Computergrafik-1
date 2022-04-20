//Objekte und Strukturen anlegen, verwalten resetten und l¨oschen. Dies stellt den Bereich Control“ des MVC Konzeptes dar.

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
#include "../CgUtils/Functions.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "CgUtils/ObjLoader.h"
#include <string>
#include <cmath>

CgSceneControl::CgSceneControl()
{
    m_triangle      =nullptr;
    m_cube          =nullptr;
    m_rotation      =nullptr;
    m_polyline      =nullptr;
    m_loadObj       =nullptr;

    m_current_transformation    =glm::mat4(1.);
    m_lookAt_matrix             =glm::lookAt(glm::vec3(0.0,0.0,1.0),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));
    m_proj_matrix               =glm::mat4x4(glm::vec4(1.792591, 0.0, 0.0, 0.0), glm::vec4(0.0, 1.792591, 0.0, 0.0), glm::vec4(0.0, 0.0, -1.0002, -1.0), glm::vec4(0.0, 0.0, -0.020002, 0.0));
    m_trackball_rotation        =glm::mat4(1.);

//    m_triangle = new CgExampleTriangle(Functions::getId());

//    m_cube= new CgUnityCube(21);
//    for(unsigned int i = 0; i < m_cube->getFaceCentroid().size() ; i++) {
//        std::vector<glm::vec3> pp;
//        pp.push_back(m_cube->getFaceCentroid()[i]);
//        pp.push_back((m_cube->getFaceCentroid()[i]) + (m_cube->getFaceNormals()[i]) );
//        m_polylines.push_back(new CgPolyline(i, pp));
//    }

//    curve.push_back( glm::vec3(0.0  , 1.5  , 0.0) );
//    curve.push_back( glm::vec3(1.0  , 0.5  , 0.0) );
//    curve.push_back( glm::vec3(1.0  ,-0.5  , 0.0) );
//    curve.push_back( glm::vec3(0.0  ,-1.5  , 0.0) );
//    m_polyline = new CgPolyline(Functions::getId(), curve);

}


CgSceneControl::~CgSceneControl()
{
    if(m_triangle!=NULL)
        delete m_triangle;

    if(m_cube!=NULL)
        delete m_cube;

    for(unsigned int i = 0; i < m_polylines.size() ; ++i) {
        if(m_polylines[i] != NULL)
            delete m_polylines[i];
    }

    if (m_polyline != NULL)
            delete m_polyline;

    if(m_rotation!=NULL)
        delete m_rotation;

    if(m_loadObj!=NULL)
        delete m_loadObj;
}

void CgSceneControl::setRenderer(CgBaseRenderer* r)
{
    m_renderer=r;
    m_renderer->setSceneControl(this);

    //set Color in the beginn of the Rendering - removed form rederObjects()!
    m_renderer->setUniformValue("mycolor",glm::vec4(0.0,1.0,0.0,1.0));

    if(m_triangle!=NULL)
        m_renderer->init(m_triangle);

    if(m_cube!=NULL)
        m_renderer->init(m_cube);

    for(unsigned int i = 0; i < m_polylines.size() ; i++) {
        if(m_polylines[i] != NULL)
            m_renderer->init(m_polylines[i]);
    }

    if(m_polyline!=NULL)
            m_renderer->init(m_polyline);

    if(m_rotation!=NULL)
            m_renderer->init(m_rotation);

    if(m_loadObj!=NULL)
            m_renderer->init(m_loadObj);
}


void CgSceneControl::renderObjects()
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

    if(m_triangle!=NULL)
        m_renderer->render(m_triangle);

    if(m_cube!=NULL)
        m_renderer->render(m_cube);

    for(unsigned int i = 0; i < m_polylines.size() ; i++) {
        if(m_polylines[i] != NULL)
            m_renderer->render(m_polylines[i]);
    }

    if(m_polyline!=NULL)
        m_renderer->render(m_polyline);

    if(m_rotation!=NULL)
        m_renderer->render(m_rotation);

    if(m_loadObj!=NULL)
        m_renderer->render(m_loadObj);

}

void CgSceneControl::handleEvent(CgBaseEvent* e)
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

    //change color event Farber aendern Event
    if(e->getType() & Cg::CgButtonColorChangePress)
    {
        CgColorChangeEvent* ev = (CgColorChangeEvent*)e;
        std::cout << *ev << std::endl;

        double red = ev->getRed() / 255.0;
        double blue = ev->getBlue() / 255.0;
        double green = ev->getGreen() / 255.0;

        m_renderer->setUniformValue("mycolor"   ,glm::vec4(red,green,blue,1.0));
        m_renderer->redraw();
    }

    if(e->getType() & Cg::CgButton_LR_UA_start) {
        CgLaneRiesenfeldEvent* ev = (CgLaneRiesenfeldEvent*)e;
        std::cout << *ev << std::endl;
        if( m_polyline != nullptr) {
            m_polyline->setVertices(Functions::Lane_Riesenfeld_Unterteilungs_Algorithmus(curve, ev->getSubdivisionStep()));

            m_renderer->init(m_polyline);
            m_renderer->redraw();
        } else {
            std::cout << "there is no m_polyline obj" << std::endl;
        }
    }

    if(e->getType() & Cg::CgButton_LR_UA_reset) {
        CgLaneRiesenfeldEvent* ev = (CgLaneRiesenfeldEvent*)e;
        std::cout << *ev << std::endl;

        if(m_polyline != nullptr){
            if(m_rotation != nullptr) {
                delete m_rotation;
                m_polylines.clear();
                m_polyline = new CgPolyline(Functions::getId(),curve);
            }
            m_polyline->setVertices(curve);
            m_renderer->init(m_polyline);
            m_renderer->redraw();
        } else {
            std::cout << "there is no m_polyline obj to reset" << std::endl;
        }
    }

    if(e->getType() & Cg::CgButtonRotation) {
        CgRotationEvent* ev = (CgRotationEvent*)e;
        std::cout << *ev << std::endl;

        if(m_rotation != nullptr) {
            m_polyline = new CgPolyline(Functions::getId(),curve);
            m_polylines.clear();
            delete m_rotation;
        }

        if( m_polyline != nullptr) {
            double angle = 2*M_PI / (double)ev->getRotatorischeSegmente();
            int original_size = m_polyline->getVertices().size();

            for (int j = 0; j < ev->getRotatorischeSegmente(); ++j) {

                for (int i = 0; i < original_size; ++i) {
                    float x = m_polyline->getVertices().at(i)[0]*cos(angle) - m_polyline->getVertices().at(i)[2]*sin(angle);
                    float z = m_polyline->getVertices().at(i)[2]*cos(angle) + m_polyline->getVertices().at(i)[0]*sin(angle);
                    m_polyline->addVertice(glm::vec3(x, m_polyline->getVertices().at(i)[1], z));
                }

                angle+=2*M_PI / (double)ev->getRotatorischeSegmente();
            }

            m_rotation = new CgRotation(Functions::getId(), m_polyline->getVertices(), original_size);

            delete m_polyline;
            m_polyline = nullptr;

            if (ev->getNormals() == true) {
// dreiecksnormalen
//                for(unsigned int i = 0; i < m_rotation->getFaceCentroid().size() ; ++i) {
//                    std::vector<glm::vec3> vertices;

//                    vertices.push_back( m_rotation->getFaceCentroid()[i] );
//                    vertices.push_back( (m_rotation->getFaceCentroid()[i] + (m_rotation->getFaceNormals()[i]) ) );
//                    m_polylines.push_back(new CgPolyline(Functions::getId(), vertices));
//                }

                for(unsigned int i = 0; i < m_rotation->getVertexNormals().size() ; ++i) {
                    std::vector<glm::vec3> vertices;

                    vertices.push_back( m_rotation->getVertices()[i] );
                    vertices.push_back( (m_rotation->getVertices()[i] + (m_rotation->getVertexNormals()[i]) ) );
                    m_polylines.push_back(new CgPolyline(Functions::getId(), vertices));
                }

                 for(unsigned int i = 0; i < m_polylines.size(); ++i) {
                     m_renderer->render(m_polylines[i]);
                     m_renderer->init(m_polylines[i]);
                 }
                 m_renderer->redraw();
            }

            m_renderer->init(m_rotation);
            m_renderer->redraw();
        } else {
            std::cout << "there is no m_polyline obj" << std::endl;
        }
    }

    // an der Stelle an der ein Event abgearbeitet ist wird es auch gelöscht.
    delete e;
}
