// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "CreateSession.h"
#include "CommandLineTools.h"
#include "tools/GeneralConfig.h"
#pragma warning( push, 0 )
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: CreateSession constructor
// Created: RPD 2011-03-30
// -----------------------------------------------------------------------------
CreateSession::CreateSession(  const tools::Path& filePath )
    : setter_ ( new ConfigurationManipulator( filePath ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CreateSession constructor
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
CreateSession::CreateSession( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session )
    : setter_ ( new ConfigurationManipulator( config, exercise, session ) )
    , session_( session )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CreateSession destructor
// Created: SBO 2008-02-25
// -----------------------------------------------------------------------------
CreateSession::~CreateSession()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CreateSession::Commit
// Created: MCO 2010-09-30
// -----------------------------------------------------------------------------
void CreateSession::Commit()
{
    setter_->Commit();
}

// -----------------------------------------------------------------------------
// Name: CreateSession::SetDefaultValues
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void CreateSession::SetDefaultValues()
{
    {
        try
        {
            setter_->SetValue( "session/meta/date", session_ );
        }
        catch( ... )
        {
            setter_->SetValue( "session/meta/date", bpt::to_iso_string( bpt::second_clock::local_time() ) );
        }
        setter_->SetValue( "session/meta/name"   , session_ );
        setter_->SetValue( "session/meta/comment", "" );
    }
    {
        setter_->SetValue( "session/config/simulation/checkpoint/@frequency"   , "100000h" );
        setter_->SetValue( "session/config/simulation/checkpoint/@keep"        , 1 );
        setter_->SetValue( "session/config/simulation/checkpoint/@usecrc"      , true );
        setter_->SetValue( "session/config/simulation/debug/@decisional"       , false );
        setter_->SetValue( "session/config/simulation/debug/@pathfind"         , false );
        setter_->SetValue( "session/config/simulation/dispatcher/@embedded"    , true );
        setter_->SetValue( "session/config/simulation/network/@port"           , "localhost:" +  boost::lexical_cast< std::string >( GetPort( 1, SIMULATION_PORT ) ) );
        setter_->SetValue( "session/config/simulation/orbat/@checkcomposition" , false );
        setter_->SetValue( "session/config/simulation/profiling/@enabled"      , false );
        setter_->SetValue( "session/config/simulation/time/@step"              , 10 );
        setter_->SetValue( "session/config/simulation/time/@factor"            , 10 );

        //Réglage des paramètres du GC
        setter_->SetValue( "session/config/simulation/GarbageCollector/@setpause"  , 100 );
        setter_->SetValue( "session/config/simulation/GarbageCollector/@setstepmul", 200 );
        //
        setter_->SetValue( "session/config/simulation/pathfinder/@threads"         , 4 );
    }
    {
        setter_->SetValue( "session/config/dispatcher/network/@client", "localhost:" +  // $$$$ AGE 2007-10-09:
                            boost::lexical_cast< std::string >( GetPort( 1, SIMULATION_PORT ) ) );
        setter_->SetValue( "session/config/dispatcher/network/@server", "0.0.0.0:" +  boost::lexical_cast< std::string >( GetPort( 1, DISPATCHER_PORT ) ) );
        setter_->SetValue( "session/config/dispatcher/plugins/recorder/@fragmentfreq", 200 );
        setter_->SetValue( "session/config/dispatcher/plugins/recorder/@keyframesfreq", 100 );
    }
    {
        setter_->SetValue( "session/config/gaming/network/@server", "localhost:" +  // $$$$ AGE 2007-10-09:
                                    boost::lexical_cast< std::string >( GetPort( 1, DISPATCHER_PORT ) ) );
    }
    {
        setter_->SetValue( "session/config/timeline/@url", "localhost:" +
                                    boost::lexical_cast< std::string >( GetPort( 1, frontend::TIMELINE_PORT ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CreateSession::RemoveOption
// Created: RPD 2011-03-29
// -----------------------------------------------------------------------------
bool CreateSession::RemoveOption( const std::string& path )
{
    return setter_->RemoveNode( path );
}

// -----------------------------------------------------------------------------
// Name: CreateSession::HasOption
// Created: RPD 2011-03-30
// -----------------------------------------------------------------------------
bool CreateSession::HasOption( const std::string& path )
{
    return setter_->HasNode( path );
}
