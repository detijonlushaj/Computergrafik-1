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
    CgLaneRiesenfeldEvent(Cg::EventType type, int n, bool m_shownormals, bool m_reset);
    ~CgLaneRiesenfeldEvent();

    //inherited
    Cg::EventType getType();
    CgBaseEvent* clone();

    //own specific methods
    int getSubdivisionStep() const;
    int getShowNormals() const;
    bool getReset() const;


    friend std::ostream& operator <<(std::ostream& os, const CgLaneRiesenfeldEvent& e);

private:
    Cg::EventType m_type;
    int SubdivisionStep;
    bool m_showNormals;
    bool m_reset;

};

#endif // CGLANERIESENFELDEVENT_H
