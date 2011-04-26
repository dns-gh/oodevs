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
#include "protocol/Protocol.h"

using namespace sword;

// -----------------------------------------------------------------------------
// Name: IndicatorRequest constructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
IndicatorRequest::IndicatorRequest( kernel::Controller& controller, const IndicatorDefinition_ABC& definition, Publisher_ABC& publisher )
    : controller_( controller )
    , definition_( definition )
    , publisher_ ( publisher )
    , done_      ( false )
    , firstTick_ ( 0 )
    , duration_  ( std::numeric_limits< unsigned int >::max() )

{
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
void IndicatorRequest::SetTimeRange( unsigned int firstTick, unsigned int duration )
{
    firstTick_ = firstTick;
    duration_ = duration;
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
    message().set_request( request.c_str() );
    if( firstTick_ != 0 || duration_ != std::numeric_limits< unsigned int >::max() )
    {
        message().mutable_time_range()->set_first_tick( firstTick_ );
        message().mutable_time_range()->set_duration( duration_ );
    }
    message.Send( publisher_, 0 );
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
        for ( int i = 0; i < message.values_size(); ++i )
            result_[i] = message.values(i);
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
        for( std::vector< double >::iterator iter = newValues_.begin(); iter != newValues_.end(); ++iter )
            result_.push_back( *iter );
        newValues_.clear();
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::GetName
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
QString IndicatorRequest::GetName() const
{
    return definition_.GetName();
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
    return QString( error_.c_str() );
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequest::Result
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
const IndicatorRequest::T_Data& IndicatorRequest::Result() const
{
    return result_;
}
