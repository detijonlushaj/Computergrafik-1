#include "CgLaneRiesenfeldEvent.h"

CgLaneRiesenfeldEvent::CgLaneRiesenfeldEvent()
{
}

CgLaneRiesenfeldEvent::CgLaneRiesenfeldEvent(Cg::EventType type, int n, bool show, bool reset):
m_type(type),
SubdivisionStep(n),
m_showNormals(show),
m_reset(reset)
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
    return new CgLaneRiesenfeldEvent(m_type, SubdivisionStep, m_showNormals, m_reset);
}


//own specific methods
int CgLaneRiesenfeldEvent::getSubdivisionStep() const{
    return SubdivisionStep;
}

int CgLaneRiesenfeldEvent::getShowNormals() const{
    return m_showNormals;
}

bool CgLaneRiesenfeldEvent::getReset() const {
    return m_reset;
}

std::ostream& operator<<(std::ostream& os,const CgLaneRiesenfeldEvent& e)
{
    os << "Type: "<< e.m_type << "\t (SubdivisionStep:" << e.getSubdivisionStep() << "\t normals: " << e.getShowNormals() << "\t reset: " << e.getReset() <<")";
    return os;
}

