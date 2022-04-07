#include "CgPolyline.h"
#include "CgBase/CgEnums.h"
#include "CgUtils/ObjLoader.h"
#include <iostream>

CgPolyline::CgPolyline(int id, std::vector<glm::vec3> vertices):
m_type(Cg::Polyline), m_id(id), m_line_width{1}, m_face_colors{glm::vec3(0, 255.0, 0)}
{
    for(int i = 0; i< vertices.size(); i++) {
        m_vertices.push_back(glm::vec3(vertices.at(i)[0], vertices.at(i)[1], vertices.at(i)[2]));
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

void CgPolyline::setVertices(std::vector<glm::vec3> newvertices){

//    std::cout << "m_polyline vor Clear:\n";
//    for (int i = 0; i < getVertices().size(); i++) {
//        std::cout << i << " " << getVertices().at(i)[0] << " " << getVertices().at(i)[1] << " " << getVertices().at(i)[2] << std::endl;
//    }

    m_vertices.clear();

//    std::cout << "m_polyline nach Clear:\n";
//    for (int i = 0; i < getVertices().size(); i++) {
//        std::cout << i << " " << getVertices().at(i)[0] << " " << getVertices().at(i)[1] << " " << getVertices().at(i)[2] << std::endl;
//    }

    for(std::vector<unsigned int>::size_type i = 0; i< newvertices.size(); i++) {
        m_vertices.push_back(glm::vec3(newvertices.at(i)[0], newvertices.at(i)[1], newvertices.at(i)[2]));
    }

//    std::cout << "m_polyline nach Zuweisung:\n";
//    for (int i = 0; i < getVertices().size(); i++) {
//        std::cout << i << " " << getVertices().at(i)[0] << " " << getVertices().at(i)[1] << " " << getVertices().at(i)[2] << std::endl;
//    }

}
