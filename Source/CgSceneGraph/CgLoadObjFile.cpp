#include "CgLoadObjFile.h"
#include "CgBase/CgEnums.h"
#include "CgUtils/ObjLoader.h"

CgLoadObjFile::CgLoadObjFile(int id):
m_type(Cg::TriangleMesh),
m_id(id)
{
}

CgLoadObjFile::CgLoadObjFile(int id, std::vector<glm::vec3> arg_verts,  std::vector<glm::vec3> arg_normals, std::vector<unsigned int> arg_triangle_indices):
m_type(Cg::TriangleMesh),
m_id(id)
{
    m_vertices.clear();
    m_vertex_normals.clear();
    m_triangle_indices.clear();

    m_vertices=arg_verts;
    m_vertex_normals=arg_normals;
    m_triangle_indices=arg_triangle_indices;

    //5.b)
    for(unsigned int i = 0; i < m_vertices.size(); i++){
        //Normale pro Punkt:  Mittelwert der Normalen aller Dreiecke berechnet, in denen der jeweils betrachtete Punkt vorkommt!
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
