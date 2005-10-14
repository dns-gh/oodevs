// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationFlow.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 12:59 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_PopulationFlow.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Knowledge_PopulationFlow.h"

#include "DEC_Knowledge_Population.h"
#include "DEC_Knowledge_PopulationFlowPerception.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Tools/MIL_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

MIL_MOSIDManager DEC_Knowledge_PopulationFlow::idManager_;

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_PopulationFlow, "DEC_Knowledge_PopulationFlow" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlow::DEC_Knowledge_PopulationFlow( DEC_Knowledge_Population& populationKnowledge, const MIL_PopulationFlow& flowKnown )
    : pPopulationKnowledge_    ( &populationKnowledge )
    , pFlowKnown_              ( &flowKnown )
    , nID_                     ( idManager_.GetFreeSimID() )
    , direction_               ( 1., 0. )
    , rSpeed_                  ( 0. )
    , shapes_                  ()
    , nNbrAliveHumans_         ( 0 )
    , nNbrDeadHumans_          ( 0 )
    , pAttitude_               ( 0 ) // $$$
    , rRelevance_              ( 1. )
    , bHumansUpdated_          ( true )
    , bAttitudeUpdated_        ( true )
    , bRealFlowUpdated_        ( true )
    , bRelevanceUpdated_       ( true )
    , bShapeUpdated_           ( true )
    , bSpeedUpdated_           ( true )
    , bDirectionUpdated_       ( true )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
    , pCurrentPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlow::DEC_Knowledge_PopulationFlow()
    : pPopulationKnowledge_    ( 0 )
    , pFlowKnown_              ( 0 )
    , nID_                     ( 0 )
    , direction_               ( 1., 0. )
    , rSpeed_                  ( 0. )
    , shapes_                  ()
    , nNbrAliveHumans_         ( 0 )
    , nNbrDeadHumans_          ( 0 )
    , pAttitude_               ( 0 ) // $$$
    , rRelevance_              ( 1. )
    , bHumansUpdated_          ( true )
    , bAttitudeUpdated_        ( true )
    , bRealFlowUpdated_        ( true )
    , bRelevanceUpdated_       ( true )
    , bShapeUpdated_           ( true )
    , bSpeedUpdated_           ( true )
    , bDirectionUpdated_       ( true )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
    , pCurrentPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlow::~DEC_Knowledge_PopulationFlow()
{
    SendMsgDestruction();
    idManager_.ReleaseSimID( nID_ );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::load( MIL_CheckPointInArchive& file, const uint )
{
    assert( false );
//    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
//
//    file >> const_cast< MIL_AgentPion*& >( pAgentPerceiving_ ) 
//         >> pPopulationPerceived_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( false );
//    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
//         << const_cast< MIL_AgentPion*& >( pAgentPerceiving_ ) 
//         << pPopulationPerceived_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::Prepare()
{
    pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
    pCurrentPerceptionLevel_  = &PHY_PerceptionLevel::notSeen_; 
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::Update
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::Update( const DEC_Knowledge_PopulationFlowPerception& perception )
{
    pCurrentPerceptionLevel_ = &perception.GetCurrentPerceptionLevel();

    T_PointVector& shape = shapes_[ &perception.GetKnowledge().GetAgentPerceiving() ];

    if( shape != perception.GetShape() )
    {
        shape          = perception.GetShape();
        bShapeUpdated_ = true;
    }

    if( nNbrAliveHumans_ != perception.GetNbrAliveHumans() )
    {
        nNbrAliveHumans_ = perception.GetNbrAliveHumans();
        bHumansUpdated_  = true;
    }
    if( nNbrDeadHumans_ != perception.GetNbrDeadHumans() )
    {
        nNbrDeadHumans_  = perception.GetNbrDeadHumans();
        bHumansUpdated_  = true;
    }

    if( !pAttitude_ || *pAttitude_ != perception.GetAttitude() )
    {
        pAttitude_        = &perception.GetAttitude();
        bAttitudeUpdated_ = true;
    }

    if( direction_ != perception.GetDirection() )
    {
        direction_         = perception.GetDirection();
        bDirectionUpdated_ = true;
    }

    if( rSpeed_ != perception.GetSpeed() )
    {
        rSpeed_        = perception.GetSpeed();
        bSpeedUpdated_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::UpdateRelevance
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::UpdateRelevance()
{
    assert( rRelevance_ > 0. );

    if( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ )
    {
        assert( pFlowKnown_ && pFlowKnown_->IsValid() );
        if( rRelevance_ != 1. )
        {
            rRelevance_        = 1.;
            bRelevanceUpdated_ = true;
        }
        return;
    }

    // L'objet réel va être détruit
    if( pFlowKnown_ && !pFlowKnown_->IsValid() )
    {
        pFlowKnown_       = 0;
        bRealFlowUpdated_ = true;
    }

    // Si plus d'objet réel associé est si l'emplacement de l'objet est vu
//    assert( pArmyKnowing_ );
//    if ( !pFlowKnown_ && pArmyKnowing_->IsPerceived( *this ) )
//    {
//        rRelevance_ = 0.;
//        NotifyAttributeUpdated( eAttr_Relevance );
//        return;
//    }   
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::UpdateOnNetwork() const
{
    assert( pPreviousPerceptionLevel_ );
    assert( pCurrentPerceptionLevel_  );
    assert( pAttitude_ );

    if( *pPreviousPerceptionLevel_ == *pCurrentPerceptionLevel_ && !bHumansUpdated_ && !bAttitudeUpdated_ && !bRealFlowUpdated_ && !bRelevanceUpdated_ && !bShapeUpdated_ && !bDirectionUpdated_ && !bSpeedUpdated_ )
        return;

    NET_ASN_MsgPopulationFluxKnowledgeUpdate asnMsg;

    asnMsg.GetAsnMsg().oid_connaissance_flux       = nID_;
    asnMsg.GetAsnMsg().oid_connaissance_population = pPopulationKnowledge_->GetID();
    asnMsg.GetAsnMsg().oid_groupe_possesseur       = pPopulationKnowledge_->GetKnowledgeGroup().GetID();

    if( *pPreviousPerceptionLevel_ != *pCurrentPerceptionLevel_ )
    {
        asnMsg.GetAsnMsg().m.est_percuPresent = 1;
        asnMsg.GetAsnMsg().est_percu          = ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ );
    }

    if( bHumansUpdated_ )
    {
        asnMsg.GetAsnMsg().m.nb_humains_mortsPresent   = 1;
        asnMsg.GetAsnMsg().m.nb_humains_vivantsPresent = 1;
        asnMsg.GetAsnMsg().nb_humains_morts            = nNbrDeadHumans_;
        asnMsg.GetAsnMsg().nb_humains_vivants          = nNbrAliveHumans_;
    }

    if( bAttitudeUpdated_ )
    {
        asnMsg.GetAsnMsg().m.attitudePresent = 1;
        asnMsg.GetAsnMsg().attitude           = pAttitude_->GetAsnID();
    }

    if( bRealFlowUpdated_ )
    {
        asnMsg.GetAsnMsg().m.oid_flux_reelPresent = 1;
        asnMsg.GetAsnMsg().oid_flux_reel          = pFlowKnown_ ? pFlowKnown_->GetID() : 0;
    }

    if( bRelevanceUpdated_ )
    {
        asnMsg.GetAsnMsg().m.pertinencePresent = 1;
        asnMsg.GetAsnMsg().pertinence          = (uint)( rRelevance_ * 100. );
    }

    if( bDirectionUpdated_ )
    {
        asnMsg.GetAsnMsg().m.directionPresent = 1;
        NET_ASN_Tools::WriteDirection( direction_, asnMsg.GetAsnMsg().direction );
    }

    if( bSpeedUpdated_ )
    {
        asnMsg.GetAsnMsg().m.vitessePresent = 1;
        asnMsg.GetAsnMsg().vitesse          = (int)MIL_Tools::ConvertSpeedSimToMos( rSpeed_ );
    }

    if( bShapeUpdated_ )
    {
        asnMsg.GetAsnMsg().m.portions_fluxPresent = 1;
        asnMsg.GetAsnMsg().portions_flux.n        = shapes_.size();
        if( !shapes_.empty() )
        {
            asnMsg.GetAsnMsg().portions_flux.elem = new ASN1T_Itineraire[ asnMsg.GetAsnMsg().portions_flux.n ];
            uint i = 0;
            for( CIT_ShapeMap it = shapes_.begin(); it != shapes_.end(); ++it, ++i )
                NET_ASN_Tools::WritePath( it->second, asnMsg.GetAsnMsg().portions_flux.elem[ i ] );
        }
    }

    asnMsg.Send();
    if( asnMsg.GetAsnMsg().m.portions_fluxPresent )
        NET_ASN_Tools::Delete( asnMsg.GetAsnMsg().portions_flux );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::SendFullState
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::SendFullState() const
{
    assert( pPopulationKnowledge_ );
    assert( pAttitude_ );

    NET_ASN_MsgPopulationFluxKnowledgeUpdate asnMsg;

    asnMsg.GetAsnMsg().oid_connaissance_flux = nID_;
    asnMsg.GetAsnMsg().oid_connaissance_population    = pPopulationKnowledge_->GetID();
    asnMsg.GetAsnMsg().oid_groupe_possesseur          = pPopulationKnowledge_->GetKnowledgeGroup().GetID();
    
    asnMsg.GetAsnMsg().m.nb_humains_mortsPresent   = 1;
    asnMsg.GetAsnMsg().m.nb_humains_vivantsPresent = 1;
    asnMsg.GetAsnMsg().m.attitudePresent           = 1;
    asnMsg.GetAsnMsg().m.est_percuPresent          = 1;
    asnMsg.GetAsnMsg().m.oid_flux_reelPresent      = 1;
    asnMsg.GetAsnMsg().m.pertinencePresent         = 1;
    asnMsg.GetAsnMsg().m.portions_fluxPresent      = 1;
    asnMsg.GetAsnMsg().m.directionPresent          = 1;
    asnMsg.GetAsnMsg().m.vitessePresent            = 1;

    asnMsg.GetAsnMsg().nb_humains_morts   = nNbrDeadHumans_;
    asnMsg.GetAsnMsg().nb_humains_vivants = nNbrAliveHumans_;
    asnMsg.GetAsnMsg().attitude           = pAttitude_->GetAsnID();
    asnMsg.GetAsnMsg().est_percu          = ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ );
    asnMsg.GetAsnMsg().oid_flux_reel      = pFlowKnown_ ? pFlowKnown_->GetID() : 0;
    asnMsg.GetAsnMsg().pertinence         = (uint)( rRelevance_ * 100. );
    asnMsg.GetAsnMsg().vitesse            = (int)MIL_Tools::ConvertSpeedSimToMos( rSpeed_ );
    NET_ASN_Tools::WriteDirection( direction_, asnMsg.GetAsnMsg().direction );
    asnMsg.GetAsnMsg().portions_flux.n    = shapes_.size();
    if( !shapes_.empty() )
    {
        asnMsg.GetAsnMsg().portions_flux.elem = new ASN1T_Itineraire[ asnMsg.GetAsnMsg().portions_flux.n ];
        uint i = 0;
        for( CIT_ShapeMap it = shapes_.begin(); it != shapes_.end(); ++it, ++i )
            NET_ASN_Tools::WritePath( it->second, asnMsg.GetAsnMsg().portions_flux.elem[ i ] );
    }

    asnMsg.Send();  
    if( asnMsg.GetAsnMsg().m.portions_fluxPresent )
        NET_ASN_Tools::Delete( asnMsg.GetAsnMsg().portions_flux );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::SendMsgCreation
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::SendMsgCreation() const
{
    assert( pPopulationKnowledge_ );

    NET_ASN_MsgPopulationFluxKnowledgeCreation asnMsg;

    asnMsg.GetAsnMsg().oid_connaissance_flux       = nID_;
    asnMsg.GetAsnMsg().oid_connaissance_population = pPopulationKnowledge_->GetID();
    asnMsg.GetAsnMsg().oid_groupe_possesseur       = pPopulationKnowledge_->GetKnowledgeGroup().GetID();
    asnMsg.GetAsnMsg().oid_flux_reel               = pFlowKnown_ ? pFlowKnown_->GetID() : 0;

    asnMsg.Send();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::SendMsgDestruction
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::SendMsgDestruction() const
{
    assert( pPopulationKnowledge_ );

    NET_ASN_MsgPopulationFluxKnowledgeDestruction asnMsg;

    asnMsg.GetAsnMsg().oid_connaissance_flux       = nID_;
    asnMsg.GetAsnMsg().oid_connaissance_population = pPopulationKnowledge_->GetID();
    asnMsg.GetAsnMsg().oid_groupe_possesseur       = pPopulationKnowledge_->GetKnowledgeGroup().GetID();

    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::SendStateToNewClient() const
{
    SendMsgCreation();
    SendFullState  ();
}