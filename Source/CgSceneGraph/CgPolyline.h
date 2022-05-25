#ifndef CGPOLYLINE_H
#define CGPOLYLINE_H

#include "CgBase/CgBasePolyline.h"
#include <vector>
#include <glm/glm.hpp>
#include "CgBase/CgBaseRenderableObject.h"
#include "../CgUtils/Functions.h"

class CgPolyline : public CgBasePolyline
{
public:
    CgPolyline(int id, std::vector<glm::vec3> vertices);

    virtual ~CgPolyline();

    //inherited from CgBaseRenderableObject
    Cg::ObjectType getType() const override;
    unsigned int getID() const override;

    const std::vector<glm::vec3>& getVertices() const override;
    glm::vec3 getColor() const override;
    unsigned int getLineWidth() const override;

    //eigene methoden
    void setVertices(std::vector<glm::vec3>);
    void addVertice(glm::vec3);

private:
    const Cg::ObjectType m_type;
    const unsigned int m_id;
    std::vector<glm::vec3> m_vertices;
    int m_line_width;
    glm::vec3 m_face_colors;
};

inline Cg::ObjectType  CgPolyline::getType() const {return m_type;}
inline unsigned int CgPolyline::getID() const {return m_id;}

#endif // CGPOLYLINE_H
