#include "Functions.h"
#include <iostream>

int Functions::unique_id {0};
glm::vec4 Functions::white {glm::vec4(255.0, 255.0, 255.0, 1.0)};
glm::vec4 Functions::red {glm::vec4(255.0, 0.0, 0.0, 1.0)};
glm::vec4 Functions::green {glm::vec4(0.0, 255.0, 0.0, 1.0)};
glm::vec4 Functions::blue {glm::vec4(0.0, 255.0, 255.0, 1.0)};
glm::vec4 Functions::yellow {glm::vec4(255.0, 255.0, 0.0, 1.0)};
glm::vec4 Functions::pink {glm::vec4(237.0, 36.0, 207.0, 1.0)};
glm::vec4 Functions::chessPink {glm::vec4(255.0, 100.0, 255.0, 1.0)};
glm::vec4 Functions::chessBlue {glm::vec4(100.0, 100.0, 255.0, 1.0)};

int Functions::getId()
{
    ++unique_id;
    return unique_id;
}

glm::vec4 Functions::getWhite() { return white; }
glm::vec4 Functions::getRed() { return red; }
glm::vec4 Functions::getGreen() { return green; }
glm::vec4 Functions::getBlue() { return blue; }
glm::vec4 Functions::getYellow() { return yellow; }
glm::vec4 Functions::getPink() { return pink; }
glm::vec4 Functions::getChessPink() { return chessPink; }
glm::vec4 Functions::getChessBlue() { return chessBlue; }

bool Functions::XOR(bool a, bool b)
{
    return (a + b) % 2;
}

std::vector<glm::vec3> Functions::Lane_Riesenfeld_Unterteilungs_Algorithmus(std::vector<glm::vec3> verticesold, int n) {
    // Unter 3 macht der Algorithmus keinen Sinn
    if(verticesold.size() < 3 || n == 0) return verticesold;

     std::vector<glm::vec3> vertices;
     for (unsigned int i = 0; i < verticesold.size(); ++i) {
         vertices.push_back(glm::vec3(verticesold.at(i)[0], verticesold.at(i)[1], verticesold.at(i)[2]));
    }

    int size = vertices.size();
    // Liste verdoppeln
    for (int i = 0; i < size; ++i)
    {
        vertices.push_back(glm::vec3(0.0, 0.0, 0.0));
    }

    // Werte duplizieren und verschieben
    for (unsigned int i = 0; i < vertices.size(); i += 2)
    {
        vertices.at(vertices.size()-i-1) = vertices.at(size-1-(i/2));
        vertices.at(vertices.size()-i-2) = vertices.at(size-1-(i/2));
    }

    //2-4 Schritte sind ueblich!
    for (int j = 0; j < n; ++j)
    {
        // Beim Mitteln darf Liste nicht kleiner sein als Ursprungsliste
        if (size + 1 >= (int) vertices.size()) {
            std::cout << "Ursprungslistegröße: " << size << ", Neue Liste Größe: " << vertices.size()
                      << "\t Neue Liste Größe sollte nicht kleiner gleich Ursprungsliste sein, deshalb wird abgebrochen" << std::endl;
            break;
        }

        // mitteln
        for (unsigned int i = 0; i < vertices.size() - 1; ++i)
        {
            vertices.at(i) = (vertices.at(i) + vertices.at(i+1)) / 2.0f;
        }
        // letztes Element löschen
        vertices.erase(vertices.begin()+vertices.size()-1);
    }
    return vertices;
}
std::string Functions::getPathtoObj(std::string s){
    return "/home/uni/Desktop/cg1/Source/CgData/" + s;
}

