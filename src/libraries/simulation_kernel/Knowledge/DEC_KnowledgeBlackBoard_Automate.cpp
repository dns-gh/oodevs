// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeBlackBoard_Automate.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:48 $
// $Revision: 2 $
// $Workfile: DEC_KnowledgeBlackBoard_Automate.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeBlackBoard_Automate.h"
#include "DEC_KnowledgeBlackBoard_Army.h"
#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "MIL_KnowledgeGroup.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KnowledgeBlackBoard_Automate )

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Automate::DEC_KnowledgeBlackBoard_Automate( MIL_Automate& automate )
    : pAutomate_( &automate )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Automate::DEC_KnowledgeBlackBoard_Automate()
    : pAutomate_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Automate::~DEC_KnowledgeBlackBoard_Automate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_KnowledgeBlackBoard_Automate::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KnowledgeBlackBoard_Automate::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeBlackBoard_ABC >( *this );
    archive & pAutomate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeAgent( const sword::Id& asn ) const
{
    auto bbKg = pAutomate_->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        return bbKg->GetKnowledgeAgentFromID( asn.id() );
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeAgent
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeAgent( const MIL_Agent_ABC& agent ) const
{
    auto bbKg = pAutomate_->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        return bbKg->GetKnowledgeAgent( agent );
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeAgent( unsigned int nID ) const
{
    auto bbKg = pAutomate_->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        return bbKg->GetKnowledgeAgentFromID( nID );
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeObject( const sword::Id& asn ) const
{
    return ResolveKnowledgeObject( asn.id() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeObject
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeObject( const MIL_Object_ABC& object ) const
{
    if( pAutomate_->GetKnowledgeGroup()->GetKnowledge() )
        return pAutomate_->GetKnowledgeGroup()->GetKnowledge()->GetKnowledgeObjectContainer().GetKnowledgeObject( object );
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeObject( unsigned int nID ) const
{
    if( pAutomate_->GetKnowledgeGroup()->GetKnowledge() )
        return pAutomate_->GetKnowledgeGroup()->GetKnowledge()->GetKnowledgeObjectContainer().GetKnowledgeObjectFromID( nID );
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeObjectByObjectID
// Created: JSR 2011-09-28
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeObjectByObjectID( unsigned int nID ) const
{
    if( pAutomate_->GetKnowledgeGroup()->GetKnowledge() )
        return pAutomate_->GetKnowledgeGroup()->GetKnowledge()->GetKnowledgeObjectContainer().GetKnowledgeObjectFromObjectID( nID );
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Population > DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgePopulation( const sword::Id& asn ) const
{
    auto bbKg = pAutomate_->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        return bbKg->GetKnowledgePopulationFromID( asn.id() );
    return boost::shared_ptr< DEC_Knowledge_Population >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgePopulation
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Population > DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgePopulation( const MIL_Population& population ) const
{
    auto bbKg = pAutomate_->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        return bbKg->GetKnowledgePopulation( population );
    return boost::shared_ptr< DEC_Knowledge_Population >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Population > DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgePopulation( unsigned int nID ) const
{
    auto bbKg = pAutomate_->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        return bbKg->GetKnowledgePopulationFromID( nID );
    return boost::shared_ptr< DEC_Knowledge_Population >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::SendFullState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Automate::SendFullState( unsigned int /*context*/ ) const
{
//    knowledgeRapForGlobal_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::SendChangedState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Automate::SendChangedState() const
{
//    knowledgeRapForGlobal_
}

