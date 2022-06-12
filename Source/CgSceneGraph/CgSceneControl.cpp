#include "CgSceneControl.h"

CgSceneControl::CgSceneControl()
{
    m_scene = nullptr;

    m_current_transformation    =glm::mat4(1.);
    m_lookAt_matrix             =glm::lookAt(glm::vec3(0.0,0.0,1.0),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));
    m_proj_matrix               =glm::mat4x4(glm::vec4(1.792591, 0.0, 0.0, 0.0), glm::vec4(0.0, 1.792591, 0.0, 0.0), glm::vec4(0.0, 0.0, -1.0002, -1.0), glm::vec4(0.0, 0.0, -0.020002, 0.0));
    m_trackball_rotation        =glm::mat4(1.);
    m_scalemat                  = glm::mat4(1.);

    doTranslate = false;
    doScale = false;
    doRotate = false;
    doX=false;
    doY=false;
    doZ=false;
    showAABB=false;
    entity_selected=false;

    selected_entity = NULL;
}


CgSceneControl::~CgSceneControl()
{
    if (m_scene != NULL)
        delete m_scene;
    if (m_renderer != NULL)
        delete m_renderer;
    if (selected_entity != NULL)
        delete selected_entity;
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

    if (entity_selected && selected_entity != NULL && selected_entity != nullptr)
        setCurrentTransformation(selected_entity->getCurrentTransformation()*selected_entity->getObjectTransformation());

    // iterate all children
    if (m_scene!=NULL) {
        m_scene->popMatrix();
        m_scene->pushMatrix(m_scene->getRootNode()->getCurrentTransformation());
        m_scene->render(this, m_scene->getRootNode());
    }

    // set coordinate system
    if (entity_selected && selected_entity != m_scene->getRootNode() && selected_entity != NULL) {
        for (int i=0; i<3; ++i) {
            // verschieben zum selektierten Objekt
            setCurrentTransformation(selected_entity->getCurrentTransformation()*selected_entity->getObjectTransformation());
            m_renderer->setUniformValue("mycolor", m_scene->getCoordSystem()->getColorSystem()[i]);
            m_renderer->render(m_scene->getCoordSystem()->getCoordSystem()[i]);
        }

        setCurrentTransformation(glm::mat4(1.0f));
        m_renderer->render(selected_entity->getObject());
        glm::mat4 currentTransformation = selected_entity->getCurrentTransformation() * selected_entity->getObjectTransformation();
        glm::mat4 currentTransformation_inverse = glm::inverse(currentTransformation);

        CgRay* local_ray = new CgRay(Functions::getId());

        local_ray->setA(currentTransformation_inverse * m_scene->getRay()->getA());
        local_ray->setB(currentTransformation_inverse * m_scene->getRay()->getB());
        local_ray->setDirection(currentTransformation_inverse * m_scene->getRay()->getDirection());

        m_renderer->init(local_ray);
        m_renderer->render(local_ray);
        delete local_ray;
    }

    setCurrentTransformation(glm::mat4(1.0));
    if (m_scene != NULL &&  m_scene->getRay() != NULL) {
        m_renderer->setUniformValue("mycolor", glm::vec4(100.0, 0.0, 255.0, 1.0));
        m_renderer->render(m_scene->getRay());
    }

    getRenderer()->setUniformValue("mycolor", Functions::getRed());
    for (unsigned int i = 0; i < m_scene->getIntersections().size(); ++i) {
        glm::vec3 q = m_scene->getIntersections()[i];
        CgUnityCube* obj_intersection = new CgUnityCube(Functions::getId(), q);
        getRenderer()->init(obj_intersection);
        getRenderer()->render(obj_intersection);
        delete obj_intersection;
    }
    std::cout << "----------------------------------------\n";
}

