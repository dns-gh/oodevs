#ifndef __StubMIL_AutomateType_h_
#define __StubMIL_AutomateType_h_

#include "Entities/Automates/MIL_AutomateType.h"

class StubMIL_AutomateType : public MIL_AutomateType
{
public:
    StubMIL_AutomateType( const DEC_Model_ABC& model ) : MIL_AutomateType( model ){}
};

#endif