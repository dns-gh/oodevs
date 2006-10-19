// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Convoy/MIL_AgentTypePionLOGConvoy.cpp $
// $Author: Nld $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGConvoy.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentTypePionLOGConvoy.h"
#include "MIL_AgentPionLOGConvoy.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGConvoy constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGConvoy::MIL_AgentTypePionLOGConvoy( const std::string& strName, MIL_InputArchive& archive )
    : MIL_AgentTypePionLOG_ABC( strName, archive )
{ 
    // $$$ ?? Checker que le type ne contient aucun equipement ?
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGConvoy destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGConvoy::~MIL_AgentTypePionLOGConvoy()
{

}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGConvoy::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePionLOGConvoy::InstanciatePion( uint nID, MIL_Automate& automate, MIL_InputArchive& archive ) const
{
    return *new MIL_AgentPionLOGConvoy( *this, nID, automate, archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGConvoy::InstanciatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePionLOGConvoy::InstanciatePion( uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition ) const
{
    return *new MIL_AgentPionLOGConvoy( *this, nID, automate, vPosition );
}
