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

#include "simulation_kernel_pch.h"
#include "DEC_KS_Sharing.h"
#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "DEC_Knowledge_Agent.h"
#include "MIL_KnowledgeGroup.h"
#include "MIL_AgentServer.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_Sharing )

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::sShareSource::sShareSource
// Created: NLD 2005-04-20
// -----------------------------------------------------------------------------
DEC_KS_Sharing::sShareSource::sShareSource()
    : pShareSource_       ( 0 )
    , vSharedCircleCenter_( )
    , rSharedCircleRadius_( std::numeric_limits< double >::max() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::sShareSource::sShareSource
// Created: NLD 2005-04-20
// -----------------------------------------------------------------------------
DEC_KS_Sharing::sShareSource::sShareSource( const MIL_KnowledgeGroup& shareSource )
    : pShareSource_       ( &shareSource )
    , vSharedCircleCenter_( )
    , rSharedCircleRadius_( std::numeric_limits< double >::max() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::sShareSource::sShareSource
// Created: NLD 2005-04-20
// -----------------------------------------------------------------------------
DEC_KS_Sharing::sShareSource::sShareSource( const MIL_KnowledgeGroup& shareSource, const MT_Vector2D& vSharedCircleCenter, double rSharedCircleRadius )
    : pShareSource_       ( &shareSource )
    , vSharedCircleCenter_( vSharedCircleCenter )
    , rSharedCircleRadius_( rSharedCircleRadius )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Sharing::DEC_KS_Sharing( DEC_KnowledgeBlackBoard_KnowledgeGroup& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard, 1 )
    , pBlackBoard_           ( &blackBoard )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KS_Sharing::DEC_KS_Sharing()
    : DEC_KnowledgeSource_ABC()
    , pBlackBoard_          ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Sharing::~DEC_KS_Sharing()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::Prepare
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_Sharing::Prepare()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::Clean
// Created: HBD 2009-12-30
// -----------------------------------------------------------------------------
void DEC_KS_Sharing::Clean()
{
    // NOTHING
}

namespace
{
    class sKnowledgeSharer : boost::noncopyable
    {
    public:
        sKnowledgeSharer( const MIL_KnowledgeGroup& knowledgeGroup, DEC_BlackBoard_CanContainKnowledgeAgent& blackBoard, const DEC_KS_Sharing::sShareSource& shareSource )
            : knowledgeGroup_( knowledgeGroup )
            , blackBoard_    ( blackBoard )
            , shareSource_   ( shareSource )
        {}

        void operator() ( DEC_Knowledge_Agent& knowledge, int currentTimeStep)
        {
            if( knowledge.GetPosition().Distance( shareSource_.vSharedCircleCenter_ ) > shareSource_.rSharedCircleRadius_ )
                return;

            MIL_Agent_ABC& agentKnown = knowledge.GetAgentKnown();

            boost::shared_ptr< DEC_Knowledge_Agent > pNewKnowledge = blackBoard_.GetKnowledgeAgent( agentKnown );
            DEC_Knowledge_Agent* newKnowledge = pNewKnowledge.get();
            if( !newKnowledge )
                newKnowledge = &blackBoard_.CreateKnowledgeAgent( knowledgeGroup_, agentKnown );
            newKnowledge->Update( knowledge, currentTimeStep );
        }

    private:
        const MIL_KnowledgeGroup&                      knowledgeGroup_;
              DEC_BlackBoard_CanContainKnowledgeAgent& blackBoard_;
        const DEC_KS_Sharing::sShareSource&            shareSource_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::Talk
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_Sharing::Talk( int currentTimeStep )
{
    assert( pBlackBoard_ );

    IT_ShareSourceMMap itShareSourceEnd = shareSources_.upper_bound( currentTimeStep );
    for( IT_ShareSourceMMap itShareSource = shareSources_.begin(); itShareSource != itShareSourceEnd; ++itShareSource )
    {
        sKnowledgeSharer func( pBlackBoard_->GetKnowledgeGroup(), pBlackBoard_->GetKnowledgeAgentContainer(), itShareSource->second );
        itShareSource->second.pShareSource_->GetKnowledge().GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( func, currentTimeStep );
    }
    shareSources_.erase( shareSources_.begin(), itShareSourceEnd );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::ShareFromSource
// Created: NLD 2005-04-20
// -----------------------------------------------------------------------------
void DEC_KS_Sharing::ShareFromSource( const MIL_KnowledgeGroup& source, unsigned int nShareTimeStep )
{
    shareSources_.insert( std::make_pair( nShareTimeStep, sShareSource( source ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::ShareFromSource
// Created: NLD 2005-04-20
// -----------------------------------------------------------------------------
void DEC_KS_Sharing::ShareFromSource( const MIL_KnowledgeGroup& source, unsigned int nShareTimeStep, const MT_Vector2D& vSharedCircleCenter, double rSharedCircleRadius )
{
    shareSources_.insert( std::make_pair( nShareTimeStep, sShareSource( source, vSharedCircleCenter, rSharedCircleRadius ) ) );
}
