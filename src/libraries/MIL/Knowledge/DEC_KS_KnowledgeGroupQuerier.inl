// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_KnowledgeGroupQuerier.inl $
// $Author: Jvt $
// $Modtime: 16/03/05 17:35 $
// $Revision: 2 $
// $Workfile: DEC_KS_KnowledgeGroupQuerier.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetKnowledgeAgentFromID
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Agent* DEC_KS_KnowledgeGroupQuerier::GetKnowledgeAgentFromID( uint nID ) const
{
    assert( pBlackBoard_ );
    return pBlackBoard_->GetKnowledgeAgentFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetKnowledgePopulationFromID
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Population* DEC_KS_KnowledgeGroupQuerier::GetKnowledgePopulationFromID( uint nID ) const
{
    assert( pBlackBoard_ );
    return pBlackBoard_->GetKnowledgePopulationFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::IsKnown
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
inline
bool DEC_KS_KnowledgeGroupQuerier::IsKnown( const MIL_Agent_ABC& agent ) const
{
    assert( pBlackBoard_ );
    return pBlackBoard_->HasKnowledgeAgent( agent );
}



