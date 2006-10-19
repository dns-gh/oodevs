// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Medical/MIL_AgentTypePionLOGMedical.h $
// $Author: Nld $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGMedical.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionLOGMedical_h_
#define __MIL_AgentTypePionLOGMedical_h_

#include "MIL.h"

#include "Entities/Specialisations/LOG/MIL_AgentTypePionLOG_ABC.h"

// =============================================================================
// @class  MIL_AgentTypePionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentTypePionLOGMedical : public MIL_AgentTypePionLOG_ABC
{
    MT_COPYNOTALLOWED( MIL_AgentTypePionLOGMedical )

public:
    MIL_AgentTypePionLOGMedical( const std::string& strName, MIL_InputArchive& archive );
    virtual ~MIL_AgentTypePionLOGMedical();

    //! @name Instanciation
    //@{
    virtual MIL_AgentPion& InstanciatePion( uint nID, MIL_Automate& automate, MIL_InputArchive& archive ) const;
    virtual MIL_AgentPion& InstanciatePion( uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition ) const; 
    //@}

public:
    static const MIL_AgentTypePion* Create( const std::string& strName, MIL_InputArchive& archive );
};

#include "MIL_AgentTypePionLOGMedical.inl"

#endif // __MIL_AgentTypePionLOGMedical_h_
