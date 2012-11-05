// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Circulation/MIL_AgentTypePionCirculation.h $
// $Author: Nld $
// $Modtime: 10/03/05 11:15 $
// $Revision: 1 $
// $Workfile: MIL_AgentTypePionCirculation.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionCirculation_h_
#define __MIL_AgentTypePionCirculation_h_

#include "Entities/Agents/MIL_AgentTypePion.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  MIL_AgentTypePionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentTypePionCirculation : public MIL_AgentTypePion
{
public:
             MIL_AgentTypePionCirculation( const std::string& strName, const std::string& strType, xml::xistream& xis );
    virtual ~MIL_AgentTypePionCirculation();

    virtual void RegisterFunctions( sword::Brain& brain, MIL_Agent_ABC& agent ) const;

    static const MIL_AgentTypePion* Create( const std::string& strName, const std::string& strType, xml::xistream& xis );
};

#endif // __MIL_AgentTypePionCirculation_h_
