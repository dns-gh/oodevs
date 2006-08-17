//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_Perception_ABC.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_Perception_ABC::PHY_Perception_ABC( PHY_RolePion_Perceiver& perceiver )
    : perceiver_( perceiver )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_Perception_ABC::~PHY_Perception_ABC()
{

}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::GetPerceiverPosition
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_Perception_ABC::GetPerceiverPosition() const
{
    return perceiver_.GetPion().GetRole< PHY_RolePion_Location >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::GetPerceiverDirection
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_Perception_ABC::GetPerceiverDirection() const
{
    return perceiver_.GetPion().GetRole< PHY_RolePion_Location >().GetDirection();
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::Compute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_Perception_ABC::Compute( const MT_Vector2D& /*vPoint*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::Execute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void PHY_Perception_ABC::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::Compute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_Perception_ABC::Compute( const MIL_Agent_ABC& /*agent*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::Compute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_Perception_ABC::Compute( const DEC_Knowledge_Agent& /*knowledge*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::Execute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void PHY_Perception_ABC::Execute( const TER_Object_ABC::T_ObjectVector& /*perceivableObjects*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::Compute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_Perception_ABC::Compute( const DEC_Knowledge_Object& /*knowledge*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::Compute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_Perception_ABC::Compute( const MIL_RealObject_ABC& /*object*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::Execute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void PHY_Perception_ABC::Execute( const TER_PopulationFlow_ABC::T_PopulationFlowVector& /*perceivableFlows*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::Compute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_Perception_ABC::Compute( const MIL_PopulationConcentration& /*concentration*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::Execute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void PHY_Perception_ABC::Execute( const TER_PopulationConcentration_ABC::T_PopulationConcentrationVector /*perceivableConcentrations*/ )
{
    // NOTHING
}

