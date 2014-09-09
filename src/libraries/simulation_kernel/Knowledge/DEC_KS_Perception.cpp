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

#include "simulation_kernel_pch.h"
#include "DEC_KS_Perception.h"
#include "DEC_KnowledgeBlackBoard_AgentPion.h"
#include "DEC_Knowledge_AgentPerception.h"
#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "DEC_Knowledge_UrbanPerception.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgentPerception.h"
#include "DEC_BlackBoard_CanContainKnowledgeObjectPerception.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulationPerception.h"
#include "DEC_BlackBoard_CanContainKnowledgeUrbanPerception.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_Perception )

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Perception::DEC_KS_Perception( DEC_KnowledgeBlackBoard_AgentPion& blackBoard )
    : DEC_KnowledgeSource_ABC        ( blackBoard, 2 )
    , pBlackBoard_                   ( &blackBoard )
    , bMakePerceptionsAvailable_     ( false )
    , bMakePerceptionsAvailableTimed_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_KS_Perception::DEC_KS_Perception ()
    : DEC_KnowledgeSource_ABC        ()
    , pBlackBoard_                   ( 0 )
    , bMakePerceptionsAvailable_     ( false )
    , bMakePerceptionsAvailableTimed_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception destructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KS_Perception::~DEC_KS_Perception()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KS_Perception::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this );
    archive & pBlackBoard_;
    archive & externalPerceptions_;
    archive & bMakePerceptionsAvailable_;
    archive & bMakePerceptionsAvailableTimed_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::Prepare
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_Perception::Prepare()
{
    assert( pBlackBoard_ );
    std::function< void( DEC_Knowledge_AgentPerception& ) > agentFunctor =  boost::bind( &DEC_Knowledge_AgentPerception::Prepare, _1 );
    pBlackBoard_->GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( agentFunctor );
    std::mem_fun_ref_t< void, DEC_Knowledge_ObjectPerception > objectFunctor = std::mem_fun_ref( &DEC_Knowledge_ObjectPerception::Prepare );
    pBlackBoard_->GetKnowledgeObjectPerceptionContainer().ApplyOnKnowledgesObjectPerception( objectFunctor );
    std::mem_fun_ref_t< void, DEC_Knowledge_PopulationPerception > populationFunctor = std::mem_fun_ref( &DEC_Knowledge_PopulationPerception::Prepare );
    pBlackBoard_->GetKnowledgePopulationPerceptionContainer().ApplyOnKnowledgesPopulationPerception( populationFunctor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::CleanKnowledgeAgentPerception
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_Perception::CleanKnowledgeAgentPerception( DEC_Knowledge_AgentPerception& knowledge )
{
    if( knowledge.Clean() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->GetKnowledgeAgentPerceptionContainer().DestroyKnowledgeAgentPerception( knowledge ); // The knowledge will be deleted
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::CleanKnowledgeObjectPerception
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_Perception::CleanKnowledgeObjectPerception( DEC_Knowledge_ObjectPerception& knowledge )
{
    if( knowledge.Clean() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->GetKnowledgeObjectPerceptionContainer().DestroyKnowledgeObjectPerception( knowledge ); // The knowledge will be deleted
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::CleanKnowledgePopulationPerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void DEC_KS_Perception::CleanKnowledgePopulationPerception( DEC_Knowledge_PopulationPerception& knowledge )
{
    if( knowledge.Clean() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->GetKnowledgePopulationPerceptionContainer().DestroyKnowledgePopulationPerception( knowledge ); // The knowledge will be deleted
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::Clean
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_Perception::Clean()
{
    assert( pBlackBoard_ );

    auto methodAgent = boost::bind( &DEC_KS_Perception::CleanKnowledgeAgentPerception, this, _1 );
    pBlackBoard_->GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( methodAgent );

    auto methodObject = boost::bind( &DEC_KS_Perception::CleanKnowledgeObjectPerception, this, _1 );
    pBlackBoard_->GetKnowledgeObjectPerceptionContainer().ApplyOnKnowledgesObjectPerception( methodObject );

    auto methodPopulation = boost::bind( &DEC_KS_Perception::CleanKnowledgePopulationPerception, this, _1 );
    pBlackBoard_->GetKnowledgePopulationPerceptionContainer().ApplyOnKnowledgesPopulationPerception( methodPopulation );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::CleanDeletedAgentKnowledges
// Created: JSR 2013-02-21
// -----------------------------------------------------------------------------
void DEC_KS_Perception::CleanDeletedAgentKnowledges()
{
    for( auto it = externalPerceptions_.begin(); it != externalPerceptions_.end(); )
    {
        if( it->first->IsMarkedForDestruction() )
            it = externalPerceptions_.erase( it );
        else
            ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::Clear
// Created: LDC 2014-09-09
// -----------------------------------------------------------------------------
void DEC_KS_Perception::Clear()
{
    externalPerceptions_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::NotifyExternalPerception
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
void DEC_KS_Perception::NotifyExternalPerception( MIL_Agent_ABC& agentPerceived, const PHY_PerceptionLevel& level )
{
    const PHY_PerceptionLevel*& pLevel = externalPerceptions_[ &agentPerceived ];
    if( !pLevel || *pLevel < level )
        pLevel = &level;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::NotifyPerception
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
bool DEC_KS_Perception::NotifyPerception( MIL_Agent_ABC& agentPerceived, const PHY_PerceptionLevel& level, bool bRecordModeEnabled )
{
    if( level == PHY_PerceptionLevel::notSeen_ || agentPerceived.IsMarkedForDestruction() )
        return false;

    assert( pBlackBoard_ );

    DEC_Knowledge_AgentPerception* pKnowledge = pBlackBoard_->GetKnowledgeAgentPerceptionContainer().GetKnowledgeAgentPerception( agentPerceived );
    bool isNewlyPerceived = ( pKnowledge == 0 );
    if( isNewlyPerceived )
        pKnowledge = &pBlackBoard_->GetKnowledgeAgentPerceptionContainer().CreateKnowledgeAgentPerception( pBlackBoard_->GetPion(), agentPerceived );
    pKnowledge->Update( level, bRecordModeEnabled );
    return isNewlyPerceived;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::NotifyPerception
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
void DEC_KS_Perception::NotifyPerception( MIL_Object_ABC& objectPerceived, const PHY_PerceptionLevel& level, bool /*bRecordModeEnabled*/ )
{
    if( level == PHY_PerceptionLevel::notSeen_ )
        return;

    assert( pBlackBoard_ );

    DEC_Knowledge_ObjectPerception* pKnowledge = pBlackBoard_->GetKnowledgeObjectPerceptionContainer().GetKnowledgeObjectPerception( objectPerceived );
    if( !pKnowledge )
        pKnowledge = &pBlackBoard_->GetKnowledgeObjectPerceptionContainer().CreateKnowledgeObjectPerception( pBlackBoard_->GetPion(), objectPerceived );

    pKnowledge->Update( level );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::NotifyPerception
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
bool DEC_KS_Perception::NotifyPerception( MIL_PopulationConcentration& concentrationPerceived, const PHY_PerceptionLevel& level, bool /*bRecordModeEnabled*/ )
{
    if( level == PHY_PerceptionLevel::notSeen_ )
        return false;

    assert( pBlackBoard_ );

    DEC_Knowledge_PopulationPerception* pKnowledge = pBlackBoard_->GetKnowledgePopulationPerceptionContainer().GetKnowledgePopulationPerception( concentrationPerceived.GetPopulation() );
    bool isNewlyPerceived = ( pKnowledge == 0 );
    if( isNewlyPerceived )
        pKnowledge = &pBlackBoard_->GetKnowledgePopulationPerceptionContainer().CreateKnowledgePopulationPerception( pBlackBoard_->GetPion(), concentrationPerceived.GetPopulation() );

    pKnowledge->Update( concentrationPerceived, level );
    return isNewlyPerceived;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::NotifyPerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_KS_Perception::NotifyPerception( MIL_PopulationFlow& flowPerceived, const PHY_PerceptionLevel& level, const T_PointVector& shape, bool /*bRecordModeEnabled*/ )
{
    if( level == PHY_PerceptionLevel::notSeen_ )
        return false;

    assert( pBlackBoard_ );

    DEC_Knowledge_PopulationPerception* pKnowledge = pBlackBoard_->GetKnowledgePopulationPerceptionContainer().GetKnowledgePopulationPerception( flowPerceived.GetPopulation() );
    bool isNewlyPerceived = ( pKnowledge == 0 );
    if( isNewlyPerceived )
        pKnowledge = &pBlackBoard_->GetKnowledgePopulationPerceptionContainer().CreateKnowledgePopulationPerception( pBlackBoard_->GetPion(), flowPerceived.GetPopulation() );

    pKnowledge->Update( flowPerceived, level, shape );
    return isNewlyPerceived;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::NotifyPerception
// Created: MGD 2009-11-30
// -----------------------------------------------------------------------------
void DEC_KS_Perception::NotifyPerception( const MIL_UrbanObject_ABC& object, const PHY_PerceptionLevel& level )
{
    if( level == PHY_PerceptionLevel::notSeen_ )
        return;

    assert( pBlackBoard_ );

    DEC_Knowledge_UrbanPerception* pKnowledge = pBlackBoard_->GetKnowledgeUrbanPerceptionContainer().GetKnowledgeUrbanPerception( object );
    if( !pKnowledge )
        pKnowledge = pBlackBoard_->GetKnowledgeUrbanPerceptionContainer().CreateKnowledgeUrbanPerception( pBlackBoard_->GetPion(), object );
    assert( pKnowledge );

    pKnowledge->Update( level );
}

// =============================================================================
// TALK
// =============================================================================
namespace
{
    struct sReferenceTimeCalculator
    {
    public:
        sReferenceTimeCalculator( unsigned int& nReferenceTimeStep )
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

        unsigned int& nReferenceTimeStep_;

    private:
        sReferenceTimeCalculator& operator=( sReferenceTimeCalculator& rhs );
    };

    struct sMakePerceptionAvailableTimedFunctor
    {
    public:
        sMakePerceptionAvailableTimedFunctor( unsigned int nReferenceTimeStep )
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
        unsigned int nReferenceTimeStep_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::Talk
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
void DEC_KS_Perception::Talk( int /*currentTimeStep*/ )
{
    assert( pBlackBoard_ );

    if( !pBlackBoard_->GetPion().RetrieveRole< PHY_RoleInterface_Perceiver >() )
        return;

    pBlackBoard_->GetPion().GetRole< PHY_RoleInterface_Perceiver >().ExecutePerceptions();

    for( auto it = externalPerceptions_.begin(); it != externalPerceptions_.end(); ++it )
        NotifyPerception( *it->first, *it->second, false );
    externalPerceptions_.clear();

    if( bMakePerceptionsAvailable_ )
    {
        std::function< void( DEC_Knowledge_AgentPerception& ) > functorAgent = boost::bind( &DEC_Knowledge_AgentPerception::MakeAvailable, _1, 0 );
        pBlackBoard_->GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( functorAgent );
        bMakePerceptionsAvailable_ = false;
    }

    else if( bMakePerceptionsAvailableTimed_ )
    {
        unsigned int nReferenceTimeStep = std::numeric_limits< unsigned int >::max();
        sReferenceTimeCalculator functor( nReferenceTimeStep );
        pBlackBoard_->GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( functor );

        if( nReferenceTimeStep != std::numeric_limits< unsigned int >::max() )
        {
            sMakePerceptionAvailableTimedFunctor functor2( nReferenceTimeStep );
            pBlackBoard_->GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( functor2 );
        }

        bMakePerceptionsAvailableTimed_ = false;
    }
}

namespace
{
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
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::HasDelayedPerceptions
// Created: JVT 2004-12-03
// -----------------------------------------------------------------------------
bool DEC_KS_Perception::HasDelayedPerceptions() const
{
    bool bHasDelayedPerceptions = false;
    assert( pBlackBoard_ );
    sDelayedPerceptionFinder agentFunctor( bHasDelayedPerceptions );
    pBlackBoard_->GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( agentFunctor );
    return bHasDelayedPerceptions;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::MakePerceptionsAvailable
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void DEC_KS_Perception::MakePerceptionsAvailable()
{
    bMakePerceptionsAvailable_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Perception::MakePerceptionsAvailableTimed
// Created: NLD 2004-11-16
// -----------------------------------------------------------------------------
void DEC_KS_Perception::MakePerceptionsAvailableTimed()
{
    bMakePerceptionsAvailableTimed_ = true;
}
