#ifndef AABB_H
#define AABB_H

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "CgBase/CgBaseTriangleMesh.h"

class CgAABB: public CgBaseTriangleMesh
{

public:
    CgAABB();
    CgAABB(int id);

    ~CgAABB();

    //inherited from CgBaseRenderableObject
    Cg::ObjectType getType() const override;
    unsigned int getID() const override;

    void init (std::vector<glm::vec3> arg_verts, std::vector<glm::vec3> arg_normals, std::vector<unsigned int> arg_triangle_indices);
    void init( std::string filename);

    //inherited from CgBaseTriangleMesh
    const std::vector<glm::vec3>& getVertices() const override;
    const std::vector<glm::vec3>& getVertexNormals() const override;
    const std::vector<glm::vec3>& getVertexColors() const override;
    const std::vector<glm::vec2>& getVertexTexCoords() const override;

    const std::vector<unsigned int>& getTriangleIndices() const override;

    const std::vector<glm::vec3>& getFaceNormals() const override;
    const std::vector<glm::vec3>& getFaceColors() const override;
    const std::vector<glm::vec3>& getFaceCentroid() const;

    void setVertices(float, float, float, float, float, float);
private:

    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_vertex_normals;
    std::vector<glm::vec3> m_vertex_colors;
    std::vector<glm::vec2> m_tex_coords;

    std::vector<unsigned int>  m_triangle_indices;

    std::vector<glm::vec3> m_face_normals;
    std::vector<glm::vec3> m_face_colors;
    std::vector<glm::vec3> m_face_centroid;

    const Cg::ObjectType m_type;
    const unsigned int m_id;
};


inline Cg::ObjectType  CgAABB::getType() const {return m_type;}
inline unsigned int CgAABB::getID() const {return m_id;}

#endif // AABB_H
