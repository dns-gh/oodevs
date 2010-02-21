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

#include "simulation_kernel_pch.h"
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
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Network/NET_Publisher_ABC.h"
#include "Tools/MIL_Tools.h"
#include "Tools/MIL_IDManager.h"
#include "Network/NET_ASN_Tools.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_PopulationFlow )

MIL_IDManager DEC_Knowledge_PopulationFlow::idManager_;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlow::DEC_Knowledge_PopulationFlow( DEC_Knowledge_Population& populationKnowledge, const MIL_PopulationFlow& flowKnown )
    : pPopulationKnowledge_    ( &populationKnowledge )
    , pFlowKnown_              ( &flowKnown )
    , nID_                     ( idManager_.GetFreeId() )
    , direction_               ( 1., 0. )
    , rSpeed_                  ( 0. )
    , flowParts_               ()
    , rNbrAliveHumans_         ( 0. )
    , rNbrDeadHumans_          ( 0. )
    , pAttitude_               ( 0 ) // $$$
    , bHumansUpdated_          ( true )
    , bAttitudeUpdated_        ( true )
    , bRealFlowUpdated_        ( true )
    , bFlowPartsUpdated_       ( true )
    , bSpeedUpdated_           ( true )
    , bDirectionUpdated_       ( true )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
    , pCurrentPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , bReconAttributesValid_   ( false )
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
    , rNbrAliveHumans_         ( 0. )
    , rNbrDeadHumans_          ( 0. )
    , pAttitude_               ( 0 ) // $$$
    , bHumansUpdated_          ( true )
    , bAttitudeUpdated_        ( true )
    , bRealFlowUpdated_        ( true )
    , bFlowPartsUpdated_       ( true )
    , bSpeedUpdated_           ( true )
    , bDirectionUpdated_       ( true )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
    , pCurrentPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , bReconAttributesValid_   ( false )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlow::~DEC_Knowledge_PopulationFlow()
{
    SendMsgDestruction();
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
        void serialize( Archive& file, DEC_Knowledge_PopulationFlow::T_FlowPartMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const DEC_Knowledge_PopulationFlow::T_FlowPartMap& map, const unsigned int )
        {
            unsigned int size = map.size();
            file << size;
            for ( DEC_Knowledge_PopulationFlow::CIT_FlowPartMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, DEC_Knowledge_PopulationFlow::T_FlowPartMap& map, const unsigned int )
        {
            unsigned int nNbr;
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
void DEC_Knowledge_PopulationFlow::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> const_cast< DEC_Knowledge_Population*& >( pPopulationKnowledge_ )
         >> const_cast< MIL_PopulationFlow*&       >( pFlowKnown_           )
         >> const_cast< unsigned int&                      >( nID_                  )
         >> direction_
         >> rSpeed_
         >> flowParts_
         >> rNbrAliveHumans_
         >> rNbrDeadHumans_
         >> bReconAttributesValid_;

    idManager_.Lock( nID_ );

    unsigned int nTmpID;
    file >> nTmpID;
    pAttitude_ = MIL_PopulationAttitude::Find( nTmpID );
    //assert( pAttitude_ ); // $$$$ SBO 2006-02-24: if popu not recognized, attitude is null (should be default "calme" ?)
    
    file >> nTmpID;
    pPreviousPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nTmpID );
    assert( pPreviousPerceptionLevel_ );

    file >> nTmpID;
    pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nTmpID );
    assert( pCurrentPerceptionLevel_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::save
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned attitudeId = ( pAttitude_ ? pAttitude_->GetID() : 0 ),
             previousId = pPreviousPerceptionLevel_->GetID(),
             currentId = pCurrentPerceptionLevel_->GetID();
    file << pPopulationKnowledge_
         << pFlowKnown_
         << nID_
         << direction_
         << rSpeed_
         << flowParts_
         << rNbrAliveHumans_
         << rNbrDeadHumans_
         << bReconAttributesValid_
         << attitudeId
         << previousId
         << currentId;
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

    assert( pPopulationKnowledge_ );
    if( pPopulationKnowledge_->IsRecon() )
    {
        bReconAttributesValid_ = true;
        if( rNbrAliveHumans_ != perception.GetNbrAliveHumans() )
        {
            rNbrAliveHumans_ = perception.GetNbrAliveHumans();
            bHumansUpdated_  = true;
        }
        if( rNbrDeadHumans_ != perception.GetNbrDeadHumans() )
        {
            rNbrDeadHumans_  = perception.GetNbrDeadHumans();
            bHumansUpdated_  = true;
        }

        if( !pAttitude_ || *pAttitude_ != perception.GetAttitude() )
        {
            pAttitude_        = &perception.GetAttitude();
            bAttitudeUpdated_ = true;
        }
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
        pFlowPart->Prepare();
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
    MT_Float rMaxLifeTime = pPopulationKnowledge_->GetKnowledgeGroup().GetType().GetKnowledgePopulationMaxLifeTime();
    if( pFlowKnown_ && pFlowKnown_->GetPopulation().HasDoneMagicMove() ) 
        rMaxLifeTime = 0.;

    for( CIT_FlowPartMap it = flowParts_.begin(); it != flowParts_.end(); ++it )
    {
        if( (*it->second).UpdateRelevance( rMaxLifeTime ) )
            bFlowPartsUpdated_ = true;
    }

    // L'objet réel va être détruit
    if( pFlowKnown_ && !pFlowKnown_->IsValid() )
    {
        pFlowKnown_       = 0;
        bRealFlowUpdated_ = true;
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
// Name: DEC_Knowledge_PopulationFlow::SendFullState
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::SendFullState() const
{
    assert( pPopulationKnowledge_ );

    client::PopulationFlowKnowledgeUpdate asnMsg;

    asnMsg().set_oid_connaissance_flux( nID_ );
    asnMsg().set_oid_connaissance_population( pPopulationKnowledge_->GetID() );
    asnMsg().set_oid_groupe_possesseur      ( pPopulationKnowledge_->GetKnowledgeGroup().GetID() );
   

    asnMsg().set_est_percu    (  ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ ) );
    asnMsg().set_oid_flux_reel(  pFlowKnown_ ? pFlowKnown_->GetID() : 0 );
    asnMsg().set_vitesse      (  (int)MIL_Tools::ConvertSpeedSimToMos( rSpeed_ ) );
    NET_ASN_Tools::WriteDirection( direction_, *asnMsg().mutable_direction() );
    if( !flowParts_.empty() )
    {
        unsigned int i = 0;
        for( CIT_FlowPartMap it = flowParts_.begin(); it != flowParts_.end(); ++it, ++i )
            (*it->second).Serialize( *asnMsg().mutable_portions_flux()->add_elem() );
    }

    if( bReconAttributesValid_ )
    {
        assert( pAttitude_ );
        asnMsg().set_nb_humains_morts  ( unsigned int( floor( rNbrDeadHumans_  + 0.5f ) ) );
        asnMsg().set_nb_humains_vivants( unsigned int( floor( rNbrAliveHumans_ + 0.5f ) ) );
        asnMsg().set_attitude          ( pAttitude_->GetAsnID() );
    }

    asnMsg.Send( NET_Publisher_ABC::Publisher() );  
    if( asnMsg().has_portions_flux()  )
    {
        asnMsg().mutable_portions_flux()->Clear();
        asnMsg().Clear();
    }
}
   
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::UpdateOnNetwork() const
{
    assert( pPreviousPerceptionLevel_ );
    assert( pCurrentPerceptionLevel_  );

    if( *pPreviousPerceptionLevel_ == *pCurrentPerceptionLevel_ && !bHumansUpdated_ && !bAttitudeUpdated_ && !bRealFlowUpdated_ && !bFlowPartsUpdated_ && !bDirectionUpdated_ && !bSpeedUpdated_ )
        return;

    client::PopulationFlowKnowledgeUpdate asnMsg;

    asnMsg().set_oid_connaissance_flux       ( nID_ );
    asnMsg().set_oid_connaissance_population ( pPopulationKnowledge_->GetID() );
    asnMsg().set_oid_groupe_possesseur       ( pPopulationKnowledge_->GetKnowledgeGroup().GetID() );

    if( *pPreviousPerceptionLevel_ != *pCurrentPerceptionLevel_ )
    {
        asnMsg().set_est_percu( ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ ) );
    }

    if( bRealFlowUpdated_ )
    {
        asnMsg().set_oid_flux_reel( pFlowKnown_ ? pFlowKnown_->GetID() : 0 );
    }

    if( bDirectionUpdated_ )
    {
        NET_ASN_Tools::WriteDirection( direction_, *asnMsg().mutable_direction() );
    }

    if( bSpeedUpdated_ )
    {
        asnMsg().set_vitesse( (int)MIL_Tools::ConvertSpeedSimToMos( rSpeed_ ) );
    }

    if( bFlowPartsUpdated_ )
    {
        if( !flowParts_.empty() )
        {
            unsigned int i = 0;
            for( CIT_FlowPartMap it = flowParts_.begin(); it != flowParts_.end(); ++it, ++i )
                (*it->second).Serialize( *asnMsg().mutable_portions_flux()->add_elem() );
        }
    }

    if( bReconAttributesValid_ )
    {
        assert( pAttitude_ );
        if( bHumansUpdated_ )
        {
            asnMsg().set_nb_humains_morts  ( unsigned int( floor( rNbrDeadHumans_  + 0.5f ) ) );
            asnMsg().set_nb_humains_vivants( unsigned int( floor( rNbrAliveHumans_ + 0.5f ) ) );
        }

        if( bAttitudeUpdated_ )
        {
            asnMsg().set_attitude( pAttitude_->GetAsnID() );
        }
    }

    asnMsg.Send( NET_Publisher_ABC::Publisher() );
    if( asnMsg().has_portions_flux()  )
    {
        asnMsg().mutable_portions_flux()->clear_elem();
        asnMsg().clear_portions_flux();
//        for( uint i = 0; i < asnMsg().portions_flux.n; ++i )
//            ASN_Delete::Delete( asnMsg().portions_flux.elem[ i ].forme );
//        if( asnMsg().portions_flux.n > 0 )
//            delete [] asnMsg().portions_flux.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::SendMsgCreation
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::SendMsgCreation() const
{
    assert( pPopulationKnowledge_ );

    client::PopulationFlowKnowledgeCreation asnMsg;

    asnMsg().set_oid_connaissance_flux       ( nID_ );
    asnMsg().set_oid_connaissance_population ( pPopulationKnowledge_->GetID() );
    asnMsg().set_oid_groupe_possesseur       ( pPopulationKnowledge_->GetKnowledgeGroup().GetID() );
    asnMsg().set_oid_flux_reel               ( pFlowKnown_ ? pFlowKnown_->GetID() : 0 );

    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::SendMsgDestruction
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::SendMsgDestruction() const
{
    assert( pPopulationKnowledge_ );

    client::PopulationFlowKnowledgeDestruction asnMsg;

    asnMsg().set_oid_connaissance_flux( nID_ );
    asnMsg().set_oid_connaissance_population( pPopulationKnowledge_->GetID() );
    asnMsg().set_oid_groupe_possesseur      ( pPopulationKnowledge_->GetKnowledgeGroup().GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
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
