// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Medical/MIL_AgentTypeInjuredHumanPion.cpp $
// $Author: Rft $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypeInjuredHumanPion.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentTypeInjuredHumanPion.h"
#include "MIL_AgentInjuredHumanPion.h"
#include "AlgorithmsFactories.h"
#include "Entities/Agents/MIL_AgentTypePion.h"

#include "xeumeuleu/xml.h"



// -----------------------------------------------------------------------------
// Name: MIL_AgentTypeInjuredHumanPion constructor
// Created: RFT 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypeInjuredHumanPion::MIL_AgentTypeInjuredHumanPion( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePion( strName, xis )
{ 
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypeInjuredHumanPion destructor
// Created: RFT 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypeInjuredHumanPion::~MIL_AgentTypeInjuredHumanPion()
{
    //NOTHING
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypeInjuredHumanPion::InstanciatePion
// Created: RFT 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AgentTypeInjuredHumanPion::InstanciatePion( uint nID, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis ) const
{
    return new MIL_AgentInjuredHumanPion( *this, nID, automate, algorithmFactories, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypeInjuredHumanPion::InstanciatePion
// Created: RFT 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AgentTypeInjuredHumanPion::InstanciatePion( uint nID, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories ) const
{
    return new MIL_AgentInjuredHumanPion( *this, nID, automate, algorithmFactories );
}

const MIL_AgentTypePion* MIL_AgentTypeInjuredHumanPion::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AgentTypeInjuredHumanPion( strName, xis );
}