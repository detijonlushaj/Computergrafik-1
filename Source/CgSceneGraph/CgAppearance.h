#ifndef CGAPPEARANCE_H
#define CGAPPEARANCE_H

#include <QObject>
#include <glm/glm.hpp>

class CgAppearance
{
private:
    glm::vec4 object_color;
    glm::vec4 old_color;

public:
    CgAppearance();

    const glm::vec4& getObjectColor();
    void setObjectColor(glm::vec4 color);
    void setObjectColorNoOldColorSave(glm::vec4 color);
    glm::vec4& getOldColor();
};

#endif // CGAPPEARANCE_H
