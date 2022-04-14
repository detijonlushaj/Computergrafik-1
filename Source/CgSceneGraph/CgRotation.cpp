#include "CgRotation.h"
#include "CgBase/CgEnums.h"
#include "CgUtils/ObjLoader.h"
#include <iostream>

CgRotation::CgRotation(int id, std::vector<glm::vec3> indices, int originial_size): m_type(Cg::TriangleMesh), m_id(id)
{
    m_vertices.clear();

    for (unsigned int i = 0; i < indices.size(); ++i) {
        m_vertices.push_back(indices.at(i));
    }
    int pos;

    for (unsigned int i = 0; i < (m_vertices.size()/originial_size)-1; i+=1) {      // i-tes Segment
        for (int j = 0; j < originial_size - 1; ++j) {                              // pos j im i-ten segment
            pos = i * originial_size + j;

            m_triangle_indices.push_back(pos);
            m_triangle_indices.push_back(pos + 1);
            m_triangle_indices.push_back(pos + originial_size + 1);

            m_triangle_indices.push_back(pos);
            m_triangle_indices.push_back(pos + originial_size + 1);
            m_triangle_indices.push_back(pos + originial_size);
        }
    }

     //Normalen & Schwerpunkt berechnen
    for (int i = 0; i < m_triangle_indices.size(); i+=3) {
        glm::vec3 vec1 = m_vertices[m_triangle_indices[i+1]] - m_vertices[m_triangle_indices[i]];
        glm::vec3 vec2 = m_vertices[m_triangle_indices[i+2]] - m_vertices[m_triangle_indices[i]];

        glm::vec3 normal = glm::cross(vec1, vec2);
        normal=glm::normalize(normal);
        m_face_normals.push_back(normal);

        // vec3 besteht aus floats nicht doubles
        glm::vec3 vec_centroid = (m_vertices[m_triangle_indices[i]] + m_vertices[m_triangle_indices[i+1]] + m_vertices[m_triangle_indices[i+2]]) / (3.0f);
        m_face_centroid.push_back(vec_centroid);
    }

}

CgRotation::~CgRotation()
{
    m_vertices.clear();
    m_vertex_normals.clear();
    m_vertex_colors.clear();
    m_tex_coords.clear();
    m_triangle_indices.clear();
    m_face_normals.clear();
    m_face_colors.clear();
}

void CgRotation::init( std::vector<glm::vec3> arg_verts,  std::vector<glm::vec3> arg_normals, std::vector<unsigned int> arg_triangle_indices)
{
    m_vertices.clear();
    m_vertex_normals.clear();
    m_triangle_indices.clear();
    m_vertices=arg_verts;
    m_vertex_normals=arg_normals;
    m_triangle_indices=arg_triangle_indices;
}

void CgRotation::init( std::string filename)
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

const std::vector<glm::vec3>& CgRotation::getVertices() const
{
    return m_vertices;
}

const std::vector<glm::vec3>& CgRotation::getVertexNormals() const
{
    return m_vertex_normals;
}

const std::vector<glm::vec3>& CgRotation::getVertexColors() const
{
     return m_vertex_colors;
}

const std::vector<glm::vec2>& CgRotation::getVertexTexCoords() const
{
    return m_tex_coords;
}

const std::vector<unsigned int>& CgRotation::getTriangleIndices() const
{
    return m_triangle_indices;
}

const std::vector<glm::vec3>& CgRotation::getFaceNormals() const
{
    return m_face_normals;
}

const std::vector<glm::vec3>& CgRotation::getFaceColors() const
{
    return m_face_colors;
}

const std::vector<glm::vec3>& CgRotation::getFaceCentroid() const
{
    return m_face_centroid;
}
