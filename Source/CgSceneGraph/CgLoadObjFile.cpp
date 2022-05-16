#include "CgLoadObjFile.h"
#include "CgBase/CgEnums.h"
#include "CgUtils/ObjLoader.h"
#include <iostream>

CgLoadObjFile::CgLoadObjFile(int id):
m_type(Cg::TriangleMesh),
m_id(id)
{

}

CgLoadObjFile::CgLoadObjFile(int id, std::string filename):
m_type(Cg::TriangleMesh),
m_id(id)
{

    ObjLoader loader;
    loader.load(filename);

    loader.getPositionData(m_vertices);
    loader.getNormalData(m_vertex_normals);
    loader.getFaceIndexData(m_triangle_indices);
}

CgLoadObjFile::CgLoadObjFile(int id, std::vector<glm::vec3> arg_verts,  std::vector<glm::vec3> arg_normals, std::vector<unsigned int> arg_triangle_indices):
m_type(Cg::TriangleMesh),
m_id(id)
{
    m_vertices=arg_verts;
    m_vertex_normals=arg_normals;
    m_triangle_indices=arg_triangle_indices;

}


CgLoadObjFile::CgLoadObjFile(int id, std::vector<glm::vec3> arg_verts,  std::vector<unsigned int> arg_triangle_indices):
m_type(Cg::TriangleMesh),
m_id(id)
{
    m_vertices=arg_verts;
    m_triangle_indices=arg_triangle_indices;

    for (unsigned int i = 0; i < m_vertices.size(); i++) {
        m_vertex_normals.push_back(glm::vec3(0,0,0));
    }
    //dreieck Normalen + punkt normalen berechnen
    for (unsigned int i = 0; i < m_triangle_indices.size(); i+=3) {
        glm::vec3 vec1 = m_vertices[m_triangle_indices[i+1]] - m_vertices[m_triangle_indices[i]];
        glm::vec3 vec2 = m_vertices[m_triangle_indices[i+2]] - m_vertices[m_triangle_indices[i]];

        glm::vec3 normal = glm::cross(vec1, vec2);
        normal=glm::normalize(normal);
        m_face_normals.push_back(normal);

        m_vertex_normals.at(m_triangle_indices[i])      += normal;
        m_vertex_normals.at(m_triangle_indices[i+1])    += normal;
        m_vertex_normals.at(m_triangle_indices[i+2])    += normal;

    }

    for(unsigned int i = 0; i<m_vertex_normals.size(); i++){
        m_vertex_normals.at(i) = glm::normalize(m_vertex_normals.at(i));

    }

}



CgLoadObjFile::~CgLoadObjFile()
{
    m_vertices.clear();
    m_vertex_normals.clear();
    m_vertex_colors.clear();
    m_tex_coords.clear();
    m_triangle_indices.clear();
    m_face_normals.clear();
    m_face_colors.clear();
}

void CgLoadObjFile::init( std::vector<glm::vec3> arg_verts,  std::vector<glm::vec3> arg_normals, std::vector<unsigned int> arg_triangle_indices)
{
    m_vertices.clear();
    m_vertex_normals.clear();
    m_triangle_indices.clear();
    m_vertices=arg_verts;
    m_vertex_normals=arg_normals;
    m_triangle_indices=arg_triangle_indices;
}

void CgLoadObjFile::init( std::string filename)
{
    m_vertices.clear();
    m_vertex_normals.clear();
    m_triangle_indices.clear();

    ObjLoader loader;
    loader.load(filename);

    loader.getPositionData(m_vertices);
    loader.getNormalData(m_vertex_normals);
    loader.getFaceIndexData(m_triangle_indices);
}

const std::vector<glm::vec3>& CgLoadObjFile::getVertices() const
{
    return m_vertices;
}

const std::vector<glm::vec3>& CgLoadObjFile::getVertexNormals() const
{
    return m_vertex_normals;
}

const std::vector<glm::vec3>& CgLoadObjFile::getVertexColors() const
{
     return m_vertex_colors;
}

const std::vector<glm::vec2>& CgLoadObjFile:: getVertexTexCoords() const
{
    return m_tex_coords;
}

const std::vector<unsigned int>& CgLoadObjFile::getTriangleIndices() const
{
    return m_triangle_indices;
}

const std::vector<glm::vec3>& CgLoadObjFile::getFaceNormals() const
{
    return m_face_normals;
}

const std::vector<glm::vec3>& CgLoadObjFile::getFaceColors() const
{
    return m_face_colors;
}
