// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_Population.inl $
// $Author: Jvt $
// $Modtime: 17/03/05 12:34 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_Population.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetID
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
uint DEC_Knowledge_Population::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetKnowledgeGroup
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
const MIL_KnowledgeGroup& DEC_Knowledge_Population::GetKnowledgeGroup() const
{
    assert( pKnowledgeGroup_ );
    return *pKnowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetPopulationKnown
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
MIL_Population& DEC_Knowledge_Population::GetPopulationKnown() const
{
    assert( pPopulationKnown_ );
    return *pPopulationKnown_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::IsRecon
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_Population::IsRecon() const
{
    return bIsRecon_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Recon
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_Population::Recon()
{
    bIsRecon_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetArmy
// Created: HME 2005-12-29
// -----------------------------------------------------------------------------
inline
const MIL_Army& DEC_Knowledge_Population::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}