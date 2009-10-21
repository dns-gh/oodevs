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
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army.h"
#include "Decision/DEC_Tools.h"

BOOST_CLASS_EXPORT_GUID( DEC_KnowledgeBlackBoard_Automate, "DEC_KnowledgeBlackBoard_Automate" )

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Automate::DEC_KnowledgeBlackBoard_Automate( MIL_Automate& automate )
    : pAutomate_             ( &automate )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Automate::DEC_KnowledgeBlackBoard_Automate()
    : pAutomate_             ( 0 )
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
void DEC_KnowledgeBlackBoard_Automate::serialize( Archive& archive, const uint )
{
    archive & boost::serialization::base_object< DEC_KnowledgeBlackBoard_ABC >( *this )
            & pAutomate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeAgent( const ASN1T_UnitKnowledge& asn ) const
{
    return pAutomate_->GetKnowledgeGroup().GetKnowledge().GetKnowledgeAgentFromID( asn );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeAgent( uint nID ) const
{
    return pAutomate_->GetKnowledgeGroup().GetKnowledge().GetKnowledgeAgentFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeObject( const ASN1T_ObjectKnowledge& asn ) const
{
    return pAutomate_->GetKnowledgeGroup().GetArmy().GetKnowledge().GetKnowledgeObjectFromID( asn );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgeObject( uint nID ) const
{
    return pAutomate_->GetKnowledgeGroup().GetArmy().GetKnowledge().GetKnowledgeObjectFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgePopulation( const ASN1T_PopulationKnowledge& asn ) const
{
    return pAutomate_->GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( asn );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_Automate::ResolveKnowledgePopulation( uint nID ) const
{
    return pAutomate_->GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::SendFullState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Automate::SendFullState() const
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

