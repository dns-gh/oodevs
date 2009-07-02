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

#include "MIL.h"

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
             MIL_AgentTypePionLOG_ABC( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AgentTypePionLOG_ABC();
};

#include "MIL_AgentTypePionLOG_ABC.inl"

#endif // __MIL_AgentTypePionLOG_ABC_h_
