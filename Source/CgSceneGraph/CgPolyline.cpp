#include "CgPolyline.h"
#include "CgBase/CgEnums.h"
#include "CgUtils/ObjLoader.h"
#include <iostream>

CgPolyline::CgPolyline(int id, std::vector<glm::vec3> vertices):
m_type(Cg::Polyline),
m_id(id),
m_line_width{1},
m_face_colors{glm::vec3(0, 255.0, 0)}
{
    for(unsigned int i = 0; i< vertices.size(); i++) {
        m_vertices.push_back(vertices.at(i));
    }
}

CgPolyline::~CgPolyline(){
    m_vertices.clear();
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

void CgPolyline::setVertices(std::vector<glm::vec3> newvertices){
    m_vertices.clear();
    for(unsigned int i = 0; i< newvertices.size(); ++i) {
        m_vertices.push_back(newvertices.at(i));
    }
}

void CgPolyline::addVertice(glm::vec3 vertex) {
    m_vertices.push_back(vertex);
}
