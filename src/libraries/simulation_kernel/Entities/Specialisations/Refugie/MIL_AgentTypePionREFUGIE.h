// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Refugie/MIL_AgentTypePionREFUGIE.h $
// $Author: Nld $
// $Modtime: 10/03/05 16:07 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionREFUGIE.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionREFUGIE_h_
#define __MIL_AgentTypePionREFUGIE_h_

#include "MIL.h"

#include "Entities/Agents/MIL_AgentTypePion.h"

// =============================================================================
// @class  MIL_AgentTypePionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentTypePionREFUGIE : public MIL_AgentTypePion
{
    MT_COPYNOTALLOWED( MIL_AgentTypePionREFUGIE )

public:
    MIL_AgentTypePionREFUGIE( const std::string& strName, MIL_InputArchive& archive );
    virtual ~MIL_AgentTypePionREFUGIE();

    //! @name Accessors
    //@{
    virtual bool IsRefugee() const;
    //@}
    
public:
    static const MIL_AgentTypePion* Create( const std::string& strName, MIL_InputArchive& archive );
};

#include "MIL_AgentTypePionREFUGIE.inl"

#endif // __MIL_AgentTypePionREFUGIE_h_
