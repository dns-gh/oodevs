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

#pragma warning( disable: 4127 4244 4245 4511 4512 )
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/lexical_cast.hpp>

namespace bfs = boost::filesystem;

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: CreateSession constructor
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
CreateSession::CreateSession( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session )
    : setter_( new ConfigurationManipulator( config, exercise, session ) )
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
    setter_->Commit();
}

// -----------------------------------------------------------------------------
// Name: CreateSession::SetDefaultValues
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void CreateSession::SetDefaultValues()
{
    {
        setter_->SetValue( "session/meta/date"     , session_ );
        setter_->SetValue( "session/meta/name"     , "" );
        setter_->SetValue( "session/meta/comment"  , "" );
    }
    {
        setter_->SetValue( "session/config/simulation/checkpoint/@frequency"       , "100000h" );
        setter_->SetValue( "session/config/simulation/checkpoint/@keep"            , 1 );
        setter_->SetValue( "session/config/simulation/checkpoint/@usecrc"          , true );
        setter_->SetValue( "session/config/simulation/debug/@decisional"           , false );
        setter_->SetValue( "session/config/simulation/debug/@pathfind"             , false );
        setter_->SetValue( "session/config/simulation/debug/@diadebugger"          , false );
        setter_->SetValue( "session/config/simulation/debug/@diadebuggerport"      , 15000 );
        setter_->SetValue( "session/config/simulation/debug/@networklogger"        , false );
        setter_->SetValue( "session/config/simulation/debug/@networkloggerport"    , 20000 );
        setter_->SetValue( "session/config/simulation/decisional/@useonlybinaries" , true );
        setter_->SetValue( "session/config/simulation/dispatcher/@embedded"        , true );
        setter_->SetValue( "session/config/simulation/network/@port"               , SimulationPort( 1 ) );
        setter_->SetValue( "session/config/simulation/orbat/@checkcomposition"     , false );
        setter_->SetValue( "session/config/simulation/profiling/@enabled"          , false );
        setter_->SetValue( "session/config/simulation/time/@step"                  , 10 );
        setter_->SetValue( "session/config/simulation/time/@factor"                , 10 );
        setter_->SetValue( "session/config/simulation/pathfinder/@threads"         , 1 );
        setter_->SetValue( "session/config/simulation/hla/@enabled"                , false );
        setter_->SetValue( "session/config/simulation/hla/@federation"             , "MyFederation" );
        setter_->SetValue( "session/config/simulation/hla/@federate"               , "Sword OT Power" );
    }
    {
        setter_->SetValue( "session/config/dispatcher/network/@client", "localhost:" +  // $$$$ AGE 2007-10-09: 
                            boost::lexical_cast< std::string >( SimulationPort( 1 ) ) );
        setter_->SetValue( "session/config/dispatcher/network/@server", DispatcherPort( 1 ) );
        setter_->SetValue( "session/config/dispatcher/plugins/recorder", "" ); // $$$$ AGE 2008-02-22: 
    }
    {
        setter_->SetValue( "session/config/gaming/network/@server", "localhost:" +  // $$$$ AGE 2007-10-09: 
                                    boost::lexical_cast< std::string >( DispatcherPort( 1 ) ) );
    }
}
