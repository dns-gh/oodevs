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
