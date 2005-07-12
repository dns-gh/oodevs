// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Tc2/MIL_AgentTypePionLOGTC2.cpp $
// $Author: Nld $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGTC2.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_AgentTypePionLOGTC2.h"
#include "MIL_AgentPionLOGTC2.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGTC2 constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGTC2::MIL_AgentTypePionLOGTC2( const std::string& strName, MIL_InputArchive& archive )
    : MIL_AgentTypePionLOG_ABC( strName, archive )
{ 
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGTC2 destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGTC2::~MIL_AgentTypePionLOGTC2()
{

}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGTC2::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePionLOGTC2::InstanciatePion( MIL_Automate& automate, MIL_InputArchive& archive ) const
{
    return *new MIL_AgentPionLOGTC2( automate, archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGTC2::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePionLOGTC2::InstanciatePion( uint nID, MIL_InputArchive& archive ) const
{
    return *new MIL_AgentPionLOGTC2( *this, nID, archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGTC2::InstanciatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePionLOGTC2::InstanciatePion( uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition ) const
{
    return *new MIL_AgentPionLOGTC2( *this, nID, automate, vPosition );
}
