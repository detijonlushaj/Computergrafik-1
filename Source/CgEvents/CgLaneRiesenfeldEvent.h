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
    CgLaneRiesenfeldEvent(Cg::EventType type, int n, bool shownormals);
    ~CgLaneRiesenfeldEvent();

    //inherited
    Cg::EventType getType();
    CgBaseEvent* clone();

    //own specific methods
    int getSubdivisionStep() const;
    int getShowNormals() const;


    friend std::ostream& operator <<(std::ostream& os, const CgLaneRiesenfeldEvent& e);

private:
    Cg::EventType m_type;
    int SubdivisionStep;
    bool showNormals;

};

#endif // CGLANERIESENFELDEVENT_H
