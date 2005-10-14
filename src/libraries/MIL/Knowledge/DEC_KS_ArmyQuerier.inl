// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_ArmyQuerier.inl $
// $Author: Jvt $
// $Modtime: 16/03/05 16:18 $
// $Revision: 3 $
// $Workfile: DEC_KS_ArmyQuerier.inl $
//
// *****************************************************************************

#include "DEC_KnowledgeBlackBoard.h"

// -----------------------------------------------------------------------------
// Name: DEC_KS_ArmyQuerier::GetKnowledgesObject
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
void DEC_KS_ArmyQuerier::GetKnowledgesObject( T_KnowledgeObjectVector& container ) const
{
    assert( pBlackBoard_ );
    pBlackBoard_->GetKnowledgesObject( container );    
}


// -----------------------------------------------------------------------------
// Name: DEC_KS_ArmyQuerier::GetKnowledgeObjectFromID
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Object* DEC_KS_ArmyQuerier::GetKnowledgeObjectFromID( uint nID ) const
{
    assert( pBlackBoard_ );
    return pBlackBoard_->GetKnowledgeObjectFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ArmyQuerier::GetKnowledgeObject
// Created: NLD 2004-03-26
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Object* DEC_KS_ArmyQuerier::GetKnowledgeObject( MIL_RealObject_ABC& object ) const
{
    T_KnowledgeObjectVector knowledges;
    
    assert( pBlackBoard_ );
    pBlackBoard_->GetKnowledgesObject( object, knowledges );
    if( knowledges.empty() )
        return 0;
    return knowledges.front();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ArmyQuerier::IsKnown
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
inline
bool DEC_KS_ArmyQuerier::IsKnown( const MIL_RealObject_ABC& object ) const
{
    assert( pBlackBoard_ );
    return pBlackBoard_->HasKnowledgeObject( object );
}