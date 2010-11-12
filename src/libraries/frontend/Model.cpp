// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Model.h"
#include "RemoteHost.h"
#include "protocol/Launcher.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
Model::Model( const tools::GeneralConfig& config, kernel::Controller& controller )
    : config_( config )
    , controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
Model::~Model()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model::Update
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
template< typename Message >
void Model::UpdateHost( const std::string& host, const Message& message )
{
    std::map< std::string, boost::shared_ptr< RemoteHost > >::iterator it = hosts_.find( host );
    if( it != hosts_.end() )
        it->second->Update( message );
}

// -----------------------------------------------------------------------------
// Name: Model::Update
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
void Model::Update( const std::string& host, const MsgsLauncherToAdmin::MsgExercicesListResponse& message )
{
    if( message.error_code() == MsgsLauncherToAdmin::MsgExercicesListResponse::success )
        UpdateHost( host, message );
}

// -----------------------------------------------------------------------------
// Name: Model::Update
// Created: SBO 2010-10-25
// -----------------------------------------------------------------------------
void Model::Update( const std::string& host, const MsgsLauncherToAdmin::MsgControlStartAck& message )
{
    if( message.error_code() == MsgsLauncherToAdmin::MsgControlStartAck::success )
        UpdateHost( host, message );
}

// -----------------------------------------------------------------------------
// Name: Model::Update
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void Model::Update( const std::string& host, const MsgsLauncherToAdmin::MsgControlStopAck& message )
{
    if( message.error_code() == MsgsLauncherToAdmin::MsgControlStopAck::success )
        UpdateHost( host, message );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateHost
// Created: SBO 2010-10-22
// -----------------------------------------------------------------------------
Host_ABC& Model::CreateHost( LauncherPublisher& publisher, const std::string& host )
{
    boost::shared_ptr< RemoteHost > element( new RemoteHost( publisher, host, controller_ ) );
    hosts_[ element->GetId() ] = element;
    return *element;
}

// -----------------------------------------------------------------------------
// Name: Model::RemoveHost
// Created: SBO 2010-10-22
// -----------------------------------------------------------------------------
void Model::RemoveHost( Host_ABC& host )
{
    hosts_.erase( host.GetId() );
}
