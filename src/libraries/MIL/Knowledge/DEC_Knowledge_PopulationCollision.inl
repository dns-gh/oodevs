// *****************************************************************************
//
// $Created: NLD 2004-05-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationCollision.inl $
// $Author: Jvt $
// $Modtime: 16/03/05 19:36 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_PopulationCollision.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::GetPopulation
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
inline
MIL_Population& DEC_Knowledge_PopulationCollision::GetPopulation() const
{
    assert( pPopulation_ );
    return *pPopulation_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::GetAgentColliding
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
inline
const MIL_AgentPion& DEC_Knowledge_PopulationCollision::GetAgentColliding() const
{
    assert( pAgentColliding_ );
    return *pAgentColliding_;   
}  

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_PopulationCollision::Prepare()
{
    bIsValid_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_PopulationCollision::Clean()
{
    return !bIsValid_;
}
