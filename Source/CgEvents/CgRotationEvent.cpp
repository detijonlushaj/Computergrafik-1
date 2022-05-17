#include "CgRotationEvent.h"

CgRotationEvent::CgRotationEvent()
{
}

CgRotationEvent::CgRotationEvent(Cg::EventType type, int n, bool show):
m_type(type),
rotatorische_Segmente(n),
m_showNormals(show)
{
}
CgRotationEvent::~CgRotationEvent(){
}

//inherited
Cg::EventType CgRotationEvent::getType()
{
    return m_type;
}

CgBaseEvent* CgRotationEvent::clone()
{
    return new CgRotationEvent(m_type, rotatorische_Segmente,m_showNormals);
}


//own specific methods
int CgRotationEvent::getRotatorischeSegmente() const{
    return rotatorische_Segmente;
}

bool CgRotationEvent::getNormals() const{
    return m_showNormals;
}

std::ostream& operator<<(std::ostream& os,const CgRotationEvent& e)
{
    os << "Type: "<< e.m_type << "\t (Anzahl rotatorische Segmente:" << e.getRotatorischeSegmente() << "\t show normals: " << e.getNormals()<<")";
    return os;
}
