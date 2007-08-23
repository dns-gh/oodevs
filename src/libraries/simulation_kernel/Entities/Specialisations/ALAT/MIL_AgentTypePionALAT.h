// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Alat/MIL_AgentTypePionALAT.h $
// $Author: Nld $
// $Modtime: 7/03/05 10:50 $
// $Revision: 4 $
// $Workfile: MIL_AgentTypePionALAT.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionALAT_h_
#define __MIL_AgentTypePionALAT_h_

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
class MIL_AgentTypePionALAT : public MIL_AgentTypePion
{
    MT_COPYNOTALLOWED( MIL_AgentTypePionALAT )

public:
    MIL_AgentTypePionALAT( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AgentTypePionALAT();

public:
    static const MIL_AgentTypePion* Create( const std::string& strName, xml::xistream& xis );
};

#include "MIL_AgentTypePionALAT.inl"

#endif // __MIL_AgentTypePionALAT_h_
