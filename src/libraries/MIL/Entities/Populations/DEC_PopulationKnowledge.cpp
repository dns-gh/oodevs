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
{

}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge destructor
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
DEC_PopulationKnowledge::~DEC_PopulationKnowledge()
{

}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_PopulationKnowledge::serialize
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_PopulationKnowledge::serialize( Archive& file, const uint )
{
    file & attackers_
         & newAttackers_;
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
