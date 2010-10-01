// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Nbc/MIL_AgentTypePionNBC.h $
// $Author: Nld $
// $Modtime: 23/12/04 11:05 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionNBC.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionNBC_h_
#define __MIL_AgentTypePionNBC_h_

#include "Entities/Agents/MIL_AgentTypePion.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  MIL_AgentTypePionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentTypePionNBC : public MIL_AgentTypePion
{
public:
             MIL_AgentTypePionNBC( const std::string& strName, const std::string& strType, xml::xistream& xis );
    virtual ~MIL_AgentTypePionNBC();

    virtual void RegisterFunctions( directia::brain::Brain& brain, MIL_Agent_ABC& agent ) const;

    static const MIL_AgentTypePion* Create( const std::string& strName, const std::string& strType, xml::xistream& xis );
};

#endif // __MIL_AgentTypePionNBC_h_
