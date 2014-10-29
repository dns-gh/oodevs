// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_AgentTypePionLOG_ABC.cpp $
// $Author: Jvt $
// $Modtime: 12/05/05 15:30 $
// $Revision: 9 $
// $Workfile: MIL_AgentTypePionLOG_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePionLOG_ABC.h"
#include "MIL_AgentPionLOG_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOG_ABC constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOG_ABC::MIL_AgentTypePionLOG_ABC( const std::string& strName, const std::string& strType, xml::xistream& xis )
    : MIL_AgentTypePion( strName, strType, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOG_ABC destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOG_ABC::~MIL_AgentTypePionLOG_ABC()
{
    // NOTHING
}

MIL_AgentPion* MIL_AgentTypePionLOG_ABC::InstanciatePion(
    MissionController_ABC& controller, MIL_Automate& automate, xml::xistream& xis ) const
{
    return new MIL_AgentPionLOG_ABC( *this, controller, automate, xis );
}
