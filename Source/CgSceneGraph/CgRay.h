#ifndef CGRAY_H
#define CGRAY_H

#include "CgBase/CgBasePolyline.h"
#include <vector>
#include <glm/glm.hpp>
#include "CgBase/CgBaseRenderableObject.h"
#include "../CgUtils/Functions.h"

class CgRay : public CgBasePolyline
{
public:
    CgRay(int id);
    CgRay(int id, std::vector<glm::vec3> vertices);

    virtual ~CgRay();

    //inherited from CgBaseRenderableObject
    Cg::ObjectType getType() const override;
    unsigned int getID() const override;

    const std::vector<glm::vec3>& getVertices() const override;
    glm::vec3 getColor() const override;
    unsigned int getLineWidth() const override;

    //eigene methoden
    void setVertices(std::vector<glm::vec3>);
    void addVertice(glm::vec3);

    glm::vec4 getA();
    void setA(glm::vec4);
    void applyTransformationA(glm::mat4);

    glm::vec4 getB();
    void setB(glm::vec4);
    void applyTransformationB(glm::mat4);

    glm::vec4 getDirection();
    void setDirection(glm::vec4);

    void createRay();
    void resetObjCoordToWorld();
    void secureWorldCoord();

private:
    glm::vec4 a;
    glm::vec4 b;
    glm::vec4 direction;

    glm::vec4 tmp_a;
    glm::vec4 tmp_b;
    glm::vec4 tmp_direction;

    // t >= 0
    double t;

    const Cg::ObjectType m_type;
    const unsigned int m_id;
    std::vector<glm::vec3> m_vertices;
    int m_line_width;
    glm::vec3 m_face_colors;
};

inline Cg::ObjectType  CgRay::getType() const {return m_type;}
inline unsigned int CgRay::getID() const {return m_id;}

#endif // CGRAY_H
