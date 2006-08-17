// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationPerception.inl $
// $Author: Jvt $
// $Modtime: 17/03/05 12:34 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_PopulationPerception.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::GetPopulationPerceived
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
inline
MIL_Population& DEC_Knowledge_PopulationPerception::GetPopulationPerceived() const
{
    assert( pPopulationPerceived_ );
    return *pPopulationPerceived_; 
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::GetPopulationPerceiving
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
inline
const MIL_AgentPion& DEC_Knowledge_PopulationPerception::GetAgentPerceiving() const
{
    assert( pAgentPerceiving_ );
    return *pAgentPerceiving_;    
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::NotifyAttacker
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_PopulationPerception::NotifyAttacker()
{
    bAttacker_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::IsAttacker
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_PopulationPerception::IsAttacker() const
{
    return bAttacker_;
}
