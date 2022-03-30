#include "CgPolyline.h"
#include "CgBase/CgEnums.h"
#include "CgUtils/ObjLoader.h"
#include <iostream>

CgPolyline::CgPolyline():
m_type(Cg::Polyline),
m_id(42)
{

}

CgPolyline::CgPolyline(int id, glm::vec3 p1, glm::vec3 p2):
m_type(Cg::Polyline),
m_id(id)
{
    m_line_width =1;
    m_vertices.push_back(p1);
    m_vertices.push_back(p2);
    //std::cout << m_vertices[0] << " " << m_vertices[1] << std::endl;
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
