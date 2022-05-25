#include "CgAppearance.h"

CgAppearance::CgAppearance()
{

}
const glm::vec4& CgAppearance::getObjectColor() {
    return object_color;
}

void CgAppearance::setObjectColor(glm::vec4 color) {
    object_color = color;
    object_color /= 255.0;
}

glm::vec4& CgAppearance::getOldColor() { return old_color; }

void CgAppearance::setOldColor(glm::vec4 old_color) {
    this->old_color = old_color;
}