void CgSceneControl::handleEvent(CgBaseEvent* e)
{
    // die Enums sind so gebaut, dass man alle Arten von MausEvents über CgEvent::CgMouseEvent abprüfen kann,
    // siehe dazu die CgEvent enums im CgEnums.h

    if(e->getType() & Cg::CgMouseEvent)
    {
        CgMouseEvent* ev = (CgMouseEvent*)e;
        std::cout << *ev << std::endl;

        // Dann durch alle Entities iterieren und Inverse der CurrentMatrix und ObjectMatrix anwenden => Objektkoordinaten

        if (ev->getMouseButton() == 2) {
            // Pixelkoordinaten in NDCs
            float xNDC = (2.0 * ev->x())/(Functions::getWidth()) - 1.0;
            float yNDC = -(2.0 * ev->y())/(Functions::getHeight()) + 1.0;

            if (xNDC > 1) xNDC = 1;
            if (xNDC < -1) xNDC = -1;
            if (yNDC > 1) yNDC = 1;
            if (yNDC < -1) yNDC = -1;

            //  NDC mit Inverse der Proje^ktionsmatrix von m_proj_matrix und durch homogene Koordinate teilen
            // => Kamerakoordinaten
            m_scene->getRay()->setA(glm::inverse(m_proj_matrix) * glm::vec4(xNDC, yNDC, -0.01f, 1.0f));
            m_scene->getRay()->setB(glm::inverse(m_proj_matrix) * glm::vec4(xNDC, yNDC, 1.0f, 1.f));

            // Koordinaten mit Inverse von m_lookAt_matrix UND m_trackball_rotation und durch homogene Koordinate teilen
            // => Weltkoordinaten
            // currentTransformation Einheitmatrix setzten und m_scalemat wird so auch berücksichtigt
            m_scene->getRay()->applyTransformationA(glm::inverse(m_lookAt_matrix * m_trackball_rotation
                                                                 * m_current_transformation));
            m_scene->getRay()->applyTransformationB(glm::inverse(m_lookAt_matrix * m_trackball_rotation
                                                                 * m_current_transformation));

            m_scene->getRay()->setDirection(m_scene->getRay()->getB() - m_scene->getRay()->getA());

            //pickingIntersection();
            m_scene->startIntersection(this, m_scene->getRootNode());

            //m_scene->setRenderer(m_renderer);
            m_renderer->init(m_scene->getRay());
            m_renderer->redraw();
        }
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

        // zoom in
        if((!entity_selected) && ev->text()=="+")
        {
            m_scalemat = glm::scale(m_scalemat,glm::vec3(1.2,1.2,1.2));
            m_renderer->redraw();
        }

        // zoom out
        if((!entity_selected) && ev->text()=="-")
        {
            m_scalemat = glm::scale(m_scalemat,glm::vec3(0.8,0.8,0.8));
            m_renderer->redraw();
        }

        if (ev->text() == "a") {
            showAABB = !showAABB;
            m_renderer->redraw();
        }

        // unselect object
        if (ev->text() == "w") {
            if (m_scene->getCurrentEntity() != NULL) {
                // restore color of current entity because will be overwritten when selected
                glm::vec4 old_color = selected_entity->getAppearance().getOldColor();
                old_color *= 255.0;
                m_scene->getCurrentEntity()->getAppearance().setObjectColorNoOldColorSave(old_color);
                m_renderer->redraw();
                if(entity_group_selected)
                    iterateChildrenRestoreOldColor(selected_entity);
            }
            entity_selected = false;
            entity_group_selected = false;
            lastPressQ = false;
            lastPressE = false;
        }
        // select group object
        if (ev->text() == "e") {
            if (!entity_group_selected) {
                entity_selected = true;
                entity_group_selected = true;
                if (lastPressQ || lastPressE) {
                    selected_entity->getAppearance().setObjectColorNoOldColorSave(Functions::getPink());
                } else {
                    selected_entity->getAppearance().setObjectColor(Functions::getPink());
                }
                iterateChildrenSetColor(selected_entity, Functions::getPink());
                m_renderer->redraw();
            } else {
                entity_group_selected = false;
                glm::vec4 old_color = selected_entity->getAppearance().getOldColor();
                old_color *= 255.0;
                m_scene->getCurrentEntity()->getAppearance().setObjectColor(old_color);
                iterateChildrenRestoreOldColor(selected_entity);
                m_renderer->redraw();
            }
            lastPressQ = false;
            lastPressE = true;
        }
        // select object
        if(ev->text()=="q")
        {
            lastPressQ = true;
            if (entity_group_selected)
                iterateChildrenRestoreOldColor(selected_entity);
            entity_group_selected = false;
            // restore color of current entity
            if (m_scene->getCurrentEntity() != NULL) {
                glm::vec4 old_color = selected_entity->getAppearance().getOldColor();
                old_color *= 255.0;
                selected_entity->getAppearance().setObjectColor(old_color);
            }
            // select next entity and change its color
            if (entity_selected==true) {
                selected_entity = m_scene->getNextEntity();
            }
            entity_selected=true;
            if (selected_entity != NULL)
                selected_entity->getAppearance().setObjectColor(Functions::getGreen());
            m_renderer->redraw();
            lastPressE = false;        }

        if(ev->text()=="t") {
            doTranslate = true;
            doScale = false;
            doRotate = false;
            std::cout << "Mode: Translation" << doTranslate << "\n";
        }
        if(ev->text()=="s") {
            doTranslate = false;
            doScale = true;
            doRotate = false;
            std::cout << "Mode: Scaling\n";
        }
        if(ev->text()=="r") {
            doTranslate = false;
            doScale = false;
            doRotate = true;
            std::cout << "Mode: Rotation\n";
        }
        if(ev->text()=="x") {
            doX = !doX;
            if (doX) {
                std::cout << "x ausgewähllt\n";
            } else {
                std::cout << "x abgewählt\n";
            }
        }
        if(ev->text()=="y") {
            doY = !doY;
            if (doY) {
                std::cout << "y ausgewähllt\n";
            } else {
                std::cout << "y abgewählt\n";
            }
        }
        if(ev->text()=="z") {
            doZ = !doZ;
            if (doZ) {
                std::cout << "z ausgewähllt\n";
            } else {
                std::cout << "z abgewählt\n";
            }
        }

        if (doScale) {
            double scaleX = 1.0;
            double scaleY = 1.0;
            double scaleZ = 1.0;

            // increase in size if object selected
            if(ev->text()=="+")
            {
                if (doX)
                    scaleX = 1.05;
                if (doY)
                    scaleY = 1.05;
                if (doZ)
                    scaleZ = 1.05;
                if (entity_selected && !entity_group_selected) {
                    // getObjectTransformation due to only want to scale current object and not getCurrentTransformation
                    selected_entity->setObjectTransformation(glm::scale(selected_entity->getObjectTransformation(),
                                                                           glm::vec3(scaleX, scaleY, scaleZ)));
                    for (int i = 0; i < 4; ++i) {
                        std::cout   << "\n (" << selected_entity->getObjectTransformation()[i][0]
                                    << ", " << selected_entity->getObjectTransformation()[i][1]
                                    << ", " << selected_entity->getObjectTransformation()[i][2]
                                    << ", " << selected_entity->getObjectTransformation()[i][3] << ")\n";
                    }
                }

                if (entity_selected && entity_group_selected) {
                    selected_entity->setCurrentTransformation(glm::scale(selected_entity->getCurrentTransformation(),
                                                                    glm::vec3(scaleX, scaleY, scaleZ)));
                    for (int i = 0; i < 4; ++i) {
                        std::cout   << "\n (" << selected_entity->getCurrentTransformation()[i][0]
                                    << ", " << selected_entity->getCurrentTransformation()[i][1]
                                    << ", " << selected_entity->getCurrentTransformation()[i][2]
                                    << ", " << selected_entity->getCurrentTransformation()[i][3] << ")\n";
                    }
                }
            }

            // decrease in size if object selected
            if(ev->text()=="-")
            {
                if (doX)
                    scaleX = 0.95;
                if (doY)
                    scaleY = 0.95;
                if (doZ)
                    scaleZ = 0.95;
                if (entity_selected && !entity_group_selected) {
                    selected_entity->setObjectTransformation(glm::scale(selected_entity->getObjectTransformation(),
                                                                           glm::vec3(scaleX, scaleY, scaleZ)));
                    for (int i = 0; i < 4; ++i) {
                        std::cout   << "\n (" << selected_entity->getObjectTransformation()[i][0]
                                    << ", " << selected_entity->getObjectTransformation()[i][1]
                                    << ", " << selected_entity->getObjectTransformation()[i][2]
                                    << ", " << selected_entity->getObjectTransformation()[i][3] << ")\n";
                    }
                }

                if (entity_selected && entity_group_selected) {
                    selected_entity->setCurrentTransformation(glm::scale(selected_entity->getCurrentTransformation(),
                                                                    glm::vec3(scaleX, scaleY, scaleZ)));
                    for (int i = 0; i < 4; ++i) {
                        std::cout   << "\n (" << selected_entity->getCurrentTransformation()[i][0]
                                    << ", " << selected_entity->getCurrentTransformation()[i][1]
                                    << ", " << selected_entity->getCurrentTransformation()[i][2]
                                    << ", " << selected_entity->getCurrentTransformation()[i][3] << ")\n";
                    }
                }
            }
            m_renderer->redraw();
        }

        if (doTranslate) {
            double translateX = 0.0;
            double translateY = 0.0;
            double translateZ = 0.0;

            if(ev->text()=="+")
            {
                if (doX)
                    translateX = 0.05;
                if (doY)
                    translateY = 0.05;
                if (doZ)
                    translateZ = 0.05;
                if (entity_selected && !entity_group_selected) {
                    glm::mat4 translationMatrix = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0),
                                                              glm::vec4(0.0, 1.0, 0.0, 0.0),
                                                              glm::vec4(0.0, 0.0, 1.0, 0.0),
                                                              glm::vec4(translateX, translateY, translateZ, 1.0));
                    selected_entity->setObjectTransformation(translationMatrix*selected_entity->getObjectTransformation());

                    for (int i = 0; i < 4; ++i) {
                        std::cout   << "\n (" << selected_entity->getObjectTransformation()[i][0]
                                    << ", " << selected_entity->getObjectTransformation()[i][1]
                                    << ", " << selected_entity->getObjectTransformation()[i][2]
                                    << ", " << selected_entity->getObjectTransformation()[i][3] << ")\n";
                    }
                }

                if (entity_selected && entity_group_selected) {
                    glm::mat4 translationMatrix = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0),
                                                              glm::vec4(0.0, 1.0, 0.0, 0.0),
                                                              glm::vec4(0.0, 0.0, 1.0, 0.0),
                                                              glm::vec4(translateX, translateY, translateZ, 1.0));
                    selected_entity->setCurrentTransformation(translationMatrix*selected_entity->getCurrentTransformation());

                    for (int i = 0; i < 4; ++i) {
                        std::cout   << "\n (" << selected_entity->getCurrentTransformation()[i][0]
                                    << ", " << selected_entity->getCurrentTransformation()[i][1]
                                    << ", " << selected_entity->getCurrentTransformation()[i][2]
                                    << ", " << selected_entity->getCurrentTransformation()[i][3] << ")\n";
                    }
                }
            }

            if(ev->text()=="-")
            {
                if (doX)
                    translateX = -0.05;
                if (doY)
                    translateY = -0.05;
                if (doZ)
                    translateZ = -0.05;
                if (entity_selected && !entity_group_selected) {
                    glm::mat4 translationMatrix = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0),
                                                                  glm::vec4(0.0, 1.0, 0.0, 0.0),
                                                                  glm::vec4(0.0, 0.0, 1.0, 0.0),
                                                                  glm::vec4(translateX, translateY, translateZ, 1.0));
                    selected_entity->setObjectTransformation(translationMatrix*selected_entity->getObjectTransformation());

                    for (int i = 0; i < 4; ++i) {
                        std::cout   << "\n (" << selected_entity->getObjectTransformation()[i][0]
                                    << ", " << selected_entity->getObjectTransformation()[i][1]
                                    << ", " << selected_entity->getObjectTransformation()[i][2]
                                    << ", " << selected_entity->getObjectTransformation()[i][3] << ")\n";
                    }
                }

                if (entity_selected && entity_group_selected) {
                    glm::mat4 translationMatrix = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0),
                                                                      glm::vec4(0.0, 1.0, 0.0, 0.0),
                                                                      glm::vec4(0.0, 0.0, 1.0, 0.0),
                                                                      glm::vec4(translateX, translateY, translateZ, 1.0));
                    selected_entity->setCurrentTransformation(translationMatrix*selected_entity->getCurrentTransformation());

                    for (int i = 0; i < 4; ++i) {
                        std::cout   << "\n (" << selected_entity->getCurrentTransformation()[i][0]
                                    << ", " << selected_entity->getCurrentTransformation()[i][1]
                                    << ", " << selected_entity->getCurrentTransformation()[i][2]
                                    << ", " << selected_entity->getCurrentTransformation()[i][3] << ")\n";
                    }
                }
            }
            m_renderer->redraw();
        }

        if (doRotate) {
            double angle = M_PI/24;
            // x-rotation - red axis
            if(entity_selected && !entity_group_selected && ev->text()=="x")
            {
                glm::mat4 translationToOrigin =  glm::inverse(selected_entity->getObjectTransformation());
                glm::mat4 rotation_matrix = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0),
                                                     glm::vec4(0.0, cos(angle) , sin(angle), 0.0),
                                                     glm::vec4(0.0, -sin(angle) , cos(angle), 0.0),
                                                     glm::vec4(0.0, 0.0, 0.0, 1.0));
                glm::mat4 translateBack = glm::inverse(translationToOrigin);
                selected_entity->setObjectTransformation(translateBack*rotation_matrix*translationToOrigin*selected_entity->getObjectTransformation());
                m_renderer->redraw();
            }
            if(entity_selected && entity_group_selected && ev->text()=="x")
            {
                glm::mat4 translationToOrigin =  glm::inverse(selected_entity->getCurrentTransformation());
                glm::mat4 rotation_matrix = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0),
                                                     glm::vec4(0.0, cos(angle) , sin(angle), 0.0),
                                                     glm::vec4(0.0, -sin(angle) , cos(angle), 0.0),
                                                     glm::vec4(0.0, 0.0, 0.0, 1.0));
                glm::mat4 translateBack = glm::inverse(translationToOrigin);
                selected_entity->setCurrentTransformation(translateBack*rotation_matrix*translationToOrigin*selected_entity->getCurrentTransformation());
                m_renderer->redraw();
            }

            // y-rotation - green axis
            if(entity_selected && !entity_group_selected && ev->text()=="y")
            {
                glm::mat4 translationToOrigin =  glm::inverse(selected_entity->getObjectTransformation());
                glm::mat4 rotation_matrix = glm::mat4(glm::vec4(cos(angle), 0.0, -sin(angle), 0.0),
                                                     glm::vec4(0.0, 1.0, 0.0, 0.0),
                                                     glm::vec4(sin(angle), 0.0, cos(angle), 0.0),
                                                     glm::vec4(0.0, 0.0, 0.0, 1.0));
                glm::mat4 translateBack = glm::inverse(translationToOrigin);
                selected_entity->setObjectTransformation(translateBack*rotation_matrix*translationToOrigin*selected_entity->getObjectTransformation());
                m_renderer->redraw();
            }
            if(entity_selected && entity_group_selected && ev->text()=="y")
            {
                glm::mat4 translationToOrigin =  glm::inverse(selected_entity->getCurrentTransformation());
                glm::mat4 rotation_matrix = glm::mat4(glm::vec4(cos(angle), 0.0, -sin(angle), 0.0),
                                                     glm::vec4(0.0, 1.0, 0.0, 0.0),
                                                     glm::vec4(sin(angle), 0.0, cos(angle), 0.0),
                                                     glm::vec4(0.0, 0.0, 0.0, 1.0));
                glm::mat4 translateBack = glm::inverse(translationToOrigin);
                selected_entity->setCurrentTransformation(translateBack*rotation_matrix*translationToOrigin*selected_entity->getCurrentTransformation());
                m_renderer->redraw();
            }

            // z-rotation - blue axis
            if(entity_selected && !entity_group_selected && ev->text()=="z")
            {
                glm::mat4 translationToOrigin =  glm::inverse(selected_entity->getObjectTransformation());
                glm::mat4 rotation_matrix = glm::mat4(glm::vec4(cos(angle), -sin(angle), 0.0, 0.0),
                                                     glm::vec4(sin(angle), cos(angle), 0.0, 0.0),
                                                     glm::vec4(0.0, 0.0, 1.0, 0.0),
                                                     glm::vec4(0.0, 0.0, 0.0, 1.0));
                glm::mat4 translateBack = glm::inverse(translationToOrigin);
                selected_entity->setObjectTransformation(translateBack*rotation_matrix*translationToOrigin*selected_entity->getObjectTransformation());
                m_renderer->redraw();
            }
            if(entity_selected && entity_group_selected && ev->text()=="z")
            {
                glm::mat4 translationToOrigin =  glm::inverse(selected_entity->getCurrentTransformation());
                glm::mat4 rotation_matrix = glm::mat4(glm::vec4(cos(angle), -sin(angle), 0.0, 0.0),
                                                     glm::vec4(sin(angle), cos(angle), 0.0, 0.0),
                                                     glm::vec4(0.0, 0.0, 1.0, 0.0),
                                                     glm::vec4(0.0, 0.0, 0.0, 1.0));
                glm::mat4 translateBack = glm::inverse(translationToOrigin);
                selected_entity->setCurrentTransformation(translateBack*rotation_matrix*translationToOrigin*selected_entity->getCurrentTransformation());
                m_renderer->redraw();
            }
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

