#include "CgCoordSystem.h"

CgCoordSystem::CgCoordSystem() {
}

CgCoordSystem::CgCoordSystem(std::vector<CgPolyline*> polylines) {
    coord_system = polylines;
    color_system.push_back(Functions::getRed());
    color_system.push_back(Functions::getYellow());
    color_system.push_back(Functions::getBlue());
}

CgCoordSystem::~CgCoordSystem() {
    for (unsigned int i=0; i<coord_system.size(); ++i) {
        if (coord_system[i] != NULL) {
            delete coord_system[i];
        }
    }
}

const std::vector<CgPolyline*>& CgCoordSystem::getCoordSystem() const {
    return coord_system;
}

const std::vector<glm::vec4>& CgCoordSystem::getColorSystem() const {
    return color_system;
}
