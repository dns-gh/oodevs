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

#include "simulation_kernel_pch.h"

#include "MIL_AgentTypePionLOGTC2.h"
#include "MIL_AgentPionLOGTC2.h"

#include <xeumeuleu/xml.h>



// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGTC2 constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGTC2::MIL_AgentTypePionLOGTC2( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePionLOG_ABC( strName, xis )
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
MIL_AgentPion& MIL_AgentTypePionLOGTC2::InstanciatePion( uint nID, MIL_Automate& automate, xml::xistream& xis ) const
{
    return *new MIL_AgentPionLOGTC2( *this, nID, automate, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGTC2::InstanciatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePionLOGTC2::InstanciatePion( uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition ) const
{
    return *new MIL_AgentPionLOGTC2( *this, nID, automate, vPosition );
}
