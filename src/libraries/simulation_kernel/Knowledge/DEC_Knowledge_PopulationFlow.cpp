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
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Network/NET_Publisher_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_Tools.h"
#include "protocol/ClientSenders.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_PopulationFlow )

MIL_IDManager DEC_Knowledge_PopulationFlow::idManager_;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlow::DEC_Knowledge_PopulationFlow( DEC_Knowledge_Population& populationKnowledge, const MIL_PopulationFlow& flowKnown )
    : pPopulationKnowledge_    ( &populationKnowledge )
    , pFlowKnown_              ( &flowKnown )
    , pPopulation_             ( &flowKnown.GetPopulation() )
    , nID_                     ( idManager_.GetId() )
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
    , pHackedPerceptionLevel_  ( &PHY_PerceptionLevel::notSeen_ )
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
    , pPopulation_             ( knowledge.pPopulation_ )
    , nID_                     ( idManager_.GetId() )
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
    , pHackedPerceptionLevel_  ( knowledge.pHackedPerceptionLevel_ )
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
    , pPopulation_             ( 0 )
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
    , pHackedPerceptionLevel_  ( &PHY_PerceptionLevel::notSeen_ )
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
         >> const_cast< MIL_Population*& >( pPopulation_ )
         >> const_cast< unsigned int& >( nID_ )
         >> direction_
         >> rSpeed_
         >> flowParts_
         >> nNbrAliveHumans_
         >> nNbrDeadHumans_
         >> bReconAttributesValid_;
    idManager_.GetId( nID_, true );
    unsigned int nTmpID;
    file >> nTmpID;
    pAttitude_ = MIL_PopulationAttitude::Find( nTmpID );
    //assert( pAttitude_ ); // $$$$ SBO 2006-02-24: if popu not recognized, attitude is null (should be default "calme" ?)
    file >> pPreviousPerceptionLevel_
         >> pCurrentPerceptionLevel_
         >> pHackedPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::save
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned attitudeId = ( pAttitude_ ? pAttitude_->GetID() : 0 );
    file << pPopulationKnowledge_
         << pFlowKnown_
         << pPopulation_
         << nID_
         << direction_
         << rSpeed_
         << flowParts_
         << nNbrAliveHumans_
         << nNbrDeadHumans_
         << bReconAttributesValid_
         << attitudeId
         << pPreviousPerceptionLevel_
         << pCurrentPerceptionLevel_
         << pHackedPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::WriteKnowledges
