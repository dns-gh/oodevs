// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_test_app_pch.h"
#include "App.h"
#include "ResponseHandler.h"
#include "PBTools.h"

#include "protocol/LauncherSenders.h"
#pragma warning( push )
#pragma warning( disable: 4127 4512 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <iostream>
#include <xeumeuleu/xml.h>

namespace po = boost::program_options;

namespace
{
void UpdateClient(frontend::LauncherClient& client)
{
    bool interrupted = false;
    try
    {
        while( !interrupted )
        {
            client.Update();
            try
            {
                boost::this_thread::interruption_point();
            }
            catch( const boost::thread_interrupted& /*e*/ )
            {
                interrupted = true;
            }
            boost::this_thread::yield();
        }
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
    }
    catch( ... )
    {
        std::cerr << "unknown exception" << std::endl;
    }
}
const std::string DEFAULT_MESSAGE="<message type='AdminToLauncher'>"
                        "<field name='context' value='0'/>"
                        "<field name='message'>"
                            "<message type='Content'>"
                                "<field name='exercise_list_request'>"
                                    "<message type='ExerciseListRequest'>"
                                    "</message>"
                                "</field>"
                            "</message>"
                        "</field>"
                    "</message>";
}

namespace launcher_test_app
{
// -----------------------------------------------------------------------------
// Name: App constructor
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
App::App( int argc, char* argv[] )
    : host_( "localhost" )
    , port_( 33000 )
    , app( argc, argv )
    , connectionHandler_( mutex_, cond_ )
    , responseHandler_( new ResponseHandler(mutex_, cond_ ) )
    , client_( controllers_.controller_ )
{
    po::options_description options;
    po::variables_map values;
    std::string commandFile;
    options.add_options()( "host", po::value< std::string >( &host_ ), "launcher host" );
    options.add_options()( "port", po::value< unsigned int >( &port_ ), "launcher port" );
    options.add_options()( "file", po::value< std::string >( &commandFile ), "command file" );
    po::store( po::command_line_parser( argc, argv ).options( options ).run(), values );
    if( values.find( "file" ) != values.end() )
    {
        commandFile = values["file"].as<std::string>();
    }
    if( commandFile.size() == 0 )
    {
        xml::xistringstream xis(DEFAULT_MESSAGE);
        xis >> xml::start( "message" );
        messages_.push_back( boost::shared_ptr< google::protobuf::Message > (PBTools::FromXML(xis) ) );
        xis >> xml::end;
    }
    else
    {
        xml::xifstream xis(commandFile);
        xis >> xml::start("messages")
                >> xml::list("message", *this, &App::ReadMessage)
            >> xml::end;
    }
    po::notify( values );
}
// -----------------------------------------------------------------------------
// Name: App destructor
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
App::~App()
{
    updateThread_->interrupt();
    updateThread_->join();
    updateThread_.reset();
}
// -----------------------------------------------------------------------------
// Name: App::Connect
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void App::Connect()
{
    client_.Connect( host_, port_, connectionHandler_);
    updateThread_.reset( new boost::thread( &UpdateClient, boost::ref( client_ ) ) );
    Wait();
    client_.Register( responseHandler_ );
}
// -----------------------------------------------------------------------------
// Name: App::Wait
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void App::Wait(int messages)
{
    while( messages != 0 )
    {
        std::string error;
        {
            boost::mutex::scoped_lock lock( mutex_ );
            cond_.wait( lock );
            error = connectionHandler_.Error();
        }
        if( !connectionHandler_.Connected() )
        {
            updateThread_->interrupt();
            updateThread_->join();
            throw std::runtime_error( "not connected : " + error );
        }
        --messages;
    }
}

// -----------------------------------------------------------------------------
// Name: App::Send
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void App::Send(google::protobuf::Message* msg)
{
    if( dynamic_cast<sword::AdminToLauncher*>(msg) )
        Send<sword::AdminToLauncher>(*dynamic_cast<sword::AdminToLauncher*>(msg));
}

// -----------------------------------------------------------------------------
// Name: App::ReadMessage
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void App::ReadMessage(xml::xistream& xis)
{
    messages_.push_back( boost::shared_ptr< google::protobuf::Message > (PBTools::FromXML(xis) ) );
}

// -----------------------------------------------------------------------------
// Name: App::ProcessMessages
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void App::ProcessMessages()
{
    while( !messages_.empty() )
    {
        boost::shared_ptr< google::protobuf::Message > msg = messages_.front();
        messages_.pop_front();
        Send( msg.get() );
        Wait();
    }
}
}
