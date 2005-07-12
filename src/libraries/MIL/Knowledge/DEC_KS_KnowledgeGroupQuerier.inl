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
// Name: DEC_KS_KnowledgeGroupQuerier::GetKnowledgeAgentFromMosID
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Agent* DEC_KS_KnowledgeGroupQuerier::GetKnowledgeAgentFromMosID( uint nID ) const
{
    assert( pBlackBoard_ );
    return pBlackBoard_->GetKnowledgeAgentFromMosID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetKnowledgeAgentFromDiaID
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Agent* DEC_KS_KnowledgeGroupQuerier::GetKnowledgeAgentFromDiaID( uint nID ) const
{
    assert( pBlackBoard_ );
    return pBlackBoard_->GetKnowledgeAgentFromDiaID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::IsAgentKnown
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
inline
bool DEC_KS_KnowledgeGroupQuerier::IsAgentKnown( const MIL_Agent_ABC& agent ) const
{
    assert( pBlackBoard_ );
    return pBlackBoard_->HasKnowledgeAgent( agent );
}



