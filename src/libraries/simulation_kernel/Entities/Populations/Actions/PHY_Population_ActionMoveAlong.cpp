#include "simulation_kernel_pch.h"
#include "PHY_Population_ActionMoveAlong.h"
#include "Entities/Populations/MIL_Population.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionMoveAlong constructor
// Created: LDC 2013-03-29
// -----------------------------------------------------------------------------
PHY_Population_ActionMoveAlong::PHY_Population_ActionMoveAlong( MIL_Population& population, const std::vector< boost::shared_ptr< MT_Vector2D > >& destination )
    : population_( population )
    , destination_( destination )
{
    population_.CancelMove();
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionMoveAlong destructor
// Created: LDC 2013-03-29
// -----------------------------------------------------------------------------
PHY_Population_ActionMoveAlong::~PHY_Population_ActionMoveAlong()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionMoveAlong::Execute
// Created: LDC 2013-03-29
// -----------------------------------------------------------------------------
void PHY_Population_ActionMoveAlong::Execute()
{
    population_.MoveAlong( destination_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionMoveAlong::ExecuteSuspended
// Created: LDC 2013-03-29
// -----------------------------------------------------------------------------
void PHY_Population_ActionMoveAlong::ExecuteSuspended()
{
    // NOTHING
}
