#include "CgAppearance.h"

CgAppearance::CgAppearance()
{

}
const glm::vec4& CgAppearance::getObjectColor() {
    return object_color;
}

void CgAppearance::setObjectColor(glm::vec4 color) {
    old_color = object_color;
    object_color = color;
    object_color /= 255.0;
}

void CgAppearance::setObjectColorNoOldColorSave(glm::vec4 color) {
    object_color = color;
    object_color /= 255.0;
}

glm::vec4& CgAppearance::getOldColor() { return old_color; }
