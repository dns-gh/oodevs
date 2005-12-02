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

#include "MIL_pch.h"

#include "DEC_PopulationKnowledge.h"

#include "Entities/Agents/MIL_Agent_ABC.h"

BOOST_CLASS_EXPORT_GUID( DEC_PopulationKnowledge, "DEC_PopulationKnowledge" )

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge constructor
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
DEC_PopulationKnowledge::DEC_PopulationKnowledge()
    : attackers_   ()
    , newAttackers_()
    , securers_    ()
    , newSecurers_ ()
{

}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge destructor
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
DEC_PopulationKnowledge::~DEC_PopulationKnowledge()
{

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
        void serialize( Archive& file, DEC_PopulationKnowledge::T_AgentSet& set, const uint nVersion )
        {
            split_free( file, set, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const DEC_PopulationKnowledge::T_AgentSet& set, const uint )
        {
            file << set.size();
            for ( DEC_PopulationKnowledge::CIT_AgentSet it = set.begin(); it != set.end(); ++it )
            {
                file << *it;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, DEC_PopulationKnowledge::T_AgentSet& set, const uint )
        {
            uint nNbr;
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
void DEC_PopulationKnowledge::serialize( Archive& file, const uint )
{
    file & attackers_
         & newAttackers_
         & securers_
         & newSecurers_;
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::Update
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
void DEC_PopulationKnowledge::Update()
{
    attackers_.clear();
    std::swap( attackers_, newAttackers_ );
    securers_.clear();
    std::swap( securers_, newSecurers_ );
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
void DEC_PopulationKnowledge::GetPionsAttacking( T_PopulationKnowledgeAgentDiaIDVector& container ) const
{
    container.clear();
    for( CIT_AgentSet it = attackers_.begin(); it != attackers_.end(); ++it )
        container.push_back( (void*)(**it).GetID() );        
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::GetPionsSecuring
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
void DEC_PopulationKnowledge::GetPionsSecuring( T_PopulationKnowledgeAgentDiaIDVector& container ) const
{
    container.clear();
    for( CIT_AgentSet it = securers_.begin(); it != securers_.end(); ++it )
        container.push_back( (void*)(**it).GetID() );
}