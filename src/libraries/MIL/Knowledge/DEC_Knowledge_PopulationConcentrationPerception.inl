// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationConcentrationPerception.inl $
// $Author: Jvt $
// $Modtime: 17/03/05 12:34 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_PopulationConcentrationPerception.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::GetConcentrationPerceived
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
const MIL_PopulationConcentration& DEC_Knowledge_PopulationConcentrationPerception::GetConcentrationPerceived() const
{
    assert( pPopulationConcentrationPerceived_ );
    return *pPopulationConcentrationPerceived_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::GetCurrentPerceptionLevel
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
inline
const PHY_PerceptionLevel& DEC_Knowledge_PopulationConcentrationPerception::GetCurrentPerceptionLevel() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_;
}
