// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Rens/MIL_AgentTypePionRENS.h $
// $Author: Nld $
// $Modtime: 23/12/04 11:05 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionRENS.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionRENS_h_
#define __MIL_AgentTypePionRENS_h_

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
class MIL_AgentTypePionRENS : public MIL_AgentTypePion
{
    MT_COPYNOTALLOWED( MIL_AgentTypePionRENS )

public:
    MIL_AgentTypePionRENS( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AgentTypePionRENS();

public:
    static const MIL_AgentTypePion* Create( const std::string& strName, xml::xistream& xis );
};

#include "MIL_AgentTypePionRENS.inl"

#endif // __MIL_AgentTypePionRENS_h_
