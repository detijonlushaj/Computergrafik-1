#ifndef CGTRANSLATIONEVENT_H
#define CGTRANSLATIONEVENT_H

#include "../CgBase/CgBaseEvent.h"
#include "../CgBase/CgEnums.h"
#include <string>
#include <iostream>
#include <glm/glm.hpp>

class CgTranslationEvent : public CgBaseEvent
{
public:
    CgTranslationEvent(Cg::EventType type,int x, int y, int z, bool group);
    ~CgTranslationEvent();

    //inherited
    Cg::EventType getType();
    CgBaseEvent* clone();

    glm::vec4 getVector() const;
    void setVector(const glm::vec4 &value);

    friend std::ostream& operator <<(std::ostream& os, const CgTranslationEvent& e);

    bool getTranslate_grp() const;
    void setTranslate_grp(bool value);

private:
    Cg::EventType m_type;
    glm::vec4 vector;
    bool translate_grp;
};

#endif // CGTRANSLATIONEVENT_H

