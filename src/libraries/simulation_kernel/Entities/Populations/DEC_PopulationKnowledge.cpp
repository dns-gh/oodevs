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
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "protocol/Protocol.h"
#include <boost/serialization/split_free.hpp>

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

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, DEC_PopulationKnowledge::T_AgentSet& set, const unsigned int nVersion )
        {
            split_free( file, set, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const DEC_PopulationKnowledge::T_AgentSet& set, const unsigned int )
        {
            std::size_t size = set.size();
            file << size;
            for ( DEC_PopulationKnowledge::CIT_AgentSet it = set.begin(); it != set.end(); ++it )
                file << *it;
        }

        template< typename Archive >
        void load( Archive& file, DEC_PopulationKnowledge::T_AgentSet& set, const unsigned int )
        {
            std::size_t nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                MIL_Agent_ABC* pAgent;
                file >> pAgent;
                set.insert( pAgent );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_PopulationKnowledge::serialize
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_PopulationKnowledge::serialize( Archive& file, const unsigned int )
{
    file & attackers_
         & newAttackers_
         & securers_
         & newSecurers_;
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
    for( CIT_AgentSet it = attackers_.begin(); it != attackers_.end(); ++it )
        container.push_back( ( **it ).GetID() );
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
    for( CIT_AgentSet it = securers_.begin(); it != securers_.end(); ++it )
        container.push_back( ( **it ).GetID() );
    return container;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgeAgent
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_PopulationKnowledge::ResolveKnowledgeAgent( const sword::UnitKnowledgeId& /*asn*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgeAgent
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_PopulationKnowledge::ResolveKnowledgeAgent( const MIL_Agent_ABC& /*agent*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgeAgent
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_PopulationKnowledge::ResolveKnowledgeAgent( unsigned int /*nID*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgeObject
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_PopulationKnowledge::ResolveKnowledgeObject( const sword::ObjectKnowledgeId& asn ) const
{
    return population_.GetArmy().GetKnowledge().GetKnowledgeObjectFromID( asn.id() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgeObject
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_PopulationKnowledge::ResolveKnowledgeObject( const MIL_Object_ABC& object ) const
{
    return population_.GetArmy().GetKnowledge().GetKnowledgeObject( object );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgeObject
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_PopulationKnowledge::ResolveKnowledgeObject( unsigned int nID ) const
{
    return population_.GetArmy().GetKnowledge().GetKnowledgeObjectFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgePopulation
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_PopulationKnowledge::ResolveKnowledgePopulation( const sword::CrowdKnowledgeId& /*asn*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgePopulation
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_PopulationKnowledge::ResolveKnowledgePopulation( const MIL_Population& /*population*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::ResolveKnowledgePopulation
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_PopulationKnowledge::ResolveKnowledgePopulation( unsigned int /*nID*/ ) const
{
    return 0;
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
    CIT_LocationVector it = std::find( newChannelingLocations_.begin(), newChannelingLocations_.end(), location );
    if( it == newChannelingLocations_.end())
        newChannelingLocations_.push_back( location );
}
