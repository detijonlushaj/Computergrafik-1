#ifndef CGROTATIONEVENT_H
#define CGROTATIONEVENT_H


#include "../CgBase/CgBaseEvent.h"
#include "../CgBase/CgEnums.h"
#include <string>
#include <iostream>


class CgRotationEvent : public CgBaseEvent
{

public:
    CgRotationEvent();
    CgRotationEvent(Cg::EventType type, int n , bool show);
    ~CgRotationEvent();

    //inherited
    Cg::EventType getType();
    CgBaseEvent* clone();

    //own specific methods
    int getRotatorischeSegmente() const;
    bool getNormals() const;


    friend std::ostream& operator <<(std::ostream& os, const CgRotationEvent& e);

private:
    Cg::EventType m_type;
    int rotatorische_Segmente;
    bool m_showNormals;
};

#endif // CGROTATIONEVENT_H


