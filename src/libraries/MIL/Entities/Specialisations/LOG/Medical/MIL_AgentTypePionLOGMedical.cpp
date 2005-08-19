// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Medical/MIL_AgentTypePionLOGMedical.cpp $
// $Author: Nld $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGMedical.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_AgentTypePionLOGMedical.h"
#include "MIL_AgentPionLOGMedical.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMedical constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGMedical::MIL_AgentTypePionLOGMedical( const std::string& strName, MIL_InputArchive& archive )
    : MIL_AgentTypePionLOG_ABC( strName, archive )
{ 
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMedical destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGMedical::~MIL_AgentTypePionLOGMedical()
{

}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMedical::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePionLOGMedical::InstanciatePion( MIL_Automate& automate, MIL_InputArchive& archive ) const
{
    return *new MIL_AgentPionLOGMedical( automate, archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMedical::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePionLOGMedical::InstanciatePion( uint nID, MIL_InputArchive& archive ) const
{
    return *new MIL_AgentPionLOGMedical( *this, nID, archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMedical::InstanciatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePionLOGMedical::InstanciatePion( uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition ) const
{
    return *new MIL_AgentPionLOGMedical( *this, nID, automate, vPosition );
}
