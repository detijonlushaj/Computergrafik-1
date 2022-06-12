#include "CgPlane.h"

CgPlane::CgPlane(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    n = glm::normalize(glm::cross(b - a, c - a));
    d = glm::dot(n, a);
}
