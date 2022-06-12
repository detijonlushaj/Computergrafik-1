#include "CgRay.h"
#include "CgBase/CgEnums.h"
#include "CgUtils/ObjLoader.h"
#include <iostream>

CgRay::CgRay(int id):
t {1.0},
m_type(Cg::Polyline),
m_id(id),
m_line_width{1},
m_face_colors{Functions::getGreen()}
{
}

CgRay::CgRay(int id, std::vector<glm::vec3> vertices):
t {1.0},
m_type(Cg::Polyline),
m_id(id),
m_line_width{1},
m_face_colors{Functions::getGreen()}
{
    for(unsigned int i = 0; i< vertices.size(); i++) {
        m_vertices.push_back(vertices.at(i));
    }
}

CgRay::~CgRay(){
    m_vertices.clear();
}

const std::vector<glm::vec3>& CgRay::getVertices() const
{
    return m_vertices;
}

glm::vec3 CgRay::getColor() const {
    return m_face_colors;
}

unsigned int CgRay::getLineWidth() const {
    return m_line_width;
}

void CgRay::setVertices(std::vector<glm::vec3> newvertices){
    m_vertices.clear();
    for(unsigned int i = 0; i< newvertices.size(); ++i) {
        m_vertices.push_back(newvertices.at(i));
    }
}

void CgRay::addVertice(glm::vec3 vertex) {
    m_vertices.push_back(vertex);
}

glm::vec4 CgRay::getA() { return a; }
void CgRay::setA(glm::vec4 a) {
    this->a = a;
    this->a = 1/this->a[3] * this->a;
    tmp_a = this->a;
}
void CgRay::applyTransformationA(glm::mat4 matrix) {
    a = matrix * a;
    this->a = 1/this->a[3] * this->a;
}

glm::vec4 CgRay::getB() { return b; }
void CgRay::setB(glm::vec4 b) {
    this->b = b;
    this->b = 1/this->b[3] * this->b;
    tmp_b = this->b;
}
void CgRay::applyTransformationB(glm::mat4 matrix) {
    b = matrix * b;
    this->b = 1/this->b[3] * this->b;
    tmp_direction = this->direction;
}

glm::vec4 CgRay::getDirection() { return direction; }
void CgRay::setDirection(glm::vec4 direction) {
    this->direction = direction;
    glm::normalize(direction);
    createRay();
}

void CgRay::createRay() {
    std::vector<glm::vec3> tmp_vertices;
    tmp_vertices.push_back(glm::vec3(a[0], a[1], a[2]));
    tmp_vertices.push_back(glm::vec3(a[0] + t*direction[0], a[1] + t*direction[1], a[2] + t*direction[2]));
    setVertices(tmp_vertices);
}

void CgRay::secureWorldCoord() {
    tmp_a = a;
    tmp_b = b;
    tmp_direction = direction;
}

void CgRay::resetObjCoordToWorld() {
    a = tmp_a;
    b = tmp_b;
    direction = tmp_direction;
}
