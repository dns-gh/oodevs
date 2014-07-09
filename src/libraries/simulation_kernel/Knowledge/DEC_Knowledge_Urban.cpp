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
#include "MIL_AgentServer.h"
#include "Network/NET_Publisher_ABC.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Knowledge/DEC_Knowledge_UrbanPerception.h"
#include "Entities/MIL_Army_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_Urban )

MIL_IDManager DEC_Knowledge_Urban::idManager_;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban constructor
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
DEC_Knowledge_Urban::DEC_Knowledge_Urban( const MIL_Army_ABC& army, const MIL_UrbanObject_ABC& wrapper )
    : DEC_Knowledge_Object( army, const_cast< MIL_UrbanObject_ABC& >( wrapper ), false )
    , armyId_                 ( army.GetID() )
    , objectId_               ( wrapper.GetID() )
    , urbanObject_            ( &wrapper )
    , rProgressPercent_       ( 0. )
    , rMaxProgressPercent_    ( 0. )
    , nTimeLastUpdate_        ( 0 )
    , bCreatedOnNetwork_      ( false )
    , bCurrentProgressUpdated_( false )
    , bMaxProgressUpdated_    ( false )
    , bLastPerceived_         ( false )
    , nLastProgressSent_      ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban constructor
// Created: JSR 2010-07-20
// -----------------------------------------------------------------------------
DEC_Knowledge_Urban::DEC_Knowledge_Urban()
    : DEC_Knowledge_Object()
    , armyId_                 ( 0 )
    , objectId_               ( 0 )
    , urbanObject_            ( 0 )
    , rProgressPercent_       ( 0. )
    , rMaxProgressPercent_    ( 0. )
    , nTimeLastUpdate_        ( 0 )
    , bCreatedOnNetwork_      ( true )
    , bCurrentProgressUpdated_( false )
    , bMaxProgressUpdated_    ( false )
    , bLastPerceived_         ( false )
    , nLastProgressSent_      ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban destructor
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
DEC_Knowledge_Urban::~DEC_Knowledge_Urban()
{
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
// Created: JSR 2014-07-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::ComputeProgress( const MIL_Agent_ABC& agent )
{
    if( agent.IsDead() )
        return;
    // ugly, urbanObject_ is just for simulation_kernel_test. should be cleaned (or not...)
    if( !urbanObject_ && MIL_AgentServer::IsInitialized() )
        urbanObject_ = static_cast< MIL_UrbanObject_ABC* >( MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( objectId_ ) );
    if( !urbanObject_ )
        return;
    float maxRecce = ( agent.Get< PHY_RoleInterface_Perceiver >().IsReconnoitering( urbanObject_ )
                       || agent.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock() == urbanObject_ ) ?
                       1.f : 0.25f;
    if( rProgressPercent_ >= maxRecce )
        return;
    const MT_Rect& box = urbanObject_->GetLocalisation().GetBoundingBox();
    const double radius = box.GetPointDownLeft().Distance( box.GetPointUpRight() ) * 0.5;
    double currentRadius = rProgressPercent_ * radius;
    const double ratio = urbanObject_->GetFloorNumber() * urbanObject_->GetOccupation();
    currentRadius += DEC_GeometryFunctions::GetUrbanSearchSpeed( urbanObject_ ) / ( ratio ? ratio : 1.f );
    rProgressPercent_ = std::min( maxRecce, radius == 0 ? 1.f : static_cast< float >( currentRadius / radius ) );
    rMaxProgressPercent_ = std::max( rMaxProgressPercent_, rProgressPercent_ );
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
    if( std::find( perceivedByAutomate_.begin(), perceivedByAutomate_.end(), pAutomateSource ) == perceivedByAutomate_.end() )
        perceivedByAutomate_.push_back( pAutomateSource );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::UpdateRelevance
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::UpdateRelevance()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::WriteMsgPerceptionSources
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::WriteMsgPerceptionSources( sword::UrbanKnowledgeUpdate& message ) const
{
    message.mutable_automat_perceptions();
    for( auto it = perceivedByAutomate_.begin(); it != perceivedByAutomate_.end(); ++it )
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
    int progress = static_cast< int >( 100.f * rProgressPercent_ );
    if( std::abs( nLastProgressSent_ - progress ) > 0 )
    {
        message().set_progress( progress );
        message().set_max_progress( static_cast< int >( rMaxProgressPercent_ * 100 ) );
        nLastProgressSent_ = progress;
        bMustSend = true;
    }
    if( perceivedByAutomate_.empty() && bLastPerceived_ )
    {
        message().set_perceived( false );
        bLastPerceived_ = false;
        bMustSend = true;
    }
    else if( !perceivedByAutomate_.empty() && !bLastPerceived_ )
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
        nTimeLastUpdate_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
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
    int progress = static_cast< int >( rProgressPercent_ * 100 );
    message().set_progress( progress );
    message().set_max_progress( static_cast< int >( rMaxProgressPercent_ * 100 ) );
    nLastProgressSent_ = progress;
    message().set_perceived( bLastPerceived_ );
    if( !perceivedByAutomate_.empty() )
        WriteMsgPerceptionSources( message() );
    message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::UpdateOnNetwork
// Created:MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::UpdateOnNetwork()
{
    // $$$$ _RC_ JSR 2011-07-28: A quoi ça sert d'envoyer les messages de création après le premier tick, et pas dans le constructeur? virer bCreatedOnNetwork_ si ça sert à rien.
    if( !bCreatedOnNetwork_ )
    {
        bCreatedOnNetwork_ = true;
        SendMsgCreation();
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
    SendMsgCreation();
    SendFullState();
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
