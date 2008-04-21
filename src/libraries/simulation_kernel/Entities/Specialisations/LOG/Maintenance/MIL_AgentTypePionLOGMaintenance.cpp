// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Maintenance/MIL_AgentTypePionLOGMaintenance.cpp $
// $Author: Nld $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGMaintenance.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentTypePionLOGMaintenance.h"
#include "MIL_AgentPionLOGMaintenance.h"

#include "xeumeuleu/xml.h"



// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMaintenance constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGMaintenance::MIL_AgentTypePionLOGMaintenance( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePionLOG_ABC( strName, xis )
{ 
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMaintenance destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGMaintenance::~MIL_AgentTypePionLOGMaintenance()
{

}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMaintenance::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePionLOGMaintenance::InstanciatePion( uint nID, MIL_Automate& automate, xml::xistream& xis ) const
{
    return *new MIL_AgentPionLOGMaintenance( *this, nID, automate, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMaintenance::InstanciatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePionLOGMaintenance::InstanciatePion( uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition ) const
{
    return *new MIL_AgentPionLOGMaintenance( *this, nID, automate, vPosition );
}
