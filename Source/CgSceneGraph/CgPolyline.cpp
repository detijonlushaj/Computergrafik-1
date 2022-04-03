#include "CgPolyline.h"
#include "CgBase/CgEnums.h"
#include "CgUtils/ObjLoader.h"
#include <iostream>

CgPolyline::CgPolyline(int id, std::vector<glm::vec3> pp):
m_type(Cg::Polyline),
m_id(id),
m_line_width{1},
m_face_colors{1}
{
    for(int i = 0; i< pp.size(); i++) {
        m_vertices.push_back(pp.at(i));
    }

}

CgPolyline::~CgPolyline(){
    m_vertices.clear();
    //m_face_colors.clear();
}

const std::vector<glm::vec3>& CgPolyline::getVertices() const
{
    return m_vertices;
}

glm::vec3 CgPolyline::getColor() const {
    return m_face_colors;
}
unsigned int CgPolyline::getLineWidth() const {
    return m_line_width;
}
