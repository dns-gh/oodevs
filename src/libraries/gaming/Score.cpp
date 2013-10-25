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
#include "clients_kernel/Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DisplayExtractor_ABC.h"
#include "indicators/Tendency.h"
#include "indicators/Gauge.h"
#include "protocol/AarSenders.h"
#include <boost/algorithm/string.hpp>
#pragma warning( push, 0 )
#include <QtGui/qtreewidget.h>
#pragma warning( pop )

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
Score::Score( const sword::Indicator& message, const ScoreDefinitions& definitions, kernel::Controller& controller, Publisher_ABC& publisher )
    : controller_( controller )
    , publisher_( publisher )
    , name_( ExtractRoot( QString( message.name().c_str() ) ) )
    , definition_( definitions.Get( name_ ) )
    , tendency_( new indicators::Tendency() )
    , gauge_( definition_.CreateGauge() )
    , value_( 0 )
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
// Name: Score::GetDisplayName
// Created: JSR 2013-10-23
// -----------------------------------------------------------------------------
QString Score::GetDisplayName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Score::Update
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void Score::Update( const sword::Indicator& message )
{
    tendency_->AddValue( message.value() );
    value_ = message.value();
    UpdatePlots( message );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Score::UpdatePlots
// Created: SBO 2009-06-12
// -----------------------------------------------------------------------------
void Score::UpdatePlots( const sword::Indicator& message )
{
    if( request_ )
        request_->Update( message );
}

// -----------------------------------------------------------------------------
// Name: Score::Display
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void Score::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Score", "Name" ), name_ )
             .Display( tools::translate( "Score", "Value" ), value_ );
    tendency_->Display( displayer.Item( tools::translate( "Score", "Tendency" ) ) );
    gauge_->Display( displayer.Item( tools::translate( "Score", "Gauge" ) ), value_ );
}

// -----------------------------------------------------------------------------
// Name: Score::Display
// Created: JSR 2012-10-25
// -----------------------------------------------------------------------------
void Score::Display( QTreeWidgetItem* item, kernel::DisplayExtractor_ABC& extractor, int valueCol, int tendencyCol, int gaugeCol ) const
{
    item->setText( valueCol, extractor.GetDisplayName( value_ ) );
    tendency_->Display( item, tendencyCol );
    gauge_->Display( item, extractor, gaugeCol , value_ );
}

// -----------------------------------------------------------------------------
// Name: Score::Commit
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
std::string Score::Commit( const T_Parameters& ) const
{
    return QString( "indicator://%1" ).arg( name_ ).toStdString();
}

// -----------------------------------------------------------------------------
// Name: Score::ConnectTo
// Created: SBO 2009-06-11
// -----------------------------------------------------------------------------
void Score::ConnectTo( IndicatorRequest& request )
{
    request_ = &request;
}
