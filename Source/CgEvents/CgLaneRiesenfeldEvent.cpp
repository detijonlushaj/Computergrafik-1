#include "CgLaneRiesenfeldEvent.h"

CgLaneRiesenfeldEvent::CgLaneRiesenfeldEvent()
{
}

CgLaneRiesenfeldEvent::CgLaneRiesenfeldEvent(Cg::EventType type, int n, bool show):
m_type(type),
SubdivisionStep(n),
m_showNormals(show)
{
}
CgLaneRiesenfeldEvent::~CgLaneRiesenfeldEvent(){
}

//inherited
Cg::EventType CgLaneRiesenfeldEvent::getType()
{
    return m_type;
}

CgBaseEvent* CgLaneRiesenfeldEvent::clone()
{
    return new CgLaneRiesenfeldEvent(m_type, SubdivisionStep, m_showNormals);
}


//own specific methods
int CgLaneRiesenfeldEvent::getSubdivisionStep() const{
    return SubdivisionStep;
}

int CgLaneRiesenfeldEvent::getShowNormals() const{
    return m_showNormals;
}


std::ostream& operator<<(std::ostream& os,const CgLaneRiesenfeldEvent& e)
{
    os << "Type: "<< e.m_type << "\t (SubdivisionStep:" << e.getSubdivisionStep() << "\t normals: " << e.getShowNormals() <<")";
    return os;
}

