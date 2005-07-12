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

#include "MIL.h"

#include "Entities/Agents/MIL_AgentTypePion.h"

// =============================================================================
// @class  MIL_AgentTypePionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentTypePionNBC : public MIL_AgentTypePion
{
    MT_COPYNOTALLOWED( MIL_AgentTypePionNBC )

public:
    MIL_AgentTypePionNBC( const std::string& strName, MIL_InputArchive& archive );
    virtual ~MIL_AgentTypePionNBC();

public:
    static const MIL_AgentTypePion* Create( const std::string& strName, MIL_InputArchive& archive );
};

#include "MIL_AgentTypePionNBC.inl"

#endif // __MIL_AgentTypePionNBC_h_
