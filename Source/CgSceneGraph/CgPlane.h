#ifndef CGPLANE_H
#define CGPLANE_H

#include <glm/glm.hpp>


class CgPlane
{
public:
    CgPlane(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    glm::vec3 n;
    float d;
};

#endif // CGPLANE_H
