// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Score.h"
#include "IndicatorRequest.h"
#include "Services.h"
#include "ScoreDefinition.h"
#include "ScoreDefinitions.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_gui/Tools.h"
#include "game_asn/AarSenders.h"
#include "indicators/Tendency.h"
#include "indicators/Gauge.h"
#include <boost/algorithm/string.hpp>

namespace
{
    QString ExtractRoot( const QString& name )
    {
        return name.section( '/', 0, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: Score constructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
Score::Score( const ASN1T_MsgIndicator& message, const ScoreDefinitions& definitions, kernel::Controller& controller, Publisher_ABC& publisher )
    : controller_( controller )
    , publisher_( publisher )
    , name_( ExtractRoot( message.name ) )
    , definition_( definitions.Get( name_ ) )
    , tendency_( new indicators::Tendency() )
    , gauge_( definition_.CreateGauge() )
    , value_( 0 )
    , tendencyValue_( 0 )
    , request_( 0 )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Score destructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
Score::~Score()
{
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: Score::GetName
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
QString Score::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Score::Update
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void Score::Update( const ASN1T_MsgIndicator& message )
{
    if( boost::ends_with( message.name, "/Tendency" ) )
        tendencyValue_ = message.value;
    else
    {
        value_ = message.value;
        if( request_ )
            request_->Update( message );
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Score::Display
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void Score::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Score", "Name" ), name_ )
             .Display( tools::translate( "Score", "Value" ), value_ );
    tendency_->Display( displayer.Item( tools::translate( "Score", "Tendency" ) ), tendencyValue_ );
    gauge_->Display( displayer.Item( tools::translate( "Score", "Gauge" ) ), value_ );
}

// -----------------------------------------------------------------------------
// Name: Score::Commit
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
std::string Score::Commit( const T_Parameters& ) const
{
    return QString( "indicator://%1" ).arg( name_ ).ascii();
}

// -----------------------------------------------------------------------------
// Name: Score::ConnectTo
// Created: SBO 2009-06-11
// -----------------------------------------------------------------------------
void Score::ConnectTo( IndicatorRequest& request )
{
    request_ = &request;
}
