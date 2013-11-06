// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "IndicatorRequest.h"
#include "IndicatorDefinition_ABC.h"
#include "clients_kernel/Controller.h"
#include "protocol/AarSenders.h"
#include "protocol/ServerPublisher_ABC.h"

// -----------------------------------------------------------------------------
// Name: IndicatorRequest constructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
IndicatorRequest::IndicatorRequest( kernel::Controller& controller, const IndicatorDefinition_ABC& definition, Publisher_ABC& publisher, const QString& displayName )
    : controller_( controller )
    , definition_( definition )
    , publisher_( publisher )
    , displayName_( displayName )
    , done_( false )
    , firstTick_( 0 )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest constructor
// Created: JSR 2013-10-11
// -----------------------------------------------------------------------------
IndicatorRequest::IndicatorRequest( xml::xistream& xis, kernel::Controller& controller, const IndicatorDefinition_ABC& definition, Publisher_ABC& publisher )
    : controller_( controller )
    , definition_( definition )
    , publisher_( publisher )
    , displayName_( xis.attribute< std::string >( "name" ).c_str() )
    , done_( false )
    , firstTick_( 0 )
{
    xis >> xml::start( "parameters" )
            >> xml::list( "parameter", *this, &IndicatorRequest::ReadParameter )
        >> xml::end
        >> xml::optional >> xml::start( "time-range" )
            >> xml::attribute( "start-date", startDate_ )
            >> xml::attribute( "end-date", endDate_ )
        >> xml::end;
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest destructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
IndicatorRequest::~IndicatorRequest()
{
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::GetFirstTick
// Created: JSR 2011-04-26
// -----------------------------------------------------------------------------
unsigned int IndicatorRequest::GetFirstTick() const
{
    return firstTick_;
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::SetParameter
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
void IndicatorRequest::SetParameter( const std::string& name, const std::string& value )
{
    parameters_[ name ] = value;
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::SetTimeRange
// Created: JSR 2011-04-26
// -----------------------------------------------------------------------------
void IndicatorRequest::SetTimeRange( const std::string& startDate, const std::string& endDate )
{
    startDate_ = startDate;
    endDate_ = endDate;
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::Commit
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
void IndicatorRequest::Commit() const
{
    aar::PlotRequest message;
    const std::string request = definition_.Commit( parameters_ );
    message().set_identifier( reinterpret_cast< unsigned int >( this ) );
    message().set_request( request );
    if( !startDate_.empty() && !endDate_.empty() )
    {
        message().mutable_date_time_range()->mutable_begin_date()->set_data( startDate_ );
        message().mutable_date_time_range()->mutable_end_date()->set_data( endDate_ );
    }
    message.Send( publisher_, 0 );
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::SetDisplayName
// Created: JSR 2013-10-14
// -----------------------------------------------------------------------------
void IndicatorRequest::SetDisplayName( const QString& name )
{
    displayName_ = name;
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::Save
// Created: JSR 2013-10-11
// -----------------------------------------------------------------------------
void IndicatorRequest::Save( xml::xostream& xos ) const
{
    const std::string request = definition_.GetName().toStdString();
    xos << xml::start( "request" )
            << xml::attribute( "definition", request )
            << xml::attribute( "name", displayName_ )
            << xml::start( "parameters" );
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
        xos     << xml::start( "parameter")
                    << xml::attribute( "name", it->first )
                    << xml::attribute( "value", it->second )
                << xml::end;
    xos     << xml::end;
    if( !startDate_.empty() && !endDate_.empty() )
    {
        xos << xml::start( "time-range" )
                << xml::attribute( "start-date", startDate_ )
                << xml::attribute( "end-date", endDate_ )
            << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::ReadParameter
// Created: JSR 2013-10-11
// -----------------------------------------------------------------------------
void IndicatorRequest::ReadParameter( xml::xistream& xis )
{
    SetParameter( xis.attribute< std::string >( "name" ), xis.attribute< std::string >( "value" ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::Update
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void IndicatorRequest::Update( const sword::PlotResult& message )
{
    if( message.identifier() == reinterpret_cast< unsigned int >( this ) )
    {
        done_ = true;
        result_.resize( message.values_size() );
        for( int i = 0; i < message.values_size(); ++i )
            result_[ i ] = message.values( i );
        if( message.has_begin_tick() )
            firstTick_ = message.begin_tick();
        error_ = message.error();
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::Update
// Created: SBO 2009-06-11
// -----------------------------------------------------------------------------
void IndicatorRequest::Update( const sword::Indicator& message )
{
    if( !IsFailed() )
        newValues_.push_back( message.value() );
    if( IsDone() )
    {
        result_.insert( result_.end(), newValues_.begin(), newValues_.end() );
        newValues_.clear();
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::GetDefinitionName
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
QString IndicatorRequest::GetDefinitionName() const
{
    return definition_.GetDisplayName();
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::GetDisplayName
// Created: JSR 2013-10-10
// -----------------------------------------------------------------------------
QString IndicatorRequest::GetDisplayName() const
{
    return displayName_.isEmpty() ? GetDefinitionName() : displayName_;
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::IsPending
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
bool IndicatorRequest::IsPending() const
{
    return !done_;
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::IsDone
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
bool IndicatorRequest::IsDone() const
{
    return done_ && !result_.empty();
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::IsFailed
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
bool IndicatorRequest::IsFailed() const
{
    return done_ && result_.empty();
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::ErrorMessage
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
QString IndicatorRequest::ErrorMessage() const
{
    return error_.c_str();
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::Result
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
const IndicatorRequest::T_Data& IndicatorRequest::Result() const
{
    return result_;
}
