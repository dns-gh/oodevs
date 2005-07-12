// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_Perception.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:48 $
// $Revision: 7 $
// $Workfile: DEC_KS_Perception.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_KS_Perception.h"

#include "DEC_KnowledgeBlackBoard.h"
#include "DEC_Knowledge_AgentPerception.h"
#include "DEC_Knowledge_ObjectPerception.h"

#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"

BOOST_CLASS_EXPORT_GUID( DEC_KS_Perception, "DEC_KS_Perception" )

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Perception::DEC_KS_Perception( DEC_KnowledgeBlackBoard& blackBoard, MIL_AgentPion& agentPerceiving )
    : DEC_KnowledgeSource_ABC        ( blackBoard )
    , pAgentPerceiving_              ( &agentPerceiving )
    , bMakePerceptionsAvailable_     ( false )
    , bMakePerceptionsAvailableTimed_( false )
{
    assert( pBlackBoard_ );
    pBlackBoard_->AddToScheduler( *this, true );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_KS_Perception::DEC_KS_Perception ()
    : DEC_KnowledgeSource_ABC        ()
    , pAgentPerceiving_              ( 0 )
    , bMakePerceptionsAvailable_     ( false )
    , bMakePerceptionsAvailableTimed_( false )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception destructor
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
DEC_KS_Perception::~DEC_KS_Perception()
{
    assert( pBlackBoard_ );
    pBlackBoard_->RemoveFromScheduler( *this );    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, DEC_KS_Perception::T_AgentPerceptionMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const DEC_KS_Perception::T_AgentPerceptionMap& map, const uint )
        {
            file << map.size();
            for ( DEC_KS_Perception::CIT_AgentPerceptionMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << it->second->GetID();
            }
        }
        
        template< typename Archive >
        void load( Archive& file, DEC_KS_Perception::T_AgentPerceptionMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                MIL_Agent_ABC* pAgent;
                file >> pAgent;
                
                uint nID;
                file >> nID;
                
                map[ pAgent ] = &PHY_PerceptionLevel::FindPerceptionLevel( nID );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::load
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_KS_Perception::load( boost::archive::binary_iarchive& file, const uint )
{
    file >> boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this )
         >> externalPerceptions_
         >> pAgentPerceiving_
         >> bMakePerceptionsAvailable_
         >> bMakePerceptionsAvailableTimed_;

    assert( pBlackBoard_ );
    pBlackBoard_->AddToScheduler( *this, true );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::save
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_KS_Perception::save( boost::archive::binary_oarchive& file, const uint ) const
{
    file << boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this )
         << externalPerceptions_
         << pAgentPerceiving_
         << bMakePerceptionsAvailable_
         << bMakePerceptionsAvailableTimed_;
}

// =============================================================================
// PREPARE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::PrepareKnowledgeAgentPerception
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_Perception::PrepareKnowledgeAgentPerception( DEC_Knowledge_AgentPerception& knowledge )
{
    knowledge.Prepare();
    if ( knowledge.MustBeDestroyed() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->DestroyKnowledgeAgentPerception( knowledge ); // The knowledge will be deleted
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::PrepareKnowledgeObjectPerception
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_Perception::PrepareKnowledgeObjectPerception( DEC_Knowledge_ObjectPerception& knowledge )
{
    knowledge.Prepare();
    if ( knowledge.MustBeDestroyed() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->DestroyKnowledgeObjectPerception( knowledge ); // The knowledge will be deleted
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::Prepare
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_Perception::Prepare()
{
    assert( pBlackBoard_ );

    class_mem_fun_void_t< DEC_KS_Perception, DEC_Knowledge_AgentPerception> methodAgent( DEC_KS_Perception::PrepareKnowledgeAgentPerception, *this );
    pBlackBoard_->ApplyOnKnowledgesAgentPerception( methodAgent );

    class_mem_fun_void_t< DEC_KS_Perception, DEC_Knowledge_ObjectPerception> methodObject( DEC_KS_Perception::PrepareKnowledgeObjectPerception, *this );
    pBlackBoard_->ApplyOnKnowledgesObjectPerception( methodObject );
}

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::NotifyAgentExternalPerception
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
void DEC_KS_Perception::NotifyAgentExternalPerception( MIL_Agent_ABC& agentPerceived, const PHY_PerceptionLevel& level )
{
    const PHY_PerceptionLevel*& pLevel = externalPerceptions_[ &agentPerceived ];
    if( !pLevel || *pLevel < level )
        pLevel = &level;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::NotifyAgentPerception
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
void DEC_KS_Perception::NotifyAgentPerception( MIL_Agent_ABC& agentPerceived, const PHY_PerceptionLevel& level, bool bRecordModeEnabled )
{
    if ( level == PHY_PerceptionLevel::notSeen_ )
        return;

    assert( pBlackBoard_ );

    DEC_Knowledge_AgentPerception* pKnowledge = pBlackBoard_->GetKnowledgeAgentPerception( agentPerceived );
    if ( !pKnowledge )
    {
        assert( pAgentPerceiving_ );
        pKnowledge = &pBlackBoard_->CreateKnowledgeAgentPerception( *pAgentPerceiving_, agentPerceived );
    }
    
    pKnowledge->Update( level, bRecordModeEnabled );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::NotifyObjectPerception
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
void DEC_KS_Perception::NotifyObjectPerception( MIL_RealObject_ABC& objectPerceived, const PHY_PerceptionLevel& level, bool /*bRecordModeEnabled*/ )
{
    if( level == PHY_PerceptionLevel::notSeen_ )
        return;

    assert( pBlackBoard_ );

    DEC_Knowledge_ObjectPerception* pKnowledge = pBlackBoard_->GetKnowledgeObjectPerception( objectPerceived );
    if ( !pKnowledge )
    {
        assert( pAgentPerceiving_ );
        pKnowledge = &pBlackBoard_->CreateKnowledgeObjectPerception( *pAgentPerceiving_, objectPerceived );
    }

    pKnowledge->Update( level );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::MakePerceptionAvailable
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void DEC_KS_Perception::MakePerceptionAvailable( DEC_Knowledge_AgentPerception& perception )
{
    perception.MakeAvailable();
}

// =============================================================================
// TALK
// =============================================================================

struct sReferenceTimeCalculator
{
public:
    sReferenceTimeCalculator( uint& nReferenceTimeStep )
        : nReferenceTimeStep_( nReferenceTimeStep )
    {
    }

    sReferenceTimeCalculator( sReferenceTimeCalculator& rhs )
        : nReferenceTimeStep_( rhs.nReferenceTimeStep_ )
    {
    }

    void operator() ( const DEC_Knowledge_AgentPerception& perception ) const
    {
        if( !perception.IsAvailable() )
        {
            nReferenceTimeStep_ = std::min( nReferenceTimeStep_, perception.GetCreationTimeStep() );
        }
    }

    uint& nReferenceTimeStep_;

private:
    sReferenceTimeCalculator& operator=( sReferenceTimeCalculator& rhs );
};

struct sMakePerceptionAvailableTimedFunctor
{
public:
    sMakePerceptionAvailableTimedFunctor( uint nReferenceTimeStep )
        : nReferenceTimeStep_( nReferenceTimeStep )
    {
    }
    
    void operator() ( DEC_Knowledge_AgentPerception& perception ) const
    {
        if( !perception.IsAvailable() )
        {
            assert( perception.GetCreationTimeStep() >= nReferenceTimeStep_ );
            perception.MakeAvailable( perception.GetCreationTimeStep() - nReferenceTimeStep_ );
        }
    }

private:
    uint nReferenceTimeStep_;
};


// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::Talk
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
void DEC_KS_Perception::Talk()
{
    assert( pAgentPerceiving_ );
    assert( pBlackBoard_ );

    pAgentPerceiving_->GetRole< PHY_RolePion_Perceiver >().ExecutePerceptions();

    for( CIT_AgentPerceptionMap itExt = externalPerceptions_.begin(); itExt != externalPerceptions_.end(); ++itExt )
        NotifyAgentPerception( *itExt->first, *itExt->second, false );
    externalPerceptions_.clear();

    if( bMakePerceptionsAvailable_ )
    {
        class_mem_fun_void_t< DEC_KS_Perception, DEC_Knowledge_AgentPerception > methodAgent( DEC_KS_Perception::MakePerceptionAvailable, *this );
        pBlackBoard_->ApplyOnKnowledgesAgentPerception( methodAgent );    
        bMakePerceptionsAvailable_ = false;
    }

    else if ( bMakePerceptionsAvailableTimed_ )
    {
        uint nReferenceTimeStep = std::numeric_limits< uint >::max();
        sReferenceTimeCalculator functor( nReferenceTimeStep );
        pBlackBoard_->ApplyOnKnowledgesAgentPerception( functor );

        if ( nReferenceTimeStep != std::numeric_limits< uint >::max() )
        {
            sMakePerceptionAvailableTimedFunctor functor2( nReferenceTimeStep );
            pBlackBoard_->ApplyOnKnowledgesAgentPerception( functor2 );
        }

        bMakePerceptionsAvailableTimed_ = false;
    }
}


// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::HasDelayedPerceptions
// Created: JVT 2004-12-03
// -----------------------------------------------------------------------------
struct sDelayedPerceptionFinder
{
    sDelayedPerceptionFinder( bool& bResult ) : bHasDelayedPerceptions_( bResult ) {}
    
    void operator() ( DEC_Knowledge_AgentPerception& perception )
    {
        bHasDelayedPerceptions_ |= !perception.IsAvailable();
    }
    
    private:
        sDelayedPerceptionFinder& operator = ( const sDelayedPerceptionFinder& );
        
    private:
        bool& bHasDelayedPerceptions_;
};

bool DEC_KS_Perception::HasDelayedPerceptions() const
{
    bool bHasDelayedPerceptions = false;
    assert( pBlackBoard_ );
    pBlackBoard_->ApplyOnKnowledgesAgentPerception( sDelayedPerceptionFinder( bHasDelayedPerceptions ) );
    return bHasDelayedPerceptions;
}
