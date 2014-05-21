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
#include "DEC_BlackBoard_CanContainKnowledgePopulation.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "DEC_Knowledge_Agent.h"
#include "DEC_Knowledge_Object.h"
#include "DEC_Knowledge_Population.h"
#include "MIL_KnowledgeGroup.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_Sharing )

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::sShareSource::sShareSource
// Created: NLD 2005-04-20
// -----------------------------------------------------------------------------
DEC_KS_Sharing::sShareSource::sShareSource()
    : vSharedCircleCenter_( )
    , rSharedCircleRadius_( std::numeric_limits< double >::max() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::sShareSource::sShareSource
// Created: NLD 2005-04-20
// -----------------------------------------------------------------------------
DEC_KS_Sharing::sShareSource::sShareSource( const boost::shared_ptr< MIL_KnowledgeGroup >& shareSource )
    : pShareSource_       ( shareSource )
    , vSharedCircleCenter_( )
    , rSharedCircleRadius_( std::numeric_limits< double >::max() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::sShareSource::sShareSource
// Created: NLD 2005-04-20
// -----------------------------------------------------------------------------
DEC_KS_Sharing::sShareSource::sShareSource( const boost::shared_ptr< MIL_KnowledgeGroup >& shareSource, const MT_Vector2D& vSharedCircleCenter, double rSharedCircleRadius )
    : pShareSource_       ( shareSource )
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

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::CleanDeletedAgentKnowledges
// Created: JSR 2013-02-21
// -----------------------------------------------------------------------------
void DEC_KS_Sharing::CleanDeletedAgentKnowledges()
{
    // NOTHING
}

namespace
{
    class sAgentKnowledgeSharer : boost::noncopyable
    {
    public:
        sAgentKnowledgeSharer( const boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup, DEC_BlackBoard_CanContainKnowledgeAgent& blackBoard, const DEC_KS_Sharing::sShareSource& shareSource )
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
                newKnowledge = &blackBoard_.CreateKnowledgeAgent( *knowledgeGroup_, agentKnown );
            newKnowledge->Update( knowledge, currentTimeStep );
        }

    private:
              boost::shared_ptr< MIL_KnowledgeGroup > knowledgeGroup_;
              DEC_BlackBoard_CanContainKnowledgeAgent& blackBoard_;
        const DEC_KS_Sharing::sShareSource&            shareSource_;
    };

    class sPopulationKnowledgeSharer : boost::noncopyable
    {
    public:
        sPopulationKnowledgeSharer( const boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup, DEC_BlackBoard_CanContainKnowledgePopulation& blackBoard,
                                    const DEC_KS_Sharing::sShareSource& shareSource )
            : knowledgeGroup_( knowledgeGroup )
            , blackBoard_    ( blackBoard )
            , shareSource_   ( shareSource )
        {}

        void operator() ( DEC_Knowledge_Population& knowledge, int currentTimeStep)
        {
            MT_Vector2D point = knowledge.GetClosestPoint( shareSource_.vSharedCircleCenter_ );
            if( point.Distance( shareSource_.vSharedCircleCenter_ ) > shareSource_.rSharedCircleRadius_ )
                return;

            MIL_Population& population = knowledge.GetPopulationKnown();
            boost::shared_ptr< DEC_Knowledge_Population > pNewKnowledge = blackBoard_.GetKnowledgePopulation( population );
            DEC_Knowledge_Population* newKnowledge = pNewKnowledge.get();
            if( !newKnowledge )
                newKnowledge = &blackBoard_.CreateKnowledgePopulation( *knowledgeGroup_, population );
            newKnowledge->Update( knowledge, currentTimeStep );
        }

    private:
        boost::shared_ptr< MIL_KnowledgeGroup > knowledgeGroup_;
        DEC_BlackBoard_CanContainKnowledgePopulation& blackBoard_;
        const DEC_KS_Sharing::sShareSource&            shareSource_;
    };

    class sObjectKnowledgeSharer : boost::noncopyable
    {
    public:
        sObjectKnowledgeSharer( const boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup, DEC_BlackBoard_CanContainKnowledgeObject& blackBoard,
                                    const DEC_KS_Sharing::sShareSource& shareSource )
            : knowledgeGroup_( knowledgeGroup )
            , blackBoard_    ( blackBoard )
            , shareSource_   ( shareSource )
        {}

        void operator() ( DEC_Knowledge_Object& knowledge, int currentTimeStep)
        {
            MT_Vector2D point;
            knowledge.GetLocalisation().ComputeNearestPoint( shareSource_.vSharedCircleCenter_, point );

            if( point.Distance( shareSource_.vSharedCircleCenter_ ) > shareSource_.rSharedCircleRadius_ )
                return;

            MIL_Object_ABC* object = knowledge.GetObjectKnown();
            if( object )
            {
                boost::shared_ptr< DEC_Knowledge_Object > pNewKnowledge = blackBoard_.GetKnowledgeObject( *object );
                if( !pNewKnowledge.get() )
                    pNewKnowledge = blackBoard_.CreateKnowledgeObject( *object );
                pNewKnowledge->Update( knowledge, currentTimeStep );
            }
        }

    private:
        boost::shared_ptr< MIL_KnowledgeGroup > knowledgeGroup_;
        DEC_BlackBoard_CanContainKnowledgeObject& blackBoard_;
        const DEC_KS_Sharing::sShareSource&            shareSource_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::Talk
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_Sharing::Talk( int currentTimeStep )
{
    if( !pBlackBoard_ )
        return;

    auto itShareSourceEnd = shareSources_.upper_bound( currentTimeStep );
    for( auto itShareSource = shareSources_.begin(); itShareSource != itShareSourceEnd; ++itShareSource )
    {
        auto bbKg = itShareSource->second.pShareSource_->GetKnowledge();
        if( bbKg )
        {
            sAgentKnowledgeSharer agentFunc( pBlackBoard_->GetKnowledgeGroup(), pBlackBoard_->GetKnowledgeAgentContainer(), itShareSource->second );
            bbKg->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( agentFunc, currentTimeStep );
            sPopulationKnowledgeSharer populationFunc( pBlackBoard_->GetKnowledgeGroup(), pBlackBoard_->GetKnowledgePopulationContainer(), itShareSource->second );
            bbKg->GetKnowledgePopulationContainer().ApplyOnKnowledgesPopulation( populationFunc, currentTimeStep );
            sObjectKnowledgeSharer objectFunc( pBlackBoard_->GetKnowledgeGroup(), pBlackBoard_->GetKnowledgeObjectContainer(), itShareSource->second );
            bbKg->GetKnowledgeObjectContainer().ApplyOnKnowledgesObjectRef( objectFunc, currentTimeStep );
        }
    }
    shareSources_.erase( shareSources_.begin(), itShareSourceEnd );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::ShareFromSource
// Created: NLD 2005-04-20
// -----------------------------------------------------------------------------
void DEC_KS_Sharing::ShareFromSource( const boost::shared_ptr< MIL_KnowledgeGroup >& source, unsigned int nShareTimeStep )
{
    shareSources_.insert( std::make_pair( nShareTimeStep, sShareSource( source ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::ShareFromSource
// Created: NLD 2005-04-20
// -----------------------------------------------------------------------------
void DEC_KS_Sharing::ShareFromSource( const boost::shared_ptr< MIL_KnowledgeGroup >& source, unsigned int nShareTimeStep, const MT_Vector2D& vSharedCircleCenter, double rSharedCircleRadius )
{
    shareSources_.insert( std::make_pair( nShareTimeStep, sShareSource( source, vSharedCircleCenter, rSharedCircleRadius ) ) );
}
