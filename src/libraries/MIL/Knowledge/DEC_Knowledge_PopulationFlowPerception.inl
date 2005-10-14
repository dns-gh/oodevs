// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationFlowPerception.inl $
// $Author: Jvt $
// $Modtime: 17/03/05 12:34 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_PopulationFlowPerception.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetFlowPerceived
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
const MIL_PopulationFlow& DEC_Knowledge_PopulationFlowPerception::GetFlowPerceived() const
{
    assert( pPopulationFlowPerceived_ );
    return *pPopulationFlowPerceived_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetCurrentPerceptionLevel
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
inline
const PHY_PerceptionLevel& DEC_Knowledge_PopulationFlowPerception::GetCurrentPerceptionLevel() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetKnowledge
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
inline
const DEC_Knowledge_PopulationPerception& DEC_Knowledge_PopulationFlowPerception::GetKnowledge() const
{
    assert( pPopulationKnowledge_ );
    return *pPopulationKnowledge_;
}

