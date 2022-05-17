#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "CgBase/CgBaseTriangleMesh.h"

#ifndef CGUNITYCUBE_H
#define CGUNITYCUBE_H


class CgUnityCube: public CgBaseTriangleMesh
{

public:
    CgUnityCube();
    CgUnityCube(int id);
    CgUnityCube(int id,double a);


    ~CgUnityCube();

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


inline Cg::ObjectType  CgUnityCube::getType() const {return m_type;}
inline unsigned int CgUnityCube::getID() const {return m_id;}

#endif // CGUNITYCUBE_H
