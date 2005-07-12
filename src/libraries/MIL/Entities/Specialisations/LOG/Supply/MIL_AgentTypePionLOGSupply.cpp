// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Supply/MIL_AgentTypePionLOGSupply.cpp $
// $Author: Nld $
// $Modtime: 8/02/05 17:54 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGSupply.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_AgentTypePionLOGSupply.h"
#include "MIL_AgentPionLOGSupply.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGSupply constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGSupply::MIL_AgentTypePionLOGSupply( const std::string& strName, MIL_InputArchive& archive )
    : MIL_AgentTypePionLOG_ABC( strName, archive )
{ 
}



// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGSupply destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGSupply::~MIL_AgentTypePionLOGSupply()
{

}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGSupply::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePionLOGSupply::InstanciatePion( MIL_Automate& automate, MIL_InputArchive& archive ) const
{
    return *new MIL_AgentPionLOGSupply( automate, archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGSupply::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePionLOGSupply::InstanciatePion( uint nID, MIL_InputArchive& archive ) const
{
    return *new MIL_AgentPionLOGSupply( *this, nID, archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGSupply::InstanciatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePionLOGSupply::InstanciatePion( uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition ) const
{
    return *new MIL_AgentPionLOGSupply( *this, nID, automate, vPosition );
}
