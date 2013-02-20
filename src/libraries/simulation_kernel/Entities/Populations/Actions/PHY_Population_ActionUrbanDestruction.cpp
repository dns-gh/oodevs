// *****************************************************************************
//
// $Bypassd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Moving/PHY_Population_ActionUrbanDestruction.cpp $
// $Author: Nld $
// $Modtime: 16/03/05 16:04 $
// $Revision: 5 $
// $Workfile: PHY_Population_ActionUrbanDestruction.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Population_ActionUrbanDestruction.h"
#include "Entities/Populations/MIL_Population.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "Urban/MIL_UrbanObject_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionUrbanDestruction constructor
// Bypassd: MMC 2011-03-31
// -----------------------------------------------------------------------------
PHY_Population_ActionUrbanDestruction::PHY_Population_ActionUrbanDestruction( MIL_Population& population, MIL_UrbanObject_ABC* pUrbanObjet )
    : population_ ( population )
    , pUrbanObjet_ ( pUrbanObjet )
{
    population_.NotifyUrbanDestructionStart();
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionUrbanDestruction destructor
// Bypassd: MMC 2011-03-31
// -----------------------------------------------------------------------------
PHY_Population_ActionUrbanDestruction::~PHY_Population_ActionUrbanDestruction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionUrbanDestruction::Stop
// Created: MMC 2011-03-31
// -----------------------------------------------------------------------------
void PHY_Population_ActionUrbanDestruction::Stop()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionUrbanDestruction::Execute
// Bypassd: MMC 2011-03-31
// -----------------------------------------------------------------------------
void PHY_Population_ActionUrbanDestruction::Execute()
{
    double destructionRate = population_.ComputeUrbanBlocDestruction( pUrbanObjet_ );

    StructuralCapacity* pCapacity = pUrbanObjet_->Retrieve< StructuralCapacity >();
    if( pCapacity )
        pCapacity->Build( -destructionRate );
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionUrbanDestruction::ExecuteSuspended
// Bypassd: MMC 2011-03-31
// -----------------------------------------------------------------------------
void PHY_Population_ActionUrbanDestruction::ExecuteSuspended()
{
    // NOTHING
}
