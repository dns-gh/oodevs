// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Populations/DEC_PopulationKnowledge.cpp $
// $Author: Nld $
// $Modtime: 23/06/05 10:31 $
// $Revision: 20 $
// $Workfile: DEC_PopulationKnowledge.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PopulationKnowledge.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_PopulationKnowledge )

template< typename Archive >
void save_construct_data( Archive& archive, const DEC_PopulationKnowledge* popKn, const unsigned int /*version*/ )
{
    const MIL_Population* const population = &popKn->population_;
    archive << population;
}

template< typename Archive >
void load_construct_data( Archive& archive, DEC_PopulationKnowledge* popKn, const unsigned int /*version*/ )
{
    MIL_Population* population;
    archive >> population;
    ::new( popKn )DEC_PopulationKnowledge( *population );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge constructor
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
DEC_PopulationKnowledge::DEC_PopulationKnowledge( const MIL_Population& population )
    : population_        ( population )
    , bChannelingChanged_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge destructor
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
DEC_PopulationKnowledge::~DEC_PopulationKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_PopulationKnowledge::serialize
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_PopulationKnowledge::serialize( Archive& file, const unsigned int )
{
    file & attackers_;
    file & newAttackers_;
    file & securers_;
    file & newSecurers_;
         //& channelingLocations_
         //& newChannelingLocations_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::Update
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
void DEC_PopulationKnowledge::Update()
{
    attackers_.clear();
    securers_.clear();
    if( !population_.IsBlinded() )
    {
        std::swap( attackers_, newAttackers_ );
        std::swap( securers_, newSecurers_ );
    }
    bChannelingChanged_ = ( channelingLocations_ != newChannelingLocations_ );
    channelingLocations_.clear();
    std::swap( channelingLocations_, newChannelingLocations_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::Clean
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
void DEC_PopulationKnowledge::Clean()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::GetPionsAttacking
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
std::vector< unsigned int > DEC_PopulationKnowledge::GetPionsAttacking() const
{
    std::vector< unsigned int > container;
    container.reserve( attackers_.size() );
    for( auto it = attackers_.begin(); it != attackers_.end(); ++it )
        container.push_back( (*it)->GetID() );
    return container;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::GetPionsSecuring
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
std::vector< unsigned int > DEC_PopulationKnowledge::GetPionsSecuring() const
{
    std::vector< unsigned int > container;
    container.reserve( securers_.size() );
    for( auto it = securers_.begin(); it != securers_.end(); ++it )
        container.push_back( (*it)->GetID() );
    return container;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgeAgent
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_PopulationKnowledge::ResolveKnowledgeAgent( const sword::Id& asn ) const
{
    return ResolveKnowledgeAgent( asn.id() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgeAgent
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_PopulationKnowledge::ResolveKnowledgeAgent( const MIL_Agent_ABC& agent ) const
{
    boost::shared_ptr< MIL_KnowledgeGroup > knowledgeGroup = population_.GetKnowledgeGroup();
    if( knowledgeGroup )
    {
        auto bbKg = knowledgeGroup->GetKnowledge();
        if( bbKg )
            return bbKg->GetKnowledgeAgent( agent );
    }
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgeAgent
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_PopulationKnowledge::ResolveKnowledgeAgent( unsigned int nID ) const
{
    boost::shared_ptr< MIL_KnowledgeGroup > knowledgeGroup = population_.GetKnowledgeGroup();
    if( knowledgeGroup )
    {
        auto bbKg = knowledgeGroup->GetKnowledge();
        if( bbKg )
            return bbKg->GetKnowledgeAgentFromID( nID );
    }
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgeObject
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_PopulationKnowledge::ResolveKnowledgeObject( const sword::Id& asn ) const
{
    return ResolveKnowledgeObject( asn.id() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgeObject
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_PopulationKnowledge::ResolveKnowledgeObject( const MIL_Object_ABC& object ) const
{
    boost::shared_ptr< MIL_KnowledgeGroup > knowledgeGroup = population_.GetKnowledgeGroup();
    if( knowledgeGroup )
        return knowledgeGroup->ResolveKnowledgeObject( object );
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgeObject
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_PopulationKnowledge::ResolveKnowledgeObject( unsigned int nID ) const
{
    boost::shared_ptr< MIL_KnowledgeGroup > knowledgeGroup = population_.GetKnowledgeGroup();
    if( knowledgeGroup )
        return knowledgeGroup->ResolveKnowledgeObject( nID );
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgeObjectByObjectID
// Created: JSR 2011-09-28
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_PopulationKnowledge::ResolveKnowledgeObjectByObjectID( unsigned int nID ) const
{
    boost::shared_ptr< MIL_KnowledgeGroup > knowledgeGroup = population_.GetKnowledgeGroup();
    if( knowledgeGroup )
        return knowledgeGroup->ResolveKnowledgeObjectByObjectID( nID );
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgePopulation
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Population > DEC_PopulationKnowledge::ResolveKnowledgePopulation( const sword::Id& asn ) const
{
    return ResolveKnowledgePopulation( asn.id() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgePopulation
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Population > DEC_PopulationKnowledge::ResolveKnowledgePopulation( const MIL_Population& population ) const
{
    boost::shared_ptr< MIL_KnowledgeGroup > knowledgeGroup = population_.GetKnowledgeGroup();
    if( knowledgeGroup )
    {
        auto bbKg = knowledgeGroup->GetKnowledge();
        if( bbKg )
            return bbKg->GetKnowledgePopulation( population );
    }
    return boost::shared_ptr< DEC_Knowledge_Population >();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgePopulation
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Population > DEC_PopulationKnowledge::ResolveKnowledgePopulation( unsigned int nID ) const
{
    boost::shared_ptr< MIL_KnowledgeGroup > knowledgeGroup = population_.GetKnowledgeGroup();
    if( knowledgeGroup )
    {
        auto bbKg = knowledgeGroup->GetKnowledge();
        if( bbKg )
            return bbKg->GetKnowledgePopulationFromID( nID );
    }
    return boost::shared_ptr< DEC_Knowledge_Population >();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::NotifyAttackedBy
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
void DEC_PopulationKnowledge::NotifyAttackedBy( const MIL_Agent_ABC& attacker )
{
    newAttackers_.insert( &attacker );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::NotifySecuredBy
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
void DEC_PopulationKnowledge::NotifySecuredBy( const MIL_Agent_ABC& securer )
{
    newSecurers_.insert( &securer );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::GetChannelingLocations
// Created: SBO 2006-01-16
// -----------------------------------------------------------------------------
const DEC_PopulationKnowledge::T_LocationVector& DEC_PopulationKnowledge::GetChannelingLocations() const
{
    return channelingLocations_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::HasChannelingChanged
// Created: SBO 2006-01-17
// -----------------------------------------------------------------------------
bool DEC_PopulationKnowledge::HasChannelingChanged() const
{
    return bChannelingChanged_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::NotifyChanneling
// Created: SBO 2006-01-16
// -----------------------------------------------------------------------------
void DEC_PopulationKnowledge::NotifyChanneled( const TER_Localisation& location )
{
    if( std::find( newChannelingLocations_.begin(), newChannelingLocations_.end(), location )
        == newChannelingLocations_.end() )
        newChannelingLocations_.push_back( location );
}
