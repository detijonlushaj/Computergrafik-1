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

/*
const glm::mat4& CgAppearance::getObjectScale() {
    return object_scale;
}

void CgAppearance::setObjectScale(glm::mat4 scale_matrix) {
    object_scale = scale_matrix;
}

const glm::mat4& CgAppearance::getObjectTranslation() {
    return object_translation;
}

void CgAppearance::setObjectTranslation(glm::mat4 translation_matrix) {
    object_translation = translation_matrix;
}

const glm::mat4& CgAppearance::getObjectRotation() {
    return object_rotation;
}

void CgAppearance::setObjectRotation(glm::mat4 rotation_matrix) {
    object_rotation = rotation_matrix;
}
*/
