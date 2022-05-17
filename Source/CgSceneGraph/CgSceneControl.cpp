//Objekte und Strukturen anlegen, verwalten resetten und l¨oschen. Dies stellt den Bereich Control“ des MVC Konzeptes dar.

#include "CgSceneControl.h"


CgSceneControl::CgSceneControl()
{
    m_scene = nullptr;

    m_current_transformation =glm::mat4(1.);
    m_lookAt_matrix          =glm::lookAt(glm::vec3(0.0,0.0,1.0),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));
    m_proj_matrix            =glm::mat4x4(glm::vec4(1.792591, 0.0, 0.0, 0.0), glm::vec4(0.0, 1.792591, 0.0, 0.0), glm::vec4(0.0, 0.0, -1.0002, -1.0), glm::vec4(0.0, 0.0, -0.020002, 0.0));
    m_trackball_rotation     =glm::mat4(1.);
    m_scalemat               =glm::mat4(1.);

}


CgSceneControl::~CgSceneControl()
{
    if (m_scene != NULL)
        delete m_scene;
}

void CgSceneControl::setRenderer(CgBaseRenderer* r)
{
    m_renderer=r;
    m_renderer->setSceneControl(this);

}
CgBaseRenderer*& CgSceneControl::getRenderer()
{
    return m_renderer;
}

