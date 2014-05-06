// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationConcentration.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 12:59 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_PopulationConcentration.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_PopulationConcentration.h"
#include "DEC_Knowledge_Population.h"
#include "DEC_Knowledge_PopulationConcentrationPerception.h"
#include "MIL_Time_ABC.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_PopulationConcentration )

MIL_IDManager DEC_Knowledge_PopulationConcentration::idManager_;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationConcentration::DEC_Knowledge_PopulationConcentration( DEC_Knowledge_Population& populationKnowledge, const MIL_PopulationConcentration& concentrationKnown )
    : pPopulationKnowledge_     ( &populationKnowledge )
    , pConcentrationKnown_      ( &concentrationKnown )
    , nID_                      ( idManager_.GetId() )
    , nTimeLastUpdate_          ( 0 )
    , position_                 ( concentrationKnown.GetPosition() )
    , nNbrAliveHumans_          ( 0 )
    , nNbrDeadHumans_           ( 0 )
    , pAttitude_                ( 0 )
    , bReconAttributesValid_    ( false )
    , bHumansUpdated_           ( true )
    , bAttitudeUpdated_         ( true )
    , bRealConcentrationUpdated_( true )
    , bRelevanceUpdated_        ( true )
    , rRelevance_               ( 1. )
    , pPreviousPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pCurrentPerceptionLevel_  ( &PHY_PerceptionLevel::notSeen_ )
    , pHackedPerceptionLevel_   ( &PHY_PerceptionLevel::notSeen_ )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationConcentration::DEC_Knowledge_PopulationConcentration()
    : pPopulationKnowledge_( 0 )
    , pConcentrationKnown_( 0 )
    , nID_( 0 )
    , nTimeLastUpdate_( 0 )
    , position_( 0., 0. )
    , nNbrAliveHumans_( 0 )
    , nNbrDeadHumans_( 0 )
    , pAttitude_( 0 )
    , bHumansUpdated_( true )
    , bAttitudeUpdated_( true )
    , bRealConcentrationUpdated_( true )
    , bRelevanceUpdated_( true )
    , rRelevance_( 1. )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
    , pCurrentPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
    , pHackedPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
    , bReconAttributesValid_( false )
    , rLastRelevanceSent_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration constructor
