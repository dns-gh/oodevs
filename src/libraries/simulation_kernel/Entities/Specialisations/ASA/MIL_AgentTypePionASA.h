// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Asa/MIL_AgentTypePionASA.h $
// $Author: Nld $
// $Modtime: 23/12/04 11:05 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionASA.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionASA_h_
#define __MIL_AgentTypePionASA_h_

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
class MIL_AgentTypePionASA : public MIL_AgentTypePion
{
    MT_COPYNOTALLOWED( MIL_AgentTypePionASA )

public:
    MIL_AgentTypePionASA( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AgentTypePionASA();

public:
    static const MIL_AgentTypePion* Create( const std::string& strName, xml::xistream& xis );
};

#include "MIL_AgentTypePionASA.inl"

#endif // __MIL_AgentTypePionASA_h_
