// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/DEC_AutomateDecision.inl $
// $Author: Jvt $
// $Modtime: 5/04/05 14:34 $
// $Revision: 5 $
// $Workfile: DEC_AutomateDecision.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Clean
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
inline
void DEC_AutomateDecision::Clean()
{
    bStateHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::HasStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
inline
bool DEC_AutomateDecision::HasStateChanged() const
{
    return bStateHasChanged_;
}

// =============================================================================
// NOTIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::NotifyForceRatioStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
inline
void DEC_AutomateDecision::NotifyForceRatioStateChanged( E_ForceRatioState nState )
{
    if( nForceRatioState_ != nState )
    {
        nForceRatioState_ = nState;
        bStateHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::NotifyRulesOfEngagementStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
inline
void DEC_AutomateDecision::NotifyRulesOfEngagementStateChanged( E_RulesOfEngagementState nState )
{
    if( nRulesOfEngagementState_ != nState )
    {
        nRulesOfEngagementState_ = nState;
        bStateHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::NotifyCloseCombatStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
inline
void DEC_AutomateDecision::NotifyCloseCombatStateChanged( E_CloseCombatState nState )
{
    if( nCloseCombatState_ != nState )
    {
        nCloseCombatState_ = nState;
        bStateHasChanged_  = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::NotifyOperationalStateChanged
// Created: NLD 2005-07-26
// -----------------------------------------------------------------------------
inline
void DEC_AutomateDecision::NotifyOperationalStateChanged( E_OperationalState nState )
{
    if( nOperationalState_ != nState )
    {
        nOperationalState_ = nState;
        bStateHasChanged_  = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetAutomate
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
inline
MIL_Automate& DEC_AutomateDecision::GetAutomate() const
{
    assert( pAutomate_ );
    return *pAutomate_;
}