// Created: LDC 2010-04-13
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationConcentration::DEC_Knowledge_PopulationConcentration( DEC_Knowledge_Population& populationKnowledge, const DEC_Knowledge_PopulationConcentration& concentration )
    : pPopulationKnowledge_     ( &populationKnowledge )
    , pConcentrationKnown_      ( concentration.pConcentrationKnown_ )
    , nID_                      ( idManager_.GetId() )
    , nTimeLastUpdate_          ( concentration.nTimeLastUpdate_ )
    , position_                 ( concentration.position_ )
    , nNbrAliveHumans_          ( concentration.nNbrAliveHumans_ )
    , nNbrDeadHumans_           ( concentration.nNbrDeadHumans_ )
    , pAttitude_                ( concentration.pAttitude_ )
    , bHumansUpdated_           ( concentration.bHumansUpdated_ )
    , bAttitudeUpdated_         ( concentration.bAttitudeUpdated_ )
    , bRealConcentrationUpdated_( concentration.bRealConcentrationUpdated_ )
    , bRelevanceUpdated_        ( concentration.bRelevanceUpdated_ )
    , rRelevance_               ( concentration.rRelevance_ )
    , pPreviousPerceptionLevel_ ( concentration.pPreviousPerceptionLevel_ )
    , pCurrentPerceptionLevel_  ( concentration.pCurrentPerceptionLevel_ )
    , pHackedPerceptionLevel_   ( concentration.pHackedPerceptionLevel_ )
    , bReconAttributesValid_    ( concentration.bReconAttributesValid_ )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationConcentration::~DEC_Knowledge_PopulationConcentration()
{
    SendMsgDestruction();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::load
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> const_cast< DEC_Knowledge_Population*& >( pPopulationKnowledge_ )
         >> const_cast< MIL_PopulationConcentration*& >( pConcentrationKnown_  )
         >> const_cast< unsigned int& >( nID_ )
         >> nTimeLastUpdate_
         >> position_
         >> nNbrAliveHumans_
         >> nNbrDeadHumans_
         >> bReconAttributesValid_;
    idManager_.GetId( nID_, true );
    unsigned int nTmpID;
    bool bAttitudeValid;
    file >> bAttitudeValid;
    if( bAttitudeValid )
    {
        file >> nTmpID;
        pAttitude_ = MIL_PopulationAttitude::Find( nTmpID );
        assert( pAttitude_ );
    }
    file >> rRelevance_;
    file >> pCurrentPerceptionLevel_;
    file >> pPreviousPerceptionLevel_;
    file >> pHackedPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::save
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    bool attitude = ( pAttitude_ != 0 );
    file << pPopulationKnowledge_
         << pConcentrationKnown_
         << nID_
         << nTimeLastUpdate_
         << position_
         << nNbrAliveHumans_
         << nNbrDeadHumans_
         << bReconAttributesValid_
         << attitude;
    if( pAttitude_ )
    {
        unsigned int attitude = pAttitude_->GetID();
        file << attitude;
    }
    file << rRelevance_
         << pCurrentPerceptionLevel_
         << pPreviousPerceptionLevel_
         << pHackedPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::WriteKnowledges
// Created: NPT 2012-08-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::WriteKnowledges( xml::xostream& xos ) const
{
    xos << xml::start( "concentration" )
            << xml::attribute( "id", nID_ )
            << xml::attribute( "last-time-update", nTimeLastUpdate_ )
            << xml::attribute( "relevance", rRelevance_ );
    if( pAttitude_ )
        xos << xml::attribute( "attitude", pAttitude_->GetID() );
    xos     << xml::attribute( "alive-humans", nNbrAliveHumans_ )
            << xml::attribute( "dead-humans", nNbrDeadHumans_ );
    if( pCurrentPerceptionLevel_ )
        xos << xml::attribute( "perception-level", pCurrentPerceptionLevel_->GetID() );
    xos     << xml::attribute( "recon-attributes-valid", bReconAttributesValid_ )
            << xml::start( "position")
                << xml::attribute( "x", position_.rX_ )
                << xml::attribute( "y", position_.rY_ )
            << xml::end;
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::Prepare
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::Prepare()
{
    pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
    pCurrentPerceptionLevel_  = &PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::Update
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::Update( const DEC_Knowledge_PopulationConcentrationPerception& perception )
{
    nTimeLastUpdate_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    pCurrentPerceptionLevel_ = pHackedPerceptionLevel_ == &PHY_PerceptionLevel::notSeen_ ? 
                              &perception.GetCurrentPerceptionLevel() : pHackedPerceptionLevel_;
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
            MIL_Report::PostEventInteger( perception.GetAgentPerceiving(), report::eRC_ArmedCiviliansDetected, armedIndividuals );
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
// Name: DEC_Knowledge_PopulationConcentration::UpdateFromCrowdPerception
// Created: JSR 2013-07-11
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::UpdateFromCrowdPerception( int currentTimeStep )
{
    nTimeLastUpdate_ = currentTimeStep;
    pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::recognized_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::Update
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::Update( const DEC_Knowledge_PopulationCollision& /*collision*/ )
{
    nTimeLastUpdate_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::UpdateRelevance
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::UpdateRelevance()
{
    assert( rRelevance_ > 0. );
    if( *pHackedPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ || *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ )
    {
        assert( pConcentrationKnown_ && pConcentrationKnown_->IsValid() );
        ChangeRelevance( 1. );
        return;
    }
    assert( rRelevance_ >= 0. && rRelevance_ <= 1. );
    // Magic move de la population percue
    if( pConcentrationKnown_ && pConcentrationKnown_->GetPopulation().HasDoneMagicMove() )
        ChangeRelevance( 0. );
    // Degradation : effacement au bout de X minutes
    const double rTimeRelevanceDegradation = ( MIL_Time_ABC::GetTime().GetCurrentTimeStep() - nTimeLastUpdate_ ) / pPopulationKnowledge_->GetMaxLifeTime();
    const double rRelevance = std::max( 0., rRelevance_ - rTimeRelevanceDegradation );
    ChangeRelevance( rRelevance );
    nTimeLastUpdate_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    // L'objet réel va être détruit
    if( pConcentrationKnown_ && !pConcentrationKnown_->IsValid() )
    {
        pConcentrationKnown_ = 0;
        bRealConcentrationUpdated_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::SendMsgCreation
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::SendMsgCreation() const
{
    assert( pPopulationKnowledge_ );
    client::CrowdConcentrationKnowledgeCreation asnMsg;
    asnMsg().mutable_knowledge()->set_id( nID_ );
    asnMsg().mutable_crowd()->set_id( pPopulationKnowledge_->GetID() );
    asnMsg().mutable_knowledge_group()->set_id( pPopulationKnowledge_->GetKnowledgeGroupId() );
    asnMsg().mutable_concentration()->set_id( pConcentrationKnown_ ? pConcentrationKnown_->GetID() : 0 );
    NET_ASN_Tools::WritePoint( position_, *asnMsg().mutable_position() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::SendMsgDestruction
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::SendMsgDestruction() const
{
    assert( pPopulationKnowledge_ );
    client::CrowdConcentrationKnowledgeDestruction asnMsg;
    asnMsg().mutable_knowledge()->set_id( nID_ );
    asnMsg().mutable_crowd()->set_id( pPopulationKnowledge_->GetID() );
    asnMsg().mutable_knowledge_group()->set_id( pPopulationKnowledge_->GetKnowledgeGroupId() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::SendFullState
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::SendFullState()
{
    assert( pPopulationKnowledge_ );
    client::CrowdConcentrationKnowledgeUpdate asnMsg;
    asnMsg().mutable_knowledge()->set_id( nID_ );
    asnMsg().mutable_crowd()->set_id( pPopulationKnowledge_->GetID() );
    asnMsg().mutable_knowledge_group()->set_id( pPopulationKnowledge_->GetKnowledgeGroupId() );
    asnMsg().set_perceived( ( *pHackedPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ ) || ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ ) );
    asnMsg().mutable_concentration()->set_id( pConcentrationKnown_ ? pConcentrationKnown_->GetID() : 0 );
    asnMsg().set_pertinence( static_cast< unsigned int >( rRelevance_ * 100. ) );
    rLastRelevanceSent_ = rRelevance_;
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
// Name: DEC_Knowledge_PopulationConcentration::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::UpdateOnNetwork()
{
    assert( pPreviousPerceptionLevel_ );
    assert( pCurrentPerceptionLevel_  );
    if( *pPreviousPerceptionLevel_ == *pCurrentPerceptionLevel_ && !bHumansUpdated_ && !bAttitudeUpdated_ && !bRealConcentrationUpdated_ && !bRelevanceUpdated_ )
        return;
    client::CrowdConcentrationKnowledgeUpdate asnMsg;
    asnMsg().mutable_knowledge()->set_id( nID_ );
    asnMsg().mutable_crowd()->set_id( pPopulationKnowledge_->GetID() );
    asnMsg().mutable_knowledge_group()->set_id( pPopulationKnowledge_->GetKnowledgeGroupId() );

    if( *pPreviousPerceptionLevel_ != *pCurrentPerceptionLevel_ )
        asnMsg().set_perceived( ( *pHackedPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ ) || ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ ) );
    if( bRealConcentrationUpdated_ )
        asnMsg().mutable_concentration()->set_id( pConcentrationKnown_ ? pConcentrationKnown_->GetID() : 0 );
    if( bRelevanceUpdated_ )
    {
        asnMsg().set_pertinence( static_cast< unsigned int >( rRelevance_ * 100. ) );
        rLastRelevanceSent_ = rRelevance_;
    }
    if( bAttitudeUpdated_ && pAttitude_ )
        asnMsg().set_attitude( pAttitude_->GetAsnID() );
    if( bReconAttributesValid_ )
    {
        if( bHumansUpdated_ )
        {
            asnMsg().set_dead( nNbrDeadHumans_ );
            asnMsg().set_alive( nNbrAliveHumans_ );
        }
    }
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::SendStateToNewClient()
{
    SendMsgCreation();
    SendFullState();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::Clean
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationConcentration::Clean()
{
    bHumansUpdated_ = false;
    bAttitudeUpdated_  = false;
    bRealConcentrationUpdated_ = false;
    bRelevanceUpdated_ = false;
    return rRelevance_ <= 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::ChangeRelevance
// Created: NLD 2005-08-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::ChangeRelevance( double rNewRelevance )
{
    if( rRelevance_ == rNewRelevance )
        return;
    static const double rDeltaForNetwork = 0.05;
    if( fabs( rLastRelevanceSent_ - rNewRelevance ) > rDeltaForNetwork || rNewRelevance == 0. || rNewRelevance == 1. )
        bRelevanceUpdated_ = true;
    rRelevance_ = rNewRelevance;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::HackPerceptionLevel
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::HackPerceptionLevel( const PHY_PerceptionLevel* pPerceptionLevel )
{
    if( *pPerceptionLevel > *pCurrentPerceptionLevel_ )
    {
        rRelevance_ = 1.0;
        pCurrentPerceptionLevel_ = pPerceptionLevel;
        nTimeLastUpdate_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::GetRelevance
// Created: LGY 2013-04-02
// -----------------------------------------------------------------------------
double DEC_Knowledge_PopulationConcentration::GetRelevance() const
{
    return rRelevance_;
}
