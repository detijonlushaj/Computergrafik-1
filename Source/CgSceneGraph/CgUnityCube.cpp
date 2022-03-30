#include "CgUnityCube.h"
#include "CgBase/CgEnums.h"
#include "CgUtils/ObjLoader.h"
#include <iostream>


CgUnityCube::CgUnityCube(int id): m_type(Cg::TriangleMesh), m_id(id)
{

    // Vorderseite
    m_vertices.push_back(glm::vec3(-sqrt(2)/2,0.0,0.5));  // linke ecke x, y, z
    m_vertices.push_back(glm::vec3(0.0,-sqrt(2)/2,0.5));  // untere ecke
    m_vertices.push_back(glm::vec3(0.0,sqrt(2)/2,0.5));   // obere ecke
    m_vertices.push_back(glm::vec3(sqrt(2)/2,0.0,0.5));   // rechte ecke

    // Rückseite
    m_vertices.push_back(glm::vec3(-sqrt(2)/2,0.0,-0.5));
    m_vertices.push_back(glm::vec3(0.0,-sqrt(2)/2,-0.5));
    m_vertices.push_back(glm::vec3(0.0,sqrt(2)/2,-0.5));
    m_vertices.push_back(glm::vec3(sqrt(2)/2,0.0,-0.5));

    // Umlaufsinn gegen den Uhrzeiger
    // Fläche Vorderseite:
    // links, unten, oben
    m_triangle_indices.push_back(0);
    m_triangle_indices.push_back(1);
    m_triangle_indices.push_back(2);
    // oben, unten, rechts
    m_triangle_indices.push_back(2);
    m_triangle_indices.push_back(1);
    m_triangle_indices.push_back(3);

    // Fläche Rückseite:
    // unten, links, oben
    m_triangle_indices.push_back(5);
    m_triangle_indices.push_back(4);
    m_triangle_indices.push_back(6);
    // oben, rechts, unten
    m_triangle_indices.push_back(6);
    m_triangle_indices.push_back(7);
    m_triangle_indices.push_back(5);

    // Fläche linke untere Seite:
    // untern vorne, links vorne, unten hinten
    m_triangle_indices.push_back(1);
    m_triangle_indices.push_back(0);
    m_triangle_indices.push_back(5);
    // links vorne, links hinten, unten hinten
    m_triangle_indices.push_back(0);
    m_triangle_indices.push_back(4);
    m_triangle_indices.push_back(5);

    // Fläche linke obere Seite
    // links vorne, oben vorne, links hinten
    m_triangle_indices.push_back(0);
    m_triangle_indices.push_back(2);
    m_triangle_indices.push_back(4);
    // oben hinten, oben vorne, links hinten
    m_triangle_indices.push_back(6);
    m_triangle_indices.push_back(2);
    m_triangle_indices.push_back(4);

    // Fläche rechte untere Seite
    // rechts vorne, unten vorne, unten hinten
    m_triangle_indices.push_back(3);
    m_triangle_indices.push_back(1);
    m_triangle_indices.push_back(5);
    // rechts hinten, unten hinten, rechts vorne
    m_triangle_indices.push_back(7);
    m_triangle_indices.push_back(5);
    m_triangle_indices.push_back(3);

    // Fläche rechte obere Seite
    // rechts vorne, rechts hinten, oben vorne
    m_triangle_indices.push_back(3);
    m_triangle_indices.push_back(7);
    m_triangle_indices.push_back(2);

    // rechts hinten, oben hinten, oben vorne
    m_triangle_indices.push_back(7);
    m_triangle_indices.push_back(6);
    m_triangle_indices.push_back(2);

    // Normalen & Schwerpunkt berechnen
    for (int i = 0; i < m_triangle_indices.size(); i+=3) {
        glm::vec3 vec1 = m_vertices[m_triangle_indices[i+1]] - m_vertices[m_triangle_indices[i]];
        glm::vec3 vec2 = m_vertices[m_triangle_indices[i+2]] - m_vertices[m_triangle_indices[i]];
        glm::vec3 normal = glm::cross(vec1, vec2);
        normal=glm::normalize(normal);

      // glm::vec3 normal = (glm::vec3)glm::cross(vec1, vec2);
//        glm::vec3 normal = glm::vec3(vec1[1]vec2[2]-vec1[2]vec2[1], vec1[3]vec2[1]-vec1[1]vec2[3], vec1[0]vec2[1]-vec1[1]vec2[0]);
//        std::cout << vec1[1]vec2[2]-vec1[2]vec2[1] << " " << vec1[3]vec2[1]-vec1[1]vec2[3] << " " << vec1[0]vec2[1]-vec1[1]vec2[0];
//        std::cout << normal[0] << " " << normal[1] << " "<< normal[2] << std::endl;

        std::cout << normal[0] << " " << normal[1] << " "<< normal[2] << std::endl;
        m_face_normals.push_back(normal);

        // vec3 besteht aus floats nicht doubles
        glm::vec3 vec_centroid = (m_vertices[m_triangle_indices[i]] + m_vertices[m_triangle_indices[i+1]] + m_vertices[m_triangle_indices[i+2]]) / (3.0f);
        m_face_centroid.push_back(vec_centroid);
    }

}



CgUnityCube::~CgUnityCube()
{
    m_vertices.clear();
    m_vertex_normals.clear();
    m_vertex_colors.clear();
    m_tex_coords.clear();
    m_triangle_indices.clear();
    m_face_normals.clear();
    m_face_colors.clear();
}

void CgUnityCube::init( std::vector<glm::vec3> arg_verts,  std::vector<glm::vec3> arg_normals, std::vector<unsigned int> arg_triangle_indices)
{
    m_vertices.clear();
    m_vertex_normals.clear();
    m_triangle_indices.clear();
    m_vertices=arg_verts;
    m_vertex_normals=arg_normals;
    m_triangle_indices=arg_triangle_indices;
}

void CgUnityCube::init( std::string filename)
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

const std::vector<glm::vec3>& CgUnityCube::getVertices() const
{
    return m_vertices;
}

const std::vector<glm::vec3>& CgUnityCube::getVertexNormals() const
{
    return m_vertex_normals;
}

const std::vector<glm::vec3>& CgUnityCube::getVertexColors() const
{
     return m_vertex_colors;
}

const std::vector<glm::vec2>& CgUnityCube::getVertexTexCoords() const
{
    return m_tex_coords;
}

const std::vector<unsigned int>& CgUnityCube::getTriangleIndices() const
{
    return m_triangle_indices;
}

const std::vector<glm::vec3>& CgUnityCube::getFaceNormals() const
{
    return m_face_normals;
}

const std::vector<glm::vec3>& CgUnityCube::getFaceColors() const
{
    return m_face_colors;
}

const std::vector<glm::vec3>& CgUnityCube::getFaceCentroid() const
{
    return m_face_centroid;
}
