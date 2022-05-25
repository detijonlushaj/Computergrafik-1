#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "CgBase/CgBaseTriangleMesh.h"

#ifndef CGROTATION_H
#define CGROTATION_H


class CgRotation: public CgBaseTriangleMesh
{

public:
    CgRotation(int, std::vector<glm::vec3>, int);
    CgRotation(int id, std::vector<glm::vec3> indices, int originial_size, int n);

    ~CgRotation();

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


inline Cg::ObjectType  CgRotation::getType() const {return m_type;}
inline unsigned int CgRotation::getID() const {return m_id;}

#endif // CGRotation_H
