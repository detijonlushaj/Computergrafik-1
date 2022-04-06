#include "Functions.h"
#include <iostream>

std::vector<glm::vec3> Functions::Lane_Riesenfeld_Unterteilungs_Algorithmus(std::vector<glm::vec3> verticesold, int n) {
    // Unter 3 macht der Algorithmus keinen Sinn
    if(verticesold.size() < 3 || n == 0) return verticesold;

     std::vector<glm::vec3> *vertices {new std::vector<glm::vec3>()};
     for (std::vector<unsigned int>::size_type i = 0; i < verticesold.size(); i++)
     {
            vertices->push_back(verticesold.at(i));
     }

    int size = vertices->size();
    // Liste verdoppeln
    for (int i = 0; i < size; i++)
    {
        glm::vec3 vector = {verticesold.at(i)[0],verticesold.at(i)[1],verticesold.at(i)[2]};
        vertices->push_back(vector);
    }

    // Werte duplizieren und verschieben
    for (std::vector<unsigned int>::size_type i = 0; i < vertices->size(); i += 2)
    {
        vertices->at(vertices->size()-i-1) = vertices->at(size-1-(i/2) );
        vertices->at(vertices->size()-i-2) = vertices->at(size-1-(i/2) );
    }

    //2-4 Schritte sind ueblich!
    for (int j = 0; j < n; j++)
    {
        // Beim Mitteln darf Liste nicht kleiner sein als Ursprungsliste
        if (size + 1 >= (int) vertices->size()) {
            std::cout << "Ursprungslistegröße: " << size << ", Neue Liste Größe: " << vertices->size()
                      << "\t Neue Liste Größe sollte nicht kleiner gleich Ursprungsliste sein, deshalb wird abgebrochen" << std::endl;
        }

        // mitteln
        for (std::vector<unsigned int>::size_type i = 0; i < vertices->size() - 1; i++)
        {
            vertices->at(i) = (vertices->at(i) + vertices->at(i+1)) / 2.0f;
        }
        // letztes Element löschen
        vertices->erase(vertices->begin()+vertices->size()-1);
    }
    return *vertices;
}
