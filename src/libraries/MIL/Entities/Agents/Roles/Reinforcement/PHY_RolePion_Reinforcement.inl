// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_RolePion_Reinforcement.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyReinforcementAdded
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Reinforcement::NotifyReinforcementAdded( MIL_AgentPion& reinforcement )
{
    bool bOut = reinforcements_.insert( &reinforcement ).second;
    assert( bOut );
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyReinforcementRemoved
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Reinforcement::NotifyReinforcementRemoved( MIL_AgentPion& reinforcement )
{
    int nOut = reinforcements_.erase( &reinforcement );
    assert( nOut == 1 );
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::IsReinforcing
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Reinforcement::IsReinforcing() const
{
    return pPionReinforced_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::IsReinforced
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Reinforcement::IsReinforced() const
{
    return !reinforcements_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::HasChanged
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Reinforcement::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::GetReinforcements
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
const PHY_RolePion_Reinforcement::T_PionSet& PHY_RolePion_Reinforcement::GetReinforcements() const
{
    return reinforcements_;
}

