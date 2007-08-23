// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Circulation/MIL_AgentTypePionCIRCULATION.h $
// $Author: Nld $
// $Modtime: 10/03/05 11:15 $
// $Revision: 1 $
// $Workfile: MIL_AgentTypePionCIRCULATION.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionCIRCULATION_h_
#define __MIL_AgentTypePionCIRCULATION_h_

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
class MIL_AgentTypePionCIRCULATION : public MIL_AgentTypePion
{
    MT_COPYNOTALLOWED( MIL_AgentTypePionCIRCULATION )

public:
    MIL_AgentTypePionCIRCULATION( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AgentTypePionCIRCULATION();

public:
    static const MIL_AgentTypePion* Create( const std::string& strName, xml::xistream& xis );
};

#include "MIL_AgentTypePionCIRCULATION.inl"

#endif // __MIL_AgentTypePionCIRCULATION_h_
