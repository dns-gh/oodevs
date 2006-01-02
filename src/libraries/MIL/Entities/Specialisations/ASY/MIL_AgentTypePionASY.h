// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/ASY/MIL_AgentTypePionASY.h $
// $Author: Nld $
// $Modtime: 10/03/05 16:07 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionASY.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionASY_h_
#define __MIL_AgentTypePionASY_h_

#include "MIL.h"

#include "Entities/Agents/MIL_AgentTypePion.h"

// =============================================================================
// @class  MIL_AgentTypePionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentTypePionASY : public MIL_AgentTypePion
{
    MT_COPYNOTALLOWED( MIL_AgentTypePionASY )

public:
    MIL_AgentTypePionASY( const std::string& strName, MIL_InputArchive& archive );
    virtual ~MIL_AgentTypePionASY();


public:
    static const MIL_AgentTypePion* Create( const std::string& strName, MIL_InputArchive& archive );
};

#include "MIL_AgentTypePionASY.inl"

#endif // __MIL_AgentTypePionASY_h_
