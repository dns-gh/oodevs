// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ScoreModel.h"
#include "IndicatorRequest.h"
#include "IndicatorRequests.h"
#include "ReplayScore.h"
#include "Score.h"
#include "ScoreDefinition.h"
#include "ScoreDefinitions.h"
#include "Services.h"
#include "clients_kernel/Controllers.h"
#include "protocol/AarSenders.h"
#include "protocol/Dispatcher.h"
#include "protocol/ReplaySenders.h"

using namespace sword;

// -----------------------------------------------------------------------------
// Name: ScoreModel constructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
ScoreModel::ScoreModel( kernel::Controllers& controllers, Publisher_ABC& publisher, const ScoreDefinitions& definitions )
    : controllers_( controllers )
    , publisher_( publisher )
    , definitions_( definitions )
    , requests_( new IndicatorRequests( controllers_.controller_, publisher_ ) )
    , hasReplay_( false )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ScoreModel destructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
ScoreModel::~ScoreModel()
{
    Purge();
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ScoreModel::Purge
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void ScoreModel::Purge()
{
    DeleteAll();
    scoreRequests_.clear();
    requests_->Purge();
}

// -----------------------------------------------------------------------------
// Name: ScoreModel::NotifyUpdated
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void ScoreModel::NotifyUpdated( const Services& services )
{
    hasReplay_ = services.HasService( sword::service_replay );
}

namespace
{
    QString ExtractRoot( const QString& name )
    {
        return name.section( '/', 0, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreModel::Update
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void ScoreModel::Update( const sword::Indicator& message )
{
    if( !definitions_.Find( QString( message.name().c_str() ) ) )
        return;
    Score* score = Find( ExtractRoot( QString( message.name().c_str() ) ) );
    if( !score )
    {
        if( hasReplay_ )
            score = new ReplayScore( message, definitions_, controllers_.controller_, publisher_ );
        else
            score = new Score( message, definitions_, controllers_.controller_, publisher_ );
        Register( score->GetName(), *score );
    }
    score->Update( message );
}

// -----------------------------------------------------------------------------
// Name: ScoreModel::Update
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void ScoreModel::Update( const sword::PlotResult& message )
{
    requests_->Update( message );
}

// -----------------------------------------------------------------------------
// Name: ScoreModel::CreateRequest
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
IndicatorRequest& ScoreModel::CreateRequest( Score& score )
{
    T_ScoreRequests::iterator it = scoreRequests_.find( &score );
    IndicatorRequest* request;
    if( it != scoreRequests_.end() )
        request = it->second;
    else
    {
        request = scoreRequests_[ &score ] = &requests_->CreateRequest( score );
        score.ConnectTo( *request );
    }
    request->Commit();
    return *request;
}