// Created: NPT 2012-08-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::WriteKnowledges( xml::xostream& xos ) const
{
    xos << xml::start( "flow" )
            << xml::attribute( "id", nID_ )
            << xml::attribute( "speed", rSpeed_ );
    xos << xml::attribute( "alive-humans", nNbrAliveHumans_ )
        << xml::attribute( "dead-humans", nNbrDeadHumans_ );
    if( pAttitude_ )
        xos << xml::attribute( "attitude", pAttitude_->GetID() );
    xos << xml::attribute( "recon-attributes-valid", bReconAttributesValid_ );
    if ( pCurrentPerceptionLevel_ )
        xos << xml::attribute( "perception-level", pCurrentPerceptionLevel_->GetID() );
    xos     << xml::start( "direction")
                << xml::attribute( "x", direction_.rX_ )
                << xml::attribute( "y", direction_.rY_ )
            << xml::end;
            flowParts_.WriteKnowledges( xos );
    xos << xml::end;
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
    pCurrentPerceptionLevel_ = pHackedPerceptionLevel_ == &PHY_PerceptionLevel::notSeen_ ?
        &perception.GetCurrentPerceptionLevel() : pHackedPerceptionLevel_;
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
    if( ( pPopulationKnowledge_->IsRecon() || *pCurrentPerceptionLevel_ > PHY_PerceptionLevel::detected_ ) && ( !pAttitude_ || *pAttitude_ != perception.GetAttitude() ) )
    {
        bool postEvent = pAttitude_ == 0;
        pAttitude_ = &perception.GetAttitude();
        bAttitudeUpdated_ = true;
        if( postEvent )
        {
            MIL_Report::PostEvent( perception.GetAgentPerceiving(), report::eRC_AttitudePopulation, pAttitude_->GetID() );
            auto bbKg = perception.GetAgentPerceiving().GetKnowledgeGroup()->GetKnowledge();
            if( bbKg )
            {
                boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( pPopulationKnowledge_->GetID() );
                if( pKnowledge )
                {
                    if( perception.IsDestructingUrbanblocks() )
                        MIL_Report::PostEvent( perception.GetAgentPerceiving(), report::eRC_CloseCrowdUrbanDestruction, pKnowledge );
                    if( perception.IsDemonstrating() )
                        MIL_Report::PostEvent( perception.GetAgentPerceiving(), report::eRC_CloseCrowdDemonstration, pKnowledge );
                }
            }
        }
    }
    if( *pPreviousPerceptionLevel_ != PHY_PerceptionLevel::identified_ && *pCurrentPerceptionLevel_ == PHY_PerceptionLevel::identified_ )
    {
        unsigned int armedIndividuals = static_cast< unsigned int >( pPopulationKnowledge_->GetPopulationKnown().GetArmedIndividuals() * 100u );
        if( armedIndividuals > 0u )
            MIL_Report::PostEvent( perception.GetAgentPerceiving(), report::eRC_ArmedCiviliansDetected, armedIndividuals );
    }
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
// Name: DEC_Knowledge_PopulationFlow::UpdateFromCrowdPerception
// Created: JSR 2013-07-11
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::UpdateFromCrowdPerception( int /*currentTimeStep*/ )
{
    pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::recognized_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::UpdateRelevance
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::UpdateRelevance()
{
    double rMaxLifeTime = pPopulationKnowledge_->GetMaxLifeTime();
    const bool magicMove = pPopulation_->HasDoneMagicMove();
    if( magicMove )
    {
        rMaxLifeTime = 0.;
    }
    if( flowParts_.UpdateRelevance( rMaxLifeTime ) )
        bFlowPartsUpdated_ = true;
    // L'objet réel va être détruit
    if( magicMove || ( pFlowKnown_ && !pFlowKnown_->IsValid() ) )
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
    asnMsg().mutable_knowledge_group()->set_id( pPopulationKnowledge_->GetKnowledgeGroupId() );
    asnMsg().set_perceived( ( *pHackedPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ ) || ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ ) );
    asnMsg().mutable_flow()->set_id( pFlowKnown_ ? pFlowKnown_->GetID() : 0 );
    asnMsg().set_speed( MIL_Tools::ConvertSpeedSimToMos( rSpeed_ ) );
    NET_ASN_Tools::WriteDirection( direction_, *asnMsg().mutable_direction() );
    flowParts_.Serialize( *asnMsg().mutable_parts()->add_elem() );
    if( pAttitude_ )
        asnMsg().set_attitude( pAttitude_->GetAsnID() );
    if( bReconAttributesValid_ )
    {
        asnMsg().set_dead( nNbrDeadHumans_ );
        asnMsg().set_alive( nNbrAliveHumans_ );
    }
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
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
    asnMsg().mutable_knowledge_group()->set_id( pPopulationKnowledge_->GetKnowledgeGroupId() );
    if( *pPreviousPerceptionLevel_ != *pCurrentPerceptionLevel_ )
        asnMsg().set_perceived( ( *pHackedPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ ) || ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ ) );
    if( bRealFlowUpdated_ )
        asnMsg().mutable_flow()->set_id( pFlowKnown_ ? pFlowKnown_->GetID() : 0 );
    if( bDirectionUpdated_ )
        NET_ASN_Tools::WriteDirection( direction_, *asnMsg().mutable_direction() );
    if( bSpeedUpdated_ )
        asnMsg().set_speed( MIL_Tools::ConvertSpeedSimToMos( rSpeed_ ) );
    if( bFlowPartsUpdated_ )
        flowParts_.Serialize( *asnMsg().mutable_parts()->add_elem() );
    if( bAttitudeUpdated_ && pAttitude_ )
        asnMsg().set_attitude( pAttitude_->GetAsnID() );
    if( bReconAttributesValid_ && bHumansUpdated_ )
    {
        asnMsg().set_dead( nNbrDeadHumans_  );
        asnMsg().set_alive( nNbrAliveHumans_ );
    }
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
    if( asnMsg().has_parts() )
    {
        asnMsg().mutable_parts()->clear_elem();
        asnMsg().clear_parts();
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
    asnMsg().mutable_knowledge_group()->set_id( pPopulationKnowledge_->GetKnowledgeGroupId() );
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
    asnMsg().mutable_knowledge_group()->set_id( pPopulationKnowledge_->GetKnowledgeGroupId() );
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
    pHackedPerceptionLevel_ = pCurrentPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::GetRelevance
// Created: LGY 2013-04-02
// -----------------------------------------------------------------------------
double DEC_Knowledge_PopulationFlow::GetRelevance() const
{
    return flowParts_.GetRelevance();
}
