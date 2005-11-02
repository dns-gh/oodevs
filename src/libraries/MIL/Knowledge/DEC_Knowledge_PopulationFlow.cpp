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
#include "DEC_Knowledge_PopulationFlowPart.h"
#include "DEC_Knowledge_PopulationCollision.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Tools/MIL_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

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
    , flowParts_               ()
    , nNbrAliveHumans_         ( 0 )
    , nNbrDeadHumans_          ( 0 )
    , pAttitude_               ( 0 ) // $$$
    , bHumansUpdated_          ( true )
    , bAttitudeUpdated_        ( true )
    , bRealFlowUpdated_        ( true )
    , bFlowPartsUpdated_       ( true )
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
    , flowParts_               ()
    , nNbrAliveHumans_         ( 0 )
    , nNbrDeadHumans_          ( 0 )
    , pAttitude_               ( 0 ) // $$$
    , bHumansUpdated_          ( true )
    , bAttitudeUpdated_        ( true )
    , bRealFlowUpdated_        ( true )
    , bFlowPartsUpdated_       ( true )
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

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, DEC_Knowledge_PopulationFlow::T_FlowPartMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const DEC_Knowledge_PopulationFlow::T_FlowPartMap& map, const uint )
        {
            file << map.size();
            for ( DEC_Knowledge_PopulationFlow::CIT_FlowPartMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, DEC_Knowledge_PopulationFlow::T_FlowPartMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                MIL_AgentPion* pPion;
                file >> pPion;
                file >> map[ pPion ];
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::load
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> const_cast< DEC_Knowledge_Population*& >( pPopulationKnowledge_ )
         >> const_cast< MIL_PopulationFlow*&       >( pFlowKnown_           )
         >> const_cast< uint&                      >( nID_                  )
         >> direction_
         >> rSpeed_
         >> flowParts_
         >> nNbrAliveHumans_
         >> nNbrDeadHumans_;

    uint nTmpID;
    file >> nTmpID;
    pAttitude_ = MIL_PopulationAttitude::Find( nTmpID );
    assert( pAttitude_ );
    
    file >> nTmpID;
    pPreviousPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nTmpID );
    assert( pPreviousPerceptionLevel_ );

    file >> nTmpID;
    pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nTmpID );
    assert( pCurrentPerceptionLevel_ );

    idManager_.LockSimID( nID_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::save
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << pPopulationKnowledge_
         << pFlowKnown_
         << nID_
         << direction_
         << rSpeed_
         << flowParts_
         << nNbrAliveHumans_
         << nNbrDeadHumans_
         << pAttitude_->GetID()
         << pPreviousPerceptionLevel_->GetID()
         << pCurrentPerceptionLevel_->GetID();
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
    for( CIT_FlowPartMap it = flowParts_.begin(); it != flowParts_.end(); ++it )
        (*it->second).Prepare();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::Update
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::Update( const DEC_Knowledge_PopulationFlowPerception& perception )
{
    pCurrentPerceptionLevel_ = &perception.GetCurrentPerceptionLevel();

    DEC_Knowledge_PopulationFlowPart*& pFlowPart = flowParts_[ &perception.GetKnowledge().GetAgentPerceiving() ];
    if( !pFlowPart )
    {
        bFlowPartsUpdated_ = true;
        pFlowPart = new DEC_Knowledge_PopulationFlowPart();
    }
    if( pFlowPart->Update( perception ) )
        bFlowPartsUpdated_ = true;

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
// Name: DEC_Knowledge_PopulationFlow::Update
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::Update( const DEC_Knowledge_PopulationCollision& collision  )
{
    DEC_Knowledge_PopulationFlowPart*& pFlowPart = flowParts_[ &collision.GetAgentColliding() ];
    if( !pFlowPart )
    {
        bFlowPartsUpdated_ = true;
        pFlowPart = new DEC_Knowledge_PopulationFlowPart();
    }
    if( pFlowPart->Update( collision ) )
        bFlowPartsUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::UpdateRelevance
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::UpdateRelevance()
{
    // L'objet réel va être détruit
    if( pFlowKnown_ && !pFlowKnown_->IsValid() )
    {
        pFlowKnown_       = 0;
        bRealFlowUpdated_ = true;
    }
    
    const MT_Float rMaxLifeTime = pPopulationKnowledge_->GetKnowledgeGroup().GetType().GetKnowledgePopulationMaxLifeTime();
    for( CIT_FlowPartMap it = flowParts_.begin(); it != flowParts_.end(); ++it )
    {
        if( (*it->second).UpdateRelevance( rMaxLifeTime ) )
            bFlowPartsUpdated_ = true;
    }
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationFlow::Clean()
{
    bHumansUpdated_    = false;
    bAttitudeUpdated_  = false;  
    bRealFlowUpdated_  = false;
    bFlowPartsUpdated_ = false;
    bSpeedUpdated_     = false;
    bDirectionUpdated_ = false;
    
    for( IT_FlowPartMap it = flowParts_.begin(); it != flowParts_.end(); )
    {
        if( (*it->second).Clean() )
        {
            delete it->second;
            it = flowParts_.erase( it );
        }
        else 
            ++ it;
    }
    return flowParts_.empty();
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

    if( *pPreviousPerceptionLevel_ == *pCurrentPerceptionLevel_ && !bHumansUpdated_ && !bAttitudeUpdated_ && !bRealFlowUpdated_ && !bFlowPartsUpdated_ && !bDirectionUpdated_ && !bSpeedUpdated_ )
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

    if( bFlowPartsUpdated_ )
    {
        asnMsg.GetAsnMsg().m.portions_fluxPresent = 1;
        asnMsg.GetAsnMsg().portions_flux.n        = flowParts_.size();
        if( !flowParts_.empty() )
        {
            asnMsg.GetAsnMsg().portions_flux.elem = new ASN1T_PortionFlux[ asnMsg.GetAsnMsg().portions_flux.n ];
            uint i = 0;
            for( CIT_FlowPartMap it = flowParts_.begin(); it != flowParts_.end(); ++it, ++i )
                (*it->second).Serialize( asnMsg.GetAsnMsg().portions_flux.elem[ i ] );
        }
    }

    asnMsg.Send();
    if( asnMsg.GetAsnMsg().m.portions_fluxPresent )
    {
        for( uint i = 0; i < asnMsg.GetAsnMsg().portions_flux.n; ++i )
            NET_ASN_Tools::Delete( asnMsg.GetAsnMsg().portions_flux.elem[ i ].forme );
        if( asnMsg.GetAsnMsg().portions_flux.n > 0 )
            delete [] asnMsg.GetAsnMsg().portions_flux.elem;
    }
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
    asnMsg.GetAsnMsg().m.portions_fluxPresent      = 1;
    asnMsg.GetAsnMsg().m.directionPresent          = 1;
    asnMsg.GetAsnMsg().m.vitessePresent            = 1;

    asnMsg.GetAsnMsg().nb_humains_morts   = nNbrDeadHumans_;
    asnMsg.GetAsnMsg().nb_humains_vivants = nNbrAliveHumans_;
    asnMsg.GetAsnMsg().attitude           = pAttitude_->GetAsnID();
    asnMsg.GetAsnMsg().est_percu          = ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ );
    asnMsg.GetAsnMsg().oid_flux_reel      = pFlowKnown_ ? pFlowKnown_->GetID() : 0;
    asnMsg.GetAsnMsg().vitesse            = (int)MIL_Tools::ConvertSpeedSimToMos( rSpeed_ );
    NET_ASN_Tools::WriteDirection( direction_, asnMsg.GetAsnMsg().direction );
    asnMsg.GetAsnMsg().portions_flux.n    = flowParts_.size();
    if( !flowParts_.empty() )
    {
        asnMsg.GetAsnMsg().portions_flux.elem = new ASN1T_PortionFlux[ asnMsg.GetAsnMsg().portions_flux.n ];
        uint i = 0;
        for( CIT_FlowPartMap it = flowParts_.begin(); it != flowParts_.end(); ++it, ++i )
            (*it->second).Serialize( asnMsg.GetAsnMsg().portions_flux.elem[ i ] );
    }

    asnMsg.Send();  
    if( asnMsg.GetAsnMsg().m.portions_fluxPresent )
    {
        for( uint i = 0; i < asnMsg.GetAsnMsg().portions_flux.n; ++i )
            NET_ASN_Tools::Delete( asnMsg.GetAsnMsg().portions_flux.elem[ i ].forme );
        if( asnMsg.GetAsnMsg().portions_flux.n > 0 )
            delete [] asnMsg.GetAsnMsg().portions_flux.elem;
    }
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