#ifndef __StubMIL_Automate_h_
#define __StubMIL_Automate_h_

#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/DEC_AutomateDecision.h"

class StubMIL_Automate : public MIL_Automate
{
public:
    explicit StubMIL_Automate( MIL_AutomateType& type ) : MIL_Automate( type )
    {
        RegisterRole( *new DEC_AutomateDecision( *this, 100, 100, false ) ) ;
    }
    virtual ~StubMIL_Automate() {}
};

#endif
