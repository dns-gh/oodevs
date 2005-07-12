//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#include "MIL_pch.h"

#include "PHY_Perception_ABC.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"

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


