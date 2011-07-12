#ifndef __StubMIL_AgentTypePion_h_
#define __StubMIL_AgentTypePion_h_

#include "Entities/Agents/MIL_AgentTypePion.h"

class StubMIL_AgentTypePion : public MIL_AgentTypePion
{
public:
             StubMIL_AgentTypePion( DEC_Model_ABC& model ) : MIL_AgentTypePion( &model ) {}
    virtual ~StubMIL_AgentTypePion() {}
};

#endif
