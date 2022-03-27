#include "CgColorChangeEvent.h"

CgColorChangeEvent::CgColorChangeEvent()
{

}

CgColorChangeEvent::CgColorChangeEvent(Cg::EventType type, int red, int green, int blue)
{
    m_type = type;
    m_red = red;
    m_green = green;
    m_blue = blue;
}

CgColorChangeEvent::~CgColorChangeEvent()
{

}

Cg::EventType  CgColorChangeEvent::getType()
{
    return m_type;
}

CgBaseEvent* CgColorChangeEvent::clone()
{
    return new CgColorChangeEvent(m_type, m_red, m_green, m_blue);
}

int CgColorChangeEvent::getRed() const {
    return m_red;
}

int CgColorChangeEvent::getGreen() const {
    return m_green;
}

int CgColorChangeEvent::getBlue() const {
    return m_blue;
}

std::ostream& operator<<(std::ostream& os,const CgColorChangeEvent& e)
{
    os << "Type: "<< e.m_type << " Colors: (" << e.getRed() << ", " << e.getGreen() << ", " << e.getBlue() << ")";
    return os;
}
