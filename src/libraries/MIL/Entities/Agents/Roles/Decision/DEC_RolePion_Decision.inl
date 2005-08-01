// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Decision/DEC_RolePion_Decision.inl $
// $Author: Jvt $
// $Modtime: 1/04/05 14:03 $
// $Revision: 5 $
// $Workfile: DEC_RolePion_Decision.inl $
//
// *****************************************************************************


//-----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetBehaviorPart
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
inline
DIA_BehaviorPart& DEC_RolePion_Decision::GetBehaviorPart() const
{
    assert( pBehaviorTool_ != 0 );
    return( static_cast< DIA_BehaviorPart& >( *pBehaviorTool_ ) );
}

//-----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetKnowledgePart
// Created: AGN 02-12-19
//-----------------------------------------------------------------------------
inline
DIA_Knowledge_Part& DEC_RolePion_Decision::GetKnowledgePart() const
{
    assert( pKnowledgeTool_ != 0 );
    return( static_cast< DIA_Knowledge_Part& > (*pKnowledgeTool_) );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetPion
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
MIL_AgentPion& DEC_RolePion_Decision::GetPion() const
{
    assert( pPion_ );
    return *pPion_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::Clean
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
inline
void DEC_RolePion_Decision::Clean()
{
    bStateHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::HasStateChanged
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
inline
bool DEC_RolePion_Decision::HasStateChanged() const
{
    return bStateHasChanged_;
}

// =============================================================================
// NOTIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyForceRatioStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
inline
void DEC_RolePion_Decision::NotifyForceRatioStateChanged( E_ForceRatioState nState )
{
    if( nForceRatioState_ != nState )
    {
        nForceRatioState_ = nState;
        bStateHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyRulesOfEngagementStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
inline
void DEC_RolePion_Decision::NotifyRulesOfEngagementStateChanged( E_RulesOfEngagementState nState )
{
    if( nRulesOfEngagementState_ != nState )
    {
        nRulesOfEngagementState_ = nState;
        bStateHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyCloseCombatStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
inline
void DEC_RolePion_Decision::NotifyCloseCombatStateChanged( E_CloseCombatState nState )
{
    if( nCloseCombatState_ != nState )
    {
        nCloseCombatState_ = nState;
        bStateHasChanged_  = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyOperationalStateChanged
// Created: NLD 2005-07-26
// -----------------------------------------------------------------------------
inline
void DEC_RolePion_Decision::NotifyOperationalStateChanged( E_OperationalState nState )
{
    if( nOperationalState_ != nState )
    {
        nOperationalState_ = nState;
        bStateHasChanged_  = true;
    }
}

