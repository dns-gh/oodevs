// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/MIL_AgentTypePionLOG_ABC.h $
// $Author: Nld $
// $Modtime: 5/01/05 17:09 $
// $Revision: 1 $
// $Workfile: MIL_AgentTypePionLOG_ABC.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionLOG_ABC_h_
#define __MIL_AgentTypePionLOG_ABC_h_

#include "Entities/Agents/MIL_AgentTypePion.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  MIL_AgentTypePionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentTypePionLOG_ABC : public MIL_AgentTypePion
{
public:
             MIL_AgentTypePionLOG_ABC( const std::string& strName, const std::string& strType, xml::xistream& xis );
    virtual ~MIL_AgentTypePionLOG_ABC();

    virtual MIL_AgentPion* InstanciatePion( MissionController_ABC& controller,
            MIL_Automate& automate, xml::xistream& xis ) const;
    virtual std::string GetArchetypeName() const;
    virtual void RegisterFunctions( sword::Brain& brain, MIL_Agent_ABC& agent ) const;

};

#endif // __MIL_AgentTypePionLOG_ABC_h_
