#ifndef CGAPPEARANCE_H
#define CGAPPEARANCE_H

#include <QObject>
#include <glm/glm.hpp>

class CgAppearance
{
private:
    glm::vec4 object_color;
    /*
    glm::mat4 object_scale;
    glm::mat4 object_translation;
    glm::mat4 object_rotation;
    */

public:
    CgAppearance();

    const glm::vec4& getObjectColor();
    void setObjectColor(glm::vec4 color);
    /*
    const glm::mat4& getObjectScale();
    void setObjectScale(glm::mat4 scale_matrix);

    const glm::mat4& getObjectTranslation();
    void setObjectTranslation(glm::mat4 translation_matrix);

    const glm::mat4& getObjectRotation();
    void setObjectRotation(glm::mat4 rotation_matrix);
    */
};

#endif // CGAPPEARANCE_H
