#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../CgSceneGraph/CgPolyline.h"

class CgPolyline;

class Functions
{
public:
    static std::vector<glm::vec3> Lane_Riesenfeld_Unterteilungs_Algorithmus(std::vector<glm::vec3> vertices, int n);
    static int getId();
    static bool XOR(bool a, bool b);
    static std::string getPathtoObj(std::string obj);
    static glm::vec4 getWhite();
    static glm::vec4 getRed();
    static glm::vec4 getGreen();
    static glm::vec4 getBlue();
    static glm::vec4 getYellow();
    static glm::vec4 getPink();
    static glm::vec4 getChessPink();
    static glm::vec4 getChessBlue();


    static int getWidth();
    static void setWidth(int value);

    static int getHeight();
    static void setHeight(int value);

    static std::vector<glm::vec3> getPickingRay();
    static void setPickingRay(const std::vector<glm::vec3> value);

    static CgPolyline* getPolylineRay();
    static void setPolylineRay(CgPolyline *value);

private:
    static int unique_id;
    static glm::vec4 white;
    static glm::vec4 red;
    static glm::vec4 green;
    static glm::vec4 blue;
    static glm::vec4 yellow;
    static glm::vec4 pink;
    static glm::vec4 chessPink;
    static glm::vec4 chessBlue;

    static int width;
    static int height;
};

#endif // FUNCTIONS_H
