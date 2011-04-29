// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Task.h"
#include "Connectors.h"
#include "Result_ABC.h"
#include <xeumeuleu/xml.hpp>
#include "dispatcher/MessageLoader_ABC.h"
#pragma warning( push )
#pragma warning( disable : 4996 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: Task constructor
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
Task::Task( unsigned int firstTick, unsigned int lastTick )
    : firstTick_    ( firstTick )
    , lastTick_     ( lastTick )
    , firstTickRead_( 0 )
    , hasFirstTick_ ( false )
    , currentTick_  ( 0 )
    , insideFrame_  ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Task destructor
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
Task::~Task()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Task::AddExtractor
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
void Task::AddExtractor( boost::shared_ptr< ModelFunction_ABC > function )
{
    composite_.Add( function );
}

// -----------------------------------------------------------------------------
// Name: Task::AddFunction
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void Task::AddFunction( const std::string& name, boost::shared_ptr< Slot_ABC > function )
{
    boost::shared_ptr< Slot_ABC >& s = slots_[ name ];
    if( s )
        throw std::runtime_error( "A function '" + name + "' already exists" );
    s = function;
}

// -----------------------------------------------------------------------------
// Name: Task::AddFunction
// Created: AGE 2007-09-13
// -----------------------------------------------------------------------------
void Task::AddFunction( boost::shared_ptr< Slot_ABC > function )
{
    objects_.push_back( function );
}

// -----------------------------------------------------------------------------
// Name: Task::AddConnector
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
void Task::AddConnector( const std::string& name, boost::shared_ptr< Connector_ABC > connector )
{
    boost::shared_ptr< Connector_ABC >& c = connectors_[ name ];
    if( c )
        throw std::runtime_error( "A function '" + name + "' already exists" );
    c = connector;
}

// -----------------------------------------------------------------------------
// Name: Task::Connect
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void Task::Connect( xml::xistream& xis )
{
    xis >> xml::list( *this, &Task::Connect );
}

// -----------------------------------------------------------------------------
// Name: Task::Connect
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void Task::Connect( const std::string& , xml::xistream& xis )
{
    const std::string name  = xis.attribute< std::string >( "id", "plot" );
    const std::string input = xis.attribute< std::string >( "input", "" );
    if( !name.empty() && !input.empty() )
        Connect( name, input );
}

// -----------------------------------------------------------------------------
// Name: Task::Connect
// Created: AGE 2007-09-13
// -----------------------------------------------------------------------------
void Task::Connect( const std::string& name, const std::string& input )
{
    std::vector< std::string > inputs;
    boost::algorithm::split( inputs, input, boost::algorithm::is_any_of( "," ) );
    for( unsigned i = 0; i < inputs.size(); ++i )
        Connect( name, inputs[i], i );
}

// -----------------------------------------------------------------------------
// Name: Task::Connect
// Created: AGE 2007-09-13
// -----------------------------------------------------------------------------
void Task::Connect( const std::string& name, const std::string& input, unsigned i )
{
    CIT_Connectors itC = connectors_.find( input );
    CIT_Slots itS = slots_.find( name );
    if( itC == connectors_.end() )
        throw std::runtime_error( "Could not connect '" + input + "' to '" + name + "' : could not find '" + input + "'" );
    else if( itS == slots_.end() )
        throw std::runtime_error( "Could not connect '" + input + "' to '" + name + "' : could not find '" + name + "'" );
    else
        itC->second->Connect( *(itS->second), i );
}

// -----------------------------------------------------------------------------
// Name: Task::SetResult
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void Task::SetResult( boost::shared_ptr< Result_ABC > output )
{
    result_ = output;
    AddFunction( "plot", output );
}

// -----------------------------------------------------------------------------
// Name: Task::Receive
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void Task::Receive( const sword::SimToClient& wrapper )
{
    try
    {
        if( wrapper.message().has_control_begin_tick() )
        {
             // $$$$ JSR 2011-04-29: On envoie un BeginTick si on n'en a pas encore eu, si on n'est pas sur la première keyframe ou si on a changé de tick courant.
            unsigned int tick = wrapper.message().control_begin_tick().current_tick();
            if( !hasFirstTick_ || ( currentTick_ != firstTickRead_ && tick != currentTick_ ) )
            {
                // $$$$ JSR 2011-04-29: On envoie un EndTick avant le BeginTick si on était pendant un tick.
                if( insideFrame_ )
                    composite_.EndTick();
                composite_.BeginTick();
                hasFirstTick_ = true;
            }
            currentTick_ = tick;
            insideFrame_ = true;
        }
        else if( wrapper.message().has_control_end_tick() )
        {
            composite_.EndTick();
            insideFrame_ = false;
        }
        else
        {
            // $$$$ JSR 2011-04-29: On envoie un BeginTick si on n'en a pas encore eu.
            if( !hasFirstTick_ )
            {
                currentTick_ = firstTickRead_;
                composite_.BeginTick();
                hasFirstTick_ = true;
                insideFrame_ = true;
            }
            composite_.Receive( wrapper );
        }
    }
    catch( std::exception& )
    {
        // $$$$ AGE 2007-09-25:  ?
    }
}

// -----------------------------------------------------------------------------
// Name: Task::Commit
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void Task::Commit()
{
    if( result_ )
        result_->Commit( firstTick_ - firstTickRead_, firstTick_ );
}

// -----------------------------------------------------------------------------
// Name: Task::Process
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void Task::Process( dispatcher::MessageLoader_ABC& loader )
{
    if( firstTick_ > lastTick_ )
        return;
    firstTickRead_ = loader.FindKeyFrame( firstTick_ );
    const unsigned int ticks = std::min( lastTick_, loader.GetTickNumber() - 1 ) + 1;
    loader.LoadKeyFrame( firstTickRead_, *this );
    for( unsigned int i = firstTickRead_; i < ticks - 1; ++i )
        loader.LoadFrame( i, *this );
    loader.LoadFrame( ticks - 1, *this, boost::bind( &Task::Commit, shared_from_this() ) );
}
