#ifndef CGPOLYLINE_H
#define CGPOLYLINE_H

#include "CgBase/CgBasePolyline.h"
#include <vector>
#include <glm/glm.hpp>
#include "CgBase/CgBaseRenderableObject.h"

class CgPolyline : public CgBasePolyline
{
public:
    CgPolyline();
    CgPolyline(int id, glm::vec3, glm::vec3);

    virtual ~CgPolyline();

    //inherited from CgBaseRenderableObject
    Cg::ObjectType getType() const;
    unsigned int getID() const;

    const std::vector<glm::vec3>& getVertices() const;
    glm::vec3 getColor() const;
    unsigned int getLineWidth() const;

private:
    std::vector<glm::vec3> m_vertices;
    glm::vec3 m_face_colors;
    int m_line_width;
    const Cg::ObjectType m_type;
    const unsigned int m_id;
};

inline Cg::ObjectType  CgPolyline::getType() const {return m_type;}
inline unsigned int CgPolyline::getID() const {return m_id;}

#endif // CGPOLYLINE_H
