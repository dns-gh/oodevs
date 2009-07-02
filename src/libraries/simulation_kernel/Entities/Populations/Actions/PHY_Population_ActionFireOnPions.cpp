// *****************************************************************************
//
// $Bypassd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Moving/PHY_Population_ActionFireOnPions.cpp $
// $Author: Nld $
// $Modtime: 16/03/05 16:04 $
// $Revision: 5 $
// $Workfile: PHY_Population_ActionFireOnPions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Population_ActionFireOnPions.h"
#include "Entities/Populations/MIL_Population.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionFireOnPions constructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_Population_ActionFireOnPions::PHY_Population_ActionFireOnPions( MIL_Population& population, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC( population, diaCall )
    , population_   ( population )
    , fireResults_  ( population )
    , rIntensity_   ( diaCall.GetParameter( 0 ).ToFloat() )
{    
    assert( rIntensity_ > 0. );
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionFireOnPions destructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_Population_ActionFireOnPions::~PHY_Population_ActionFireOnPions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionFireOnPions::Execute
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_Population_ActionFireOnPions::Execute()
{   
    population_.FireOnPions( rIntensity_, fireResults_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionFireOnPions::ExecuteSuspended
// Bypassd: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_Population_ActionFireOnPions::ExecuteSuspended()
{
    Execute();
}
