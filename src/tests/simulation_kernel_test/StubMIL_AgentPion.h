#ifndef __StubMIL_AgentPion_h_
#define __StubMIL_AgentPion_h_

#include "Entities/Agents/MIL_AgentPion.h"

class StubMIL_AgentPion : public MIL_AgentPion
{
public:
    StubMIL_AgentPion( MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis,
                       const MIL_Config& config ) :
                 MIL_AgentPion( type, automate, algorithmFactories, xis, config ) {}
    virtual ~StubMIL_AgentPion() {}
};

#endif
