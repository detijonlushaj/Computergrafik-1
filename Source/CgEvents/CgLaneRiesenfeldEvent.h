#ifndef CGLANERIESENFELDEVENT_H
#define CGLANERIESENFELDEVENT_H

#include "../CgBase/CgBaseEvent.h"
#include "../CgBase/CgEnums.h"
#include <string>
#include <iostream>

class CgLaneRiesenfeldEvent : public CgBaseEvent
{

public:
    CgLaneRiesenfeldEvent();
    CgLaneRiesenfeldEvent(Cg::EventType type, int n);
    ~CgLaneRiesenfeldEvent();

    //inherited
    Cg::EventType getType();
    CgBaseEvent* clone();

    //own specific methods
    int getSubdivisionStep() const;


    friend std::ostream& operator <<(std::ostream& os, const CgLaneRiesenfeldEvent& e);

private:
    Cg::EventType m_type;
    int SubdivisionStep;

};

#endif // CGLANERIESENFELDEVENT_H
