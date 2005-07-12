// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Milice/MIL_AgentTypePionMILICE.h $
// $Author: Nld $
// $Modtime: 10/03/05 16:07 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionMILICE.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionMILICE_h_
#define __MIL_AgentTypePionMILICE_h_

#include "MIL.h"

#include "Entities/Agents/MIL_AgentTypePion.h"

// =============================================================================
// @class  MIL_AgentTypePionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentTypePionMILICE : public MIL_AgentTypePion
{
    MT_COPYNOTALLOWED( MIL_AgentTypePionMILICE )

public:
    MIL_AgentTypePionMILICE( const std::string& strName, MIL_InputArchive& archive );
    virtual ~MIL_AgentTypePionMILICE();

    //! @name Accessors
    //@{
    virtual bool IsMilitia() const;
    //@}

public:
    static const MIL_AgentTypePion* Create( const std::string& strName, MIL_InputArchive& archive );
};

#include "MIL_AgentTypePionMILICE.inl"

#endif // __MIL_AgentTypePionMILICE_h_
