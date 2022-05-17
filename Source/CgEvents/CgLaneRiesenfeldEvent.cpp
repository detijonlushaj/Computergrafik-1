#include "CgLaneRiesenfeldEvent.h"

CgLaneRiesenfeldEvent::CgLaneRiesenfeldEvent()
{
}

CgLaneRiesenfeldEvent::CgLaneRiesenfeldEvent(Cg::EventType type, int n):
m_type(type),
SubdivisionStep(n)
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
    return new CgLaneRiesenfeldEvent(m_type, SubdivisionStep);
}


//own specific methods
int CgLaneRiesenfeldEvent::getSubdivisionStep() const{
    return SubdivisionStep;
}


std::ostream& operator<<(std::ostream& os,const CgLaneRiesenfeldEvent& e)
{
    os << "Type: "<< e.m_type << "\t (SubdivisionStep:" << e.getSubdivisionStep() <<")";
    return os;
}
