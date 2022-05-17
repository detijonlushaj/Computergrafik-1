#include "CgTranslationEvent.h"

CgTranslationEvent::CgTranslationEvent(Cg::EventType type,int x, int y, int z, bool group):
m_type(type)
{
    setVector(glm::vec4(x,y,z,1));
    setTranslate_grp(group);
}
CgTranslationEvent::~CgTranslationEvent(){
}

glm::vec4 CgTranslationEvent::getVector() const
{
    return vector;
}

void CgTranslationEvent::setVector(const glm::vec4 &value)
{
    vector = value;
}

bool CgTranslationEvent::getTranslate_grp() const
{
    return translate_grp;
}

void CgTranslationEvent::setTranslate_grp(bool value)
{
    translate_grp = value;
}

//inherited
Cg::EventType CgTranslationEvent::getType()
{
    return m_type;
}

CgBaseEvent* CgTranslationEvent::clone()
{
    return new CgTranslationEvent(m_type, vector[0],vector[1],vector[2],getTranslate_grp());
}


std::ostream& operator<<(std::ostream& os,const CgTranslationEvent& e)
{
    os << "Type: "<< e.m_type << "s";
    return os;
}
