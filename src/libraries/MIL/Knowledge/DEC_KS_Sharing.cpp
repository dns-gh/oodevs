// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_Sharing.cpp $
// $Author: Nld $
// $Modtime: 21/04/05 16:04 $
// $Revision: 3 $
// $Workfile: DEC_KS_Sharing.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_KS_Sharing.h"

#include "DEC_KnowledgeBlackBoard.h"
#include "DEC_Knowledge_Agent.h"
#include "MIL_KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::sShareSource::sShareSource
// Created: NLD 2005-04-20
// -----------------------------------------------------------------------------
DEC_KS_Sharing::sShareSource::sShareSource()
    : pShareSource_       ( 0 )
    , vSharedCircleCenter_( )
    , rSharedCircleRadius_( std::numeric_limits< MT_Float >::max() )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::sShareSource::sShareSource
// Created: NLD 2005-04-20
// -----------------------------------------------------------------------------
DEC_KS_Sharing::sShareSource::sShareSource( const MIL_KnowledgeGroup& shareSource )
    : pShareSource_       ( &shareSource )
    , vSharedCircleCenter_( )
    , rSharedCircleRadius_( std::numeric_limits< MT_Float >::max() )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::sShareSource::sShareSource
// Created: NLD 2005-04-20
// -----------------------------------------------------------------------------
DEC_KS_Sharing::sShareSource::sShareSource( const MIL_KnowledgeGroup& shareSource, const MT_Vector2D& vSharedCircleCenter, MT_Float rSharedCircleRadius )
    : pShareSource_       ( &shareSource )
    , vSharedCircleCenter_( vSharedCircleCenter )
    , rSharedCircleRadius_( rSharedCircleRadius )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Sharing::DEC_KS_Sharing( DEC_KnowledgeBlackBoard& blackBoard, const MIL_KnowledgeGroup& knowledgeGroup )
    : DEC_KnowledgeSource_ABC( blackBoard )
    , pKnowledgeGroup_       ( &knowledgeGroup )
{
    assert( pBlackBoard_ );
    pBlackBoard_->AddToScheduler( *this );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Sharing::~DEC_KS_Sharing()
{
    assert( pBlackBoard_ );
    pBlackBoard_->RemoveFromScheduler( *this );    
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::Prepare
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_Sharing::Prepare()
{
    // Nothing
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ArmyQuerier::sKnowledgeSharer
// Created: NLD 2004-05-06
// -----------------------------------------------------------------------------
class sKnowledgeSharer
{
public:
    sKnowledgeSharer( const MIL_KnowledgeGroup& knowledgeGroup, DEC_KnowledgeBlackBoard& blackBoard, const DEC_KS_Sharing::sShareSource& shareSource )
        : knowledgeGroup_( knowledgeGroup )
        , blackBoard_    ( blackBoard )
        , shareSource_   ( shareSource )
    {
    }

    void operator() ( DEC_Knowledge_Agent& knowledge )
    {
        if( knowledge.GetPosition().Distance( shareSource_.vSharedCircleCenter_ ) > shareSource_.rSharedCircleRadius_ )
            return;

        MIL_Agent_ABC& agentKnown = knowledge.GetAgentKnown();

        DEC_Knowledge_Agent* pNewKnowledge = blackBoard_.GetKnowledgeAgent( agentKnown );
        if( !pNewKnowledge )
            pNewKnowledge = &blackBoard_.CreateKnowledgeAgent( knowledgeGroup_, agentKnown );
        pNewKnowledge->Update( knowledge );
    }

private:
    const MIL_KnowledgeGroup&            knowledgeGroup_;
          DEC_KnowledgeBlackBoard&       blackBoard_;
    const DEC_KS_Sharing::sShareSource&  shareSource_;
};

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::Talk
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_Sharing::Talk()
{
    assert( pBlackBoard_ );
    assert( pKnowledgeGroup_ );

    IT_ShareSourceMMap itShareSourceEnd = shareSources_.upper_bound( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() );
    for( IT_ShareSourceMMap itShareSource = shareSources_.begin(); itShareSource != itShareSourceEnd; ++itShareSource )
    {
        sKnowledgeSharer func( *pKnowledgeGroup_, *pBlackBoard_, itShareSource->second );
        itShareSource->second.pShareSource_->GetKnowledge().ApplyOnKnowledgesAgent( func );
    }
    shareSources_.erase( shareSources_.begin(), itShareSourceEnd );
}
