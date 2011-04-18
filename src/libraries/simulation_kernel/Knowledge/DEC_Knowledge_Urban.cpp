// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_Urban.h"
#include "Network/NET_Publisher_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Knowledge/DEC_Knowledge_UrbanPerception.h"
#include "Entities/MIL_Army_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_Urban )

MIL_IDManager DEC_Knowledge_Urban::idManager_;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban constructor
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
DEC_Knowledge_Urban::DEC_Knowledge_Urban( const MIL_Army_ABC& army, const UrbanObjectWrapper& wrapper )
    : DEC_Knowledge_Object( army, const_cast< UrbanObjectWrapper& >( wrapper ) )
    , armyId_                 ( army.GetID() )
    , objectId_               ( wrapper.GetID() )
    , rProgressPercent_       ( 0. )
    , rMaxProgressPercent_    ( 0. )
    , nTimeLastUpdate_        ( 0 )
    , bCreatedOnNetwork_      ( false )
    , bCurrentProgressUpdated_( false )
    , bMaxProgressUpdated_    ( false )
    , bLastPerceived_         ( false )
    , rLastProgressSent_      ( 0. )
{
    if( bCreatedOnNetwork_ )
        SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban constructor
// Created: JSR 2010-07-20
// -----------------------------------------------------------------------------
DEC_Knowledge_Urban::DEC_Knowledge_Urban()
    : DEC_Knowledge_Object()
    , armyId_                 ( 0 )
    , objectId_               ( 0 )
    , rProgressPercent_       ( 0. )
    , rMaxProgressPercent_    ( 0. )
    , nTimeLastUpdate_        ( 0 )
    , bCreatedOnNetwork_      ( false )
    , bCurrentProgressUpdated_( false )
    , bMaxProgressUpdated_    ( false )
    , bLastPerceived_         ( false )
    , rLastProgressSent_      ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban destructor
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
DEC_Knowledge_Urban::~DEC_Knowledge_Urban()
{
    if( bCreatedOnNetwork_ )
        SendMsgDestruction();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::load
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Knowledge_Object >( *this )
         >> nTimeLastUpdate_
         >> objectId_
         >> armyId_
         >> rProgressPercent_
         >> rMaxProgressPercent_
         >> bCreatedOnNetwork_
         >> bCurrentProgressUpdated_
         >> bMaxProgressUpdated_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::save
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< DEC_Knowledge_Object >( *this )
         << nTimeLastUpdate_
         << objectId_
         << armyId_
         << rProgressPercent_
         << rMaxProgressPercent_
         << bCreatedOnNetwork_
         << bCurrentProgressUpdated_
         << bMaxProgressUpdated_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::Prepare
// Created: MGD 2009-12-02
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::Prepare()
{
    previousPerceivedByAutomate_.clear();
    perceivedByAutomate_.swap( previousPerceivedByAutomate_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::Update
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::Update( const DEC_Knowledge_UrbanPerception& perception )
{
    if( perception.GetCurrentPerceptionLevel() == PHY_PerceptionLevel::notSeen_ )
        return;
    ComputeProgress( perception.GetPerceiver() );
    UpdatePerceptionSources( perception );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::ComputeProgress
// Created: MGD 2010-08-12
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::ComputeProgress( const MIL_Agent_ABC& agent )
{
    UrbanObjectWrapper* object = 0;
    if( MIL_AgentServer::IsInitialized() )
        object = static_cast< UrbanObjectWrapper* >( MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( objectId_ ) );
    float complexity = object ? object->ComputeComplexity() : 0; // $$$$ ALGO TEMPORAIRE
    float progress = 0.f;
    if( complexity != 0.f )
        progress = 10 / complexity;// $$$$ @TODO MGD Add true physical configuration in ADN
    const UrbanObjectWrapper* urbanBlock = agent.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
    float maxRecce = ( urbanBlock && object == urbanBlock ) ? 1.0f : 0.25f;
    maxRecce = std::max( maxRecce, rProgressPercent_ );
    SetProgress( std::min( std::max( rProgressPercent_ + progress, rProgressPercent_ + 0.001f ), maxRecce ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::SetProgress
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::SetProgress( float progress )
{
    progress = std::max( std::min( progress, 1.f ), 0.f );
    rProgressPercent_ = progress;
    if( rProgressPercent_ > rMaxProgressPercent_ )
        rMaxProgressPercent_ = rProgressPercent_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdatePerceptionSources
// Created: MGD 2009-12-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::UpdatePerceptionSources( const DEC_Knowledge_UrbanPerception& perception )
{
    if( perception.GetCurrentPerceptionLevel() == PHY_PerceptionLevel::notSeen_ )
        return;
    const MIL_Automate* pAutomateSource = &perception.GetPerceiver().GetAutomate();
    const MIL_Agent_ABC& pionSource = perception.GetPerceiver();
    if( std::find( perceivedByAutomate_.begin(), perceivedByAutomate_.end(), pAutomateSource ) == perceivedByAutomate_.end() )
        perceivedByAutomate_.push_back( pAutomateSource );
    if( std::find( perceivedByAgent_.begin(), perceivedByAgent_.end(), &pionSource ) == perceivedByAgent_.end() )
        perceivedByAgent_.push_back( &pionSource );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::UpdateRelevance
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::UpdateRelevance()
{
    if( perceivedByAgent_.size() > 0  )
        return;
    float rTimeRelevanceDegradation = static_cast< float >( ( GetCurrentTimeStep() - nTimeLastUpdate_ ) / 100. ); //config factor
    SetProgress( rProgressPercent_ - rTimeRelevanceDegradation );
    nTimeLastUpdate_ = GetCurrentTimeStep();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::WriteMsgPerceptionSources
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::WriteMsgPerceptionSources( sword::UrbanKnowledgeUpdate& message ) const
{
    for( CIT_PerceptionSource it = perceivedByAutomate_.begin(); it != perceivedByAutomate_.end(); ++it )
        message.mutable_automat_perceptions()->add_elem()->set_id( ( *it )->GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::SendChangedState
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::SendChangedState()
{
    client::UrbanKnowledgeUpdate message;
    bool bMustSend = false;
    if( std::abs( rLastProgressSent_ - rProgressPercent_ ) >= 0.05 )
    {
        float rProgress = static_cast< float >( rProgressPercent_ * 100 ) / 5 * 0.05f; // $$$$ MCO : ^c^v bloc from SendFullState + WTF magic numbers + WTF float computations and casts !
        message().set_progress( static_cast< int >( rProgress * 100 ) );
        message().set_max_progress( static_cast< int >( rMaxProgressPercent_ * 100 ) / 5 * 5 );
        rLastProgressSent_ = rProgress;
        bMustSend = true;
    }
    if( perceivedByAutomate_.size() == 0 && bLastPerceived_ )
    {
        message().set_perceived( false );
        bLastPerceived_ = false;
        bMustSend = true;
    }
    else if( perceivedByAutomate_.size() > 0 && !bLastPerceived_ )
    {
        message().set_perceived( true ) ;
        bLastPerceived_ = true;
        bMustSend = true;
    }
    if( perceivedByAutomate_ != previousPerceivedByAutomate_ )
    {
        WriteMsgPerceptionSources( message() );
        bMustSend = true;
    }
    if( bMustSend )
    {
        nTimeLastUpdate_ = GetCurrentTimeStep();
        message().mutable_knowledge()->set_id( GetID() );
        message().mutable_party()->set_id( armyId_ );
        message().mutable_object()->set_id( GetObjectKnown()->GetID() );
        message.Send( NET_Publisher_ABC::Publisher() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::SendFullState
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::SendFullState()
{
    client::UrbanKnowledgeUpdate message;
    message().mutable_knowledge()->set_id( GetID() );
    message().mutable_party()->set_id( armyId_ );
    message().mutable_object()->set_id( GetObjectKnown()->GetID() );
    float rProgress = static_cast< float >( rProgressPercent_ * 100 ) / 5 * 0.05f; // $$$$ MCO : WTF ^c^v bloc from SendChangedState + WTF magic numbers + WTF float computations and casts !
    message().set_progress( static_cast< int >( rProgress * 100 ) );
    message().set_max_progress( static_cast< int >( rMaxProgressPercent_ * 100 ) / 5 * 5 );
    rLastProgressSent_ = rProgress;
    message().set_perceived( bLastPerceived_ );
    WriteMsgPerceptionSources( message() );
    message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::UpdateOnNetwork
// Created:MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::UpdateOnNetwork()
{
    if( !bCreatedOnNetwork_ )
    {
        SendMsgCreation();
        bCreatedOnNetwork_ = true;
        SendFullState();
        return;
    }
    SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::SendStateToNewClient
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::SendStateToNewClient()
{
    if( bCreatedOnNetwork_ )
    {
        SendMsgCreation();
        SendFullState();
        DEC_Knowledge_Object::SendStateToNewClient();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::SendMsgCreation
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::SendMsgCreation() const
{
    client::UrbanKnowledgeCreation message;
    message().mutable_knowledge()->set_id( GetID() );
    message().mutable_party()->set_id( armyId_ );
    message().mutable_object()->set_id( GetObjectKnown()->GetID() );
    message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::SendMsgDestruction
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::SendMsgDestruction() const
{
    client::UrbanKnowledgeDestruction message;
    message().mutable_knowledge()->set_id( GetID() );
    message().mutable_party()->set_id( armyId_ );
    message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::Clean
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Urban::Clean() const
{
    return false; // always maintain knowledge
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::GetLevel
// Created: MGD 2009-12-01
// -----------------------------------------------------------------------------
float DEC_Knowledge_Urban::GetCurrentRecceProgress() const
{
    return rProgressPercent_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::GetObjectKnownId
// Created: MGD 2010-02-01
// -----------------------------------------------------------------------------
unsigned int DEC_Knowledge_Urban::GetObjectKnownId() const
{
    return objectId_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::IsValid
// Created: MGD 2009-12-04
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Urban::IsValid() const
{
    return true; //@TODO MGD
}
