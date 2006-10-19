// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Supply/MIL_AgentTypePionLOGSupply.h $
// $Author: Nld $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGSupply.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionLOGSupply_h_
#define __MIL_AgentTypePionLOGSupply_h_

#include "MIL.h"

#include "Entities/Specialisations/LOG/MIL_AgentTypePionLOG_ABC.h"

// =============================================================================
// @class  MIL_AgentTypePionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentTypePionLOGSupply : public MIL_AgentTypePionLOG_ABC
{
    MT_COPYNOTALLOWED( MIL_AgentTypePionLOGSupply )

public:
    MIL_AgentTypePionLOGSupply( const std::string& strName, MIL_InputArchive& archive );
    virtual ~MIL_AgentTypePionLOGSupply();

    //! @name Instanciation
    //@{
    virtual MIL_AgentPion& InstanciatePion( uint nID, MIL_Automate& automate, MIL_InputArchive& archive ) const;
    virtual MIL_AgentPion& InstanciatePion( uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition ) const; 
    //@}

public:
    static const MIL_AgentTypePion* Create( const std::string& strName, MIL_InputArchive& archive );
};

#include "MIL_AgentTypePionLOGSupply.inl"

#endif // __MIL_AgentTypePionLOGSupply_h_
