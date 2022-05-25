#ifndef COORDSYSTEM_H
#define COORDSYSTEM_H

#include "CgPolyline.h"
#include "../CgUtils/Functions.h"

class CgPolyline;

class CgCoordSystem {

private:
    std::vector<CgPolyline*> coord_system;
    std::vector<glm::vec4> color_system;

public:
    CgCoordSystem();
    CgCoordSystem(std::vector<CgPolyline*> polylines);
    ~CgCoordSystem();

    const std::vector<CgPolyline*>& getCoordSystem() const;
    const std::vector<glm::vec4>& getColorSystem() const;
};

#endif // COORDSYSTEM_H
