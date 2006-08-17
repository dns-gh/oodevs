// *****************************************************************************
//
// $Bypassd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Moving/PHY_Population_ActionFireOnPion.cpp $
// $Author: Nld $
// $Modtime: 16/03/05 16:04 $
// $Revision: 5 $
// $Workfile: PHY_Population_ActionFireOnPion.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_Population_ActionFireOnPion.h"

#include "MIL_AgentServer.h"
#include "Decision/DEC_Tools.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_Population.h"

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionFireOnPion constructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_Population_ActionFireOnPion::PHY_Population_ActionFireOnPion( MIL_Population& population, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC( population, diaCall )
    , population_   ( population )
    , fireResults_  ( population )
    , rIntensity_   ( diaCall.GetParameter( 0 ).ToFloat() )
    , pTarget_      ( 0 )
{    
    assert( rIntensity_ > 0. );
    assert( DEC_Tools::CheckTypePopulationConnaissanceAgent( diaCall.GetParameter( 1 ) ) );
    uint nID = ( uint )diaCall.GetParameter( 1 ).ToPtr();
    pTarget_ = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( nID ); // $$$$ HLA
    assert( pTarget_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionFireOnPion destructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_Population_ActionFireOnPion::~PHY_Population_ActionFireOnPion()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionFireOnPion::Execute
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_Population_ActionFireOnPion::Execute()
{   
    population_.FireOnPion( rIntensity_, *pTarget_, fireResults_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionFireOnPion::ExecuteSuspended
// Bypassd: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_Population_ActionFireOnPion::ExecuteSuspended()
{
    Execute();
}