void CgSceneControl::renderObjects()
{

    // Materialeigenschaften setzen
    // sollte ja eigentlich pro Objekt unterschiedlich sein können, naja bekommen sie schon hin....
    m_renderer->setUniformValue("mycolor",glm::vec4(0.0,1.0,0.0,1.0));


    m_renderer->setUniformValue("matDiffuseColor"   ,glm::vec4(0.35,0.31,0.09,1.0));
    m_renderer->setUniformValue("lightDiffuseColor" ,glm::vec4(1.0,1.0,1.0,1.0));

    m_renderer->setUniformValue("matAmbientColor"   ,glm::vec4(0.25,0.22,0.06,1.0));
    m_renderer->setUniformValue("lightAmbientColor" ,glm::vec4(1.0,1.0,1.0,1.0));

    m_renderer->setUniformValue("matSpecularColor"  ,glm::vec4(0.8,0.72,0.21,1.0));
    m_renderer->setUniformValue("lightSpecularColor",glm::vec4(1.0,1.0,1.0,1.0));

    // iterate all children
    if (m_scene!=NULL) {
        m_scene->render(this, m_scene->getRootNode());
    }

    // set coordinate system
    if (entity_selected) {
        for (int i=0; i<3; ++i) {
            // verschieben zum selektierten Objekt
            setCurrentTransformation(selected_entity->getCurrentTransformation());
            m_renderer->setUniformValue("mycolor", m_scene->getCoordSystem()->getColorSystem()[i]);
            m_renderer->render(m_scene->getCoordSystem()->getCoordSystem()[i]);
        }
    }
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
        // unselect object
        if (ev->text() == "w") {
            if (entity_selected) {
                entity_selected = false;
                // restore color of current entity because will be overwritten when selected
                glm::vec4 old_color = m_scene->getCurrentEntityOldColor();
                old_color *= 255.0;
                m_scene->getCurrentEntity()->getAppearance().setObjectColor(old_color);
                m_renderer->redraw();
            }

        }
                // select group object
        if (ev->text() == "e") {
            if (!entity_group_selected) {
                entity_group_selected = true;

                selected_entity->getAppearance().setObjectColor(glm::vec4(255.0, 0.0, 0.0, 1.0));
                m_renderer->redraw();
            }

        }
        // select object
        if(ev->text()=="q")
        {
            entity_selected = true;
            entity_group_selected = false;
            // restore color of current entity
            if (m_scene->getCurrentEntity() != NULL) {
                glm::vec4 old_color = m_scene->getCurrentEntityOldColor();
                old_color *= 255.0;
                m_scene->getCurrentEntity()->getAppearance().setObjectColor(old_color);
            }
            // select next entity and change its color
            selected_entity = m_scene->getNextEntity();
            selected_entity->getAppearance().setObjectColor(glm::vec4(0.0, 255.0, 0.0, 1.0));
            m_renderer->redraw();
        }


        // zoom if object selected
        if(entity_selected && !entity_group_selected && ev->text()=="+")
        {
            // getObjectTransformation due to only want to scale current object and not getCurrentTransformation
            selected_entity->setObjectTransformation(glm::scale(selected_entity->getObjectTransformation(),
                                                                   glm::vec3(1.2,1.2,1.2)));
            //selected_entity->setCurrentTransformation(scaled_matrix);
            m_renderer->redraw();
        }
        // zoom if object group selected
        if(entity_selected && entity_group_selected && ev->text()=="+")
        {
            // getObjectTransformation due to only want to scale current object and not getCurrentTransformation
            glm::mat4 scaled_matrix = glm::scale(selected_entity->getCurrentTransformation(),
                                                 glm::vec3(1.2,1.2,1.2));
            selected_entity->setCurrentTransformation(scaled_matrix);
            m_renderer->redraw();
        }

        if((!entity_selected) && ev->text()=="+")
        {
            // glm::mat4 scalemat = glm::mat4(1.);
            m_scalemat = glm::scale(m_scalemat,glm::vec3(1.2,1.2,1.2));
            // m_current_transformation=m_current_transformation*scalemat;
            m_renderer->redraw();
        }

        if(entity_selected && !entity_group_selected && ev->text()=="-")
        {
            selected_entity->setObjectTransformation(glm::scale(selected_entity->getObjectTransformation(),
                                                                   glm::vec3(0.8,0.8,0.8)));
            //selected_entity->setCurrentTransformation(scaled_matrix);
            m_renderer->redraw();
        }
        // zoom if object group selected
        if(entity_selected && entity_group_selected && ev->text()=="-")
        {
            // getObjectTransformation due to only want to scale current object and not getCurrentTransformation
            glm::mat4 scaled_matrix = glm::scale(selected_entity->getCurrentTransformation(),
                                                 glm::vec3(0.8,0.8,0.8));
            selected_entity->setCurrentTransformation(scaled_matrix);
            m_renderer->redraw();
        }
        if((!entity_selected) && ev->text()=="-")
        {
            // glm::mat4 scalemat = glm::mat4(1.);
            m_scalemat = glm::scale(m_scalemat,glm::vec3(0.8,0.8,0.8));
            // m_current_transformation=m_current_transformation*scalemat;
            m_renderer->redraw();
        }

        // x-rotation - red axis
        if(entity_selected && !entity_group_selected && ev->text()=="x")
        {
            glm::mat4 translationToOrigin =  glm::inverse(selected_entity->getObjectTransformation());
            glm::mat4 rotation_matrix = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0),
                                                 glm::vec4(0.0, cos(M_PI/12) , sin(M_PI/12), 0.0),
                                                 glm::vec4(0.0, -sin(M_PI/12) , cos(M_PI/12), 0.0),
                                                 glm::vec4(0.0, 0.0, 0.0, 1.0));
            glm::mat4 translateBack = glm::inverse(translationToOrigin);
            selected_entity->setObjectTransformation(translateBack*rotation_matrix*translationToOrigin*selected_entity->getObjectTransformation());
            m_renderer->redraw();
        }
        if(entity_selected && entity_group_selected && ev->text()=="x")
        {
            glm::mat4 translationToOrigin =  glm::inverse(selected_entity->getCurrentTransformation());
            glm::mat4 rotation_matrix = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0),
                                                 glm::vec4(0.0, cos(M_PI/12) , sin(M_PI/12), 0.0),
                                                 glm::vec4(0.0, -sin(M_PI/12) , cos(M_PI/12), 0.0),
                                                 glm::vec4(0.0, 0.0, 0.0, 1.0));
            glm::mat4 translateBack = glm::inverse(translationToOrigin);
            selected_entity->setCurrentTransformation(translateBack*rotation_matrix*translationToOrigin*selected_entity->getCurrentTransformation());
            m_renderer->redraw();
        }

        // y-rotation - green axis
        if(entity_selected && !entity_group_selected && ev->text()=="y")
        {
            glm::mat4 translationToOrigin =  glm::inverse(selected_entity->getObjectTransformation());
            glm::mat4 rotation_matrix = glm::mat4(glm::vec4(cos(M_PI/12), 0.0, sin(M_PI/12), 0.0),
                                                 glm::vec4(0.0, 1.0, 0.0, 0.0),
                                                 glm::vec4(-sin(M_PI/12), 0.0, cos(M_PI/12), 0.0),
                                                 glm::vec4(0.0, 0.0, 0.0, 1.0));
            glm::mat4 translateBack = glm::inverse(translationToOrigin);
            selected_entity->setObjectTransformation(translateBack*rotation_matrix*translationToOrigin*selected_entity->getObjectTransformation());
            m_renderer->redraw();
        }
        if(entity_selected && entity_group_selected && ev->text()=="y")
        {
            glm::mat4 translationToOrigin =  glm::inverse(selected_entity->getCurrentTransformation());
            glm::mat4 rotation_matrix = glm::mat4(glm::vec4(cos(M_PI/12), 0.0, sin(M_PI/12), 0.0),
                                                 glm::vec4(0.0, 1.0, 0.0, 0.0),
                                                 glm::vec4(-sin(M_PI/12), 0.0, cos(M_PI/12), 0.0),
                                                 glm::vec4(0.0, 0.0, 0.0, 1.0));
            glm::mat4 translateBack = glm::inverse(translationToOrigin);
            selected_entity->setCurrentTransformation(translateBack*rotation_matrix*translationToOrigin*selected_entity->getCurrentTransformation());
            m_renderer->redraw();
        }

        // z-rotation - blue axis
        if(entity_selected && !entity_group_selected && ev->text()=="z")
        {
            glm::mat4 translationToOrigin =  glm::inverse(selected_entity->getObjectTransformation());
            glm::mat4 rotation_matrix = glm::mat4(glm::vec4(cos(M_PI/12), sin(M_PI/12), 0.0, 0.0),
                                                 glm::vec4(-sin(M_PI/12), cos(M_PI/12), 0.0, 0.0),
                                                 glm::vec4(0.0, 0.0, 1.0, 0.0),
                                                 glm::vec4(0.0, 0.0, 0.0, 1.0));
            glm::mat4 translateBack = glm::inverse(translationToOrigin);
            selected_entity->setObjectTransformation(translateBack*rotation_matrix*translationToOrigin*selected_entity->getObjectTransformation());
            m_renderer->redraw();
        }
        if(entity_selected && entity_group_selected && ev->text()=="z")
        {
            glm::mat4 translationToOrigin =  glm::inverse(selected_entity->getCurrentTransformation());
            glm::mat4 rotation_matrix = glm::mat4(glm::vec4(cos(M_PI/12), sin(M_PI/12), 0.0, 0.0),
                                                 glm::vec4(-sin(M_PI/12), cos(M_PI/12), 0.0, 0.0),
                                                 glm::vec4(0.0, 0.0, 0.0, 0.0),
                                                 glm::vec4(0.0, 0.0, 0.0, 1.0));
            glm::mat4 translateBack = glm::inverse(translationToOrigin);
            selected_entity->setCurrentTransformation(translateBack*rotation_matrix*translationToOrigin*selected_entity->getCurrentTransformation());
            m_renderer->redraw();
        }

    }

    if(e->getType() & Cg::CgButtonTranslation) {
        CgTranslationEvent* ev = (CgTranslationEvent*)e;
        std::cout << *ev << " obj " <<std::endl;

        if(!ev->getTranslate_grp()) {
            glm::mat4 translationMatrix = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0),
                                                  glm::vec4(0.0, 1.0, 0.0, 0.0),
                                                  glm::vec4(0.0, 0.0, 1.0, 0.0),
                                                  ev->getVector());
            selected_entity->setObjectTransformation(translationMatrix*selected_entity->getObjectTransformation());
        } else {
            glm::mat4 translationMatrix = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0),
                                                  glm::vec4(0.0, 1.0, 0.0, 0.0),
                                                  glm::vec4(0.0, 0.0, 1.0, 0.0),
                                                  ev->getVector());
            selected_entity->setCurrentTransformation(translationMatrix*selected_entity->getCurrentTransformation());
        }
        m_renderer->redraw();
    }


    if(e->getType() & Cg::WindowResizeEvent)
    {
        CgWindowResizeEvent* ev = (CgWindowResizeEvent*)e;
        std::cout << *ev <<std::endl;
        m_proj_matrix=glm::perspective(45.0f, (float)(ev->w()) / ev->h(), 0.01f, 100.0f);
    }

    // an der Stelle an der ein Event abgearbeitet ist wird es auch gelöscht.
    delete e;
}

void CgSceneControl::setScene(CgSceneGraph *scene) {
    m_scene = scene;
    m_scene->setRenderer(m_renderer);
}

void CgSceneControl::setCurrentTransformation(glm::mat4 transformation_matrix)
{
    m_current_transformation = m_scalemat*transformation_matrix;
    glm::mat4 mv_matrix = m_lookAt_matrix* m_trackball_rotation* m_current_transformation;
    glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(mv_matrix)));

    m_renderer->setUniformValue("projMatrix",m_proj_matrix);
    m_renderer->setUniformValue("modelviewMatrix",mv_matrix);
    m_renderer->setUniformValue("normalMatrix",normal_matrix);
}
