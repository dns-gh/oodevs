// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_PopulationInteraction.inl $
// $Author: Jvt $
// $Modtime: 17/03/05 12:45 $
// $Revision: 4 $
// $Workfile: DEC_KS_PopulationInteraction.inl $
//
// *****************************************************************************

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::NotifyPopulationCollision
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
inline
void DEC_KS_PopulationInteraction::NotifyPopulationCollision( MIL_PopulationFlow& flow )
{
    flowCollisions_.push_back( &flow );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::NotifyPopulationCollision
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
inline
void DEC_KS_PopulationInteraction::NotifyPopulationCollision( MIL_PopulationConcentration& concentration )
{
    concentrationCollisions_.push_back( &concentration );
}




