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

#include "simulation_kernel_pch.h"

#include "MIL_AgentTypePionLOGMedical.h"
#include "MIL_AgentPionLOGMedical.h"

#include "xeumeuleu/xml.h"



// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMedical constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGMedical::MIL_AgentTypePionLOGMedical( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePionLOG_ABC( strName, xis )
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
MIL_AgentPion& MIL_AgentTypePionLOGMedical::InstanciatePion( uint nID, MIL_Automate& automate, xml::xistream& xis ) const
{
    return *new MIL_AgentPionLOGMedical( *this, nID, automate, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMedical::InstanciatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePionLOGMedical::InstanciatePion( uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition ) const
{
    return *new MIL_AgentPionLOGMedical( *this, nID, automate, vPosition );
}