void CgSceneControl::iterateChildrenSetColor(CgSceneGraphEntity* entity,glm::vec4 color) {
    for(unsigned int i = 0; i < entity->getChildren().size(); i++) {
        entity->getChildren()[i]->getAppearance().setObjectColor(color);
        iterateChildrenSetColor(entity->getChildren()[i],color);
    }
}

void CgSceneControl::iterateChildrenRestoreOldColor(CgSceneGraphEntity* entity) {
    if (entity != NULL) {
        for(unsigned int i = 0; i < entity->getChildren().size(); i++) {
            glm::vec4 old_color = entity->getChildren()[i]->getAppearance().getOldColor();
            old_color *= 255.0;
            entity->getChildren()[i]->getAppearance().setObjectColor(old_color);
            iterateChildrenRestoreOldColor(entity->getChildren()[i]);
        }
    }
}

CgSceneGraphEntity* CgSceneControl::getSelectedEntity() {
    return selected_entity;
}

bool CgSceneControl::getShowAABB() { return showAABB; }

/*void CgSceneControl::pickingIntersection() {
    m_intersections.clear();
    for (unsigned int i = 0; i < m_cube->getTriangleIndices().size(); i+=3) {
        glm::vec3 a = m_cube->getVertices()[m_cube->getTriangleIndices()[i]];
        glm::vec3 b = m_cube->getVertices()[m_cube->getTriangleIndices()[i+1]];
        glm::vec3 c = m_cube->getVertices()[m_cube->getTriangleIndices()[i+2]];
        CgPlane p {CgPlane(a, b, c)};
        float t;
        glm::vec3 q;
        if (IntersectRayPlane(p, t, q))
        {
            float u, v, w;
            Barycentric(a, b, c, q, u, v, w);
            if(u >= 0 && u <= 1 && v >= 0 && v <= 1 && w >= 0 && w <= 1)
                 m_intersections.push_back(glm::vec3(q[0], q[1], q[2]));
        }
    }
}

bool CgSceneControl::IntersectRayPlane(CgPlane& p, float& t, glm::vec3& q) {
    glm::vec3 a = glm::vec3(m_scene->getRay()->getA()[0], m_scene->getRay()->getA()[1], m_scene->getRay()->getA()[2]);
    glm::vec3 ab = glm::vec3(m_scene->getRay()->getDirection()[0], m_scene->getRay()->getDirection()[1], m_scene->getRay()->getDirection()[2]);

    t = (p.d - glm::dot(p.n, a)) / glm::dot(p.n, ab);

    if (std::isfinite(t) && t >= 0.0f ) {
        q = a + t * ab;
        return 1;
    }
    return 0;
}

void CgSceneControl::Barycentric(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& q,
                               float& u, float& v, float& w) {
    glm::vec3 v0 = b - a;
    glm::vec3 v1 = c - a;
    glm::vec3 v2 = q - a;
    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d11 = glm::dot(v1, v1);
    float d20 = glm::dot(v2, v0);
    float d21 = glm::dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
}*/
