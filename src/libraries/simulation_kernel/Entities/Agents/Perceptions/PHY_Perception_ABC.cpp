//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Perception_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_Perception_ABC::PHY_Perception_ABC( PHY_RoleInterface_Perceiver& perceiver )
    : perceiver_( perceiver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_Perception_ABC::~PHY_Perception_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::GetPerceiverPosition
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_Perception_ABC::GetPerceiverPosition() const
{
    return perceiver_.GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::GetPerceiverDirection
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_Perception_ABC::GetPerceiverDirection() const
{
    return perceiver_.GetPion().GetRole< PHY_RoleInterface_Location >().GetDirection();
}

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
void PHY_Perception_ABC::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/, const detection::DetectionComputerFactory_ABC& /*detectionComputerFactory*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::Compute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_Perception_ABC::Compute( const MIL_Agent_ABC& /*agent*/ )
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
const PHY_PerceptionLevel& PHY_Perception_ABC::Compute( const MIL_Object_ABC& /*object*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::Execute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void PHY_Perception_ABC::Execute( const TER_PopulationFlow_ABC::T_ConstPopulationFlowVector& /*perceivableFlows*/ )
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
void PHY_Perception_ABC::Execute( const TER_PopulationConcentration_ABC::T_ConstPopulationConcentrationVector /*perceivableConcentrations*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Perception_ABC::GetPerceptionId
// Created: LDC 2009-07-21
// -----------------------------------------------------------------------------
int PHY_Perception_ABC::GetPerceptionId()
{
    static int id = 0;
    return id++;
}
