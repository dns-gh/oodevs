// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Populations/DEC_PopulationDecision.inl $
// $Author: Jvt $
// $Modtime: 5/04/05 14:34 $
// $Revision: 5 $
// $Workfile: DEC_PopulationDecision.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetDominationState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PopulationDecision::GetDominationState() const
{
    return rDominationState_;
}

//-----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetBehaviorPart
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
inline
DIA_BehaviorPart& DEC_PopulationDecision::GetBehaviorPart() const
{
    assert( pBehaviorTool_ != 0 );
    return( static_cast< DIA_BehaviorPart& >( *pBehaviorTool_ ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetPopulation
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
inline
MIL_Population& DEC_PopulationDecision::GetPopulation() const
{
    assert( pPopulation_ );
    return *pPopulation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::NotifyDominationStateChanged
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
inline
void DEC_PopulationDecision::NotifyDominationStateChanged( MT_Float rValue )
{
    assert( rValue >= 0. && rValue <= 1. );
    if( rDominationState_ == rValue )
        return;

    rDominationState_ = rValue;

    static const MT_Float rDeltaPercentageForNetwork = 0.05;
    if( fabs( rLastDominationState_ - rDominationState_ ) > rDeltaPercentageForNetwork || rDominationState_ == 0. || rDominationState_ == 1. )
        bStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::Clean
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
inline
void DEC_PopulationDecision::Clean()
{
    bStateHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::HasStateChanged
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
inline
bool DEC_PopulationDecision::HasStateChanged() const
{
    return bStateHasChanged_;
}
