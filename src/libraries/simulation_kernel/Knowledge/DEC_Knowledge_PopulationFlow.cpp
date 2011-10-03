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
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Network/NET_Publisher_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_Tools.h"
#include "protocol/ClientSenders.h"
#include <boost/serialization/split_free.hpp>

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
    , nNbrAliveHumans_         ( 0 )
    , nNbrDeadHumans_          ( 0 )
    , pAttitude_               ( 0 )
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
// Created: LDC 2010-04-13
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlow::DEC_Knowledge_PopulationFlow( DEC_Knowledge_Population& populationKnowledge, const DEC_Knowledge_PopulationFlow& knowledge )
    : pPopulationKnowledge_    ( &populationKnowledge )
    , pFlowKnown_              ( knowledge.pFlowKnown_ )
    , nID_                     ( idManager_.GetFreeId() )
    , direction_               ( knowledge.direction_ )
    , rSpeed_                  ( knowledge.rSpeed_ )
    , flowParts_               ( knowledge.flowParts_ )
    , nNbrAliveHumans_         ( knowledge.nNbrAliveHumans_ )
    , nNbrDeadHumans_          ( knowledge.nNbrDeadHumans_ )
    , pAttitude_               ( knowledge.pAttitude_ )
    , bHumansUpdated_          ( knowledge.bHumansUpdated_ )
    , bAttitudeUpdated_        ( knowledge.bAttitudeUpdated_ )
    , bRealFlowUpdated_        ( knowledge.bRealFlowUpdated_ )
    , bFlowPartsUpdated_       ( knowledge.bFlowPartsUpdated_ )
    , bSpeedUpdated_           ( knowledge.bSpeedUpdated_ )
    , bDirectionUpdated_       ( knowledge.bDirectionUpdated_ )
    , pPreviousPerceptionLevel_( knowledge.pPreviousPerceptionLevel_ )
    , pCurrentPerceptionLevel_ ( knowledge.pCurrentPerceptionLevel_ )
    , bReconAttributesValid_   ( knowledge.bReconAttributesValid_ )
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
    , nNbrAliveHumans_         ( 0 )
    , nNbrDeadHumans_          ( 0 )
    , pAttitude_               ( 0 )
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlow::~DEC_Knowledge_PopulationFlow()
{
    SendMsgDestruction();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::load
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> const_cast< DEC_Knowledge_Population*& >( pPopulationKnowledge_ )
         >> const_cast< MIL_PopulationFlow*& >( pFlowKnown_ )
         >> const_cast< unsigned int& >( nID_ )
         >> direction_
         >> rSpeed_
         >> flowParts_
         >> nNbrAliveHumans_
         >> nNbrDeadHumans_
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
         << nNbrAliveHumans_
         << nNbrDeadHumans_
         << bReconAttributesValid_
         << attitudeId
         << previousId
         << currentId;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::Prepare()
{
    pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
    pCurrentPerceptionLevel_  = &PHY_PerceptionLevel::notSeen_;
    flowParts_.Prepare();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::Update
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::Update( const DEC_Knowledge_PopulationFlowPerception& perception )
{
    pCurrentPerceptionLevel_ = &perception.GetCurrentPerceptionLevel();
    if( flowParts_.Update( perception ) )
        bFlowPartsUpdated_ = true;
    if( direction_ != perception.GetDirection() )
    {
        direction_ = perception.GetDirection();
        bDirectionUpdated_ = true;
    }
    if( rSpeed_ != perception.GetSpeed() )
    {
        rSpeed_ = perception.GetSpeed();
        bSpeedUpdated_ = true;
    }
    assert( pPopulationKnowledge_ );
    if( pPopulationKnowledge_->IsRecon() )
    {
        bReconAttributesValid_ = true;
        if( nNbrAliveHumans_ != perception.GetNbrAliveHumans() )
        {
            nNbrAliveHumans_ = perception.GetNbrAliveHumans();
            bHumansUpdated_ = true;
        }
        if( nNbrDeadHumans_ != perception.GetNbrDeadHumans() )
        {
            nNbrDeadHumans_ = perception.GetNbrDeadHumans();
            bHumansUpdated_ = true;
        }
        if( !pAttitude_ || *pAttitude_ != perception.GetAttitude() )
        {
            pAttitude_ = &perception.GetAttitude();
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
    if( flowParts_.Update( collision ) )
        bFlowPartsUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::UpdateRelevance
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::UpdateRelevance()
{
    double rMaxLifeTime = pPopulationKnowledge_->GetKnowledgeGroup().GetType().GetKnowledgePopulationMaxLifeTime();
    if( pFlowKnown_ && pFlowKnown_->GetPopulation().HasDoneMagicMove() )
        rMaxLifeTime = 0.;
    if( flowParts_.UpdateRelevance( rMaxLifeTime ) )
        bFlowPartsUpdated_ = true;
    // L'objet réel va être détruit
    if( pFlowKnown_ && !pFlowKnown_->IsValid() )
    {
        pFlowKnown_ = 0;
        bRealFlowUpdated_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationFlow::Clean()
{
    bHumansUpdated_ = false;
    bAttitudeUpdated_ = false;
    bRealFlowUpdated_ = false;
    bFlowPartsUpdated_ = false;
    bSpeedUpdated_ = false;
    bDirectionUpdated_ = false;
    return flowParts_.Clean();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::SendFullState
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::SendFullState() const
{
    assert( pPopulationKnowledge_ );
    client::CrowdFlowKnowledgeUpdate asnMsg;
    asnMsg().mutable_knowledge()->set_id( nID_ );
    asnMsg().mutable_crowd()->set_id( pPopulationKnowledge_->GetID() );
    asnMsg().mutable_knowledge_group()->set_id( pPopulationKnowledge_->GetKnowledgeGroup().GetId() );
    asnMsg().set_perceived( ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ ) );
    asnMsg().mutable_flow()->set_id( pFlowKnown_ ? pFlowKnown_->GetID() : 0 );
    asnMsg().set_speed( static_cast< int >( MIL_Tools::ConvertSpeedSimToMos( rSpeed_ ) ) );
    NET_ASN_Tools::WriteDirection( direction_, *asnMsg().mutable_direction() );
    flowParts_.Serialize( *asnMsg().mutable_parts()->add_elem() );
    if( bReconAttributesValid_ )
    {
        assert( pAttitude_ );
        asnMsg().set_dead( nNbrDeadHumans_ );
        asnMsg().set_alive( nNbrAliveHumans_ );
        asnMsg().set_attitude( pAttitude_->GetAsnID() );
    }
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
    if( asnMsg().has_parts() )
    {
        asnMsg().mutable_parts()->Clear();
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
    assert( pCurrentPerceptionLevel_ );
    if( *pPreviousPerceptionLevel_ == *pCurrentPerceptionLevel_ && !bHumansUpdated_ && !bAttitudeUpdated_ && !bRealFlowUpdated_ && !bFlowPartsUpdated_ && !bDirectionUpdated_ && !bSpeedUpdated_ )
        return;
    client::CrowdFlowKnowledgeUpdate asnMsg;
    asnMsg().mutable_knowledge()->set_id( nID_ );
    asnMsg().mutable_crowd()->set_id( pPopulationKnowledge_->GetID() );
    asnMsg().mutable_knowledge_group()->set_id( pPopulationKnowledge_->GetKnowledgeGroup().GetId() );
    if( *pPreviousPerceptionLevel_ != *pCurrentPerceptionLevel_ )
        asnMsg().set_perceived( ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ ) );
    if( bRealFlowUpdated_ )
        asnMsg().mutable_flow()->set_id( pFlowKnown_ ? pFlowKnown_->GetID() : 0 );
    if( bDirectionUpdated_ )
        NET_ASN_Tools::WriteDirection( direction_, *asnMsg().mutable_direction() );
    if( bSpeedUpdated_ )
        asnMsg().set_speed( static_cast< int >( MIL_Tools::ConvertSpeedSimToMos( rSpeed_ ) ) );
    if( bFlowPartsUpdated_ )
        flowParts_.Serialize( *asnMsg().mutable_parts()->add_elem() );
    if( bReconAttributesValid_ )
    {
        assert( pAttitude_ );
        if( bHumansUpdated_ )
        {
            asnMsg().set_dead( nNbrDeadHumans_  );
            asnMsg().set_alive( nNbrAliveHumans_ );
        }
        if( bAttitudeUpdated_ )
            asnMsg().set_attitude( pAttitude_->GetAsnID() );
    }
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
    if( asnMsg().has_parts() )
    {
        asnMsg().mutable_parts()->clear_elem();
        asnMsg().clear_parts();
//        for( unsigned int i = 0; i < asnMsg().parts.n; ++i )
//            ASN_Delete::Delete( asnMsg().parts.elem[ i ].forme );
//        if( asnMsg().parts.n > 0 )
//            delete [] asnMsg().parts.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::SendMsgCreation
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::SendMsgCreation() const
{
    assert( pPopulationKnowledge_ );
    client::CrowdFlowKnowledgeCreation asnMsg;
    asnMsg().mutable_knowledge()->set_id( nID_ );
    asnMsg().mutable_crowd()->set_id( pPopulationKnowledge_->GetID() );
    asnMsg().mutable_knowledge_group()->set_id( pPopulationKnowledge_->GetKnowledgeGroup().GetId() );
    asnMsg().mutable_flow()->set_id( pFlowKnown_ ? pFlowKnown_->GetID() : 0 );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::SendMsgDestruction
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::SendMsgDestruction() const
{
    assert( pPopulationKnowledge_ );
    client::CrowdFlowKnowledgeDestruction asnMsg;
    asnMsg().mutable_knowledge()->set_id( nID_ );
    asnMsg().mutable_crowd()->set_id( pPopulationKnowledge_->GetID() );
    asnMsg().mutable_knowledge_group()->set_id( pPopulationKnowledge_->GetKnowledgeGroup().GetId() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::SendStateToNewClient() const
{
    SendMsgCreation();
    SendFullState();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::HackPerceptionLevel
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::HackPerceptionLevel( const PHY_PerceptionLevel* pPerceptionLevel )
{
    if( *pPerceptionLevel > *pCurrentPerceptionLevel_ )
        pCurrentPerceptionLevel_ = pPerceptionLevel;
}