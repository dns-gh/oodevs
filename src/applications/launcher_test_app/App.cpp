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
#include "shield/proto/AdminToLauncher.pb.h"

#include <iostream>
#pragma warning( push )
#pragma warning( disable: 4127 4512 )
#include <boost/program_options.hpp>
#pragma warning( pop )
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
    , client_( controllers_.controller_ )
{
    po::options_description options;
    po::variables_map values;
    std::string commandFile;
    std::string outputFile;
    options.add_options()
            ( "host", po::value< std::string >( &host_ ), "launcher host" )
            ( "port", po::value< unsigned int >( &port_ ), "launcher port" )
            ( "file", po::value< std::string >( &commandFile ), "command file" )
            ( "output", po::value< std::string >( &outputFile ), "output file" );
    po::store( po::command_line_parser( argc, argv ).options( options ).run(), values );
    po::notify( values );
    if( outputFile.length() != 0)
    {
        outputFile_.open( outputFile.c_str() );
        responseHandler_.reset( new ResponseHandler( outputFile_, mutex_, cond_ ) );
    }
    else
        responseHandler_.reset( new ResponseHandler( std::cout, mutex_, cond_ ) );
    if( commandFile.length() == 0 )
    {
        std::cout << options << std::endl;

        xml::xistringstream xis(DEFAULT_MESSAGE);
        xis >> xml::start( "message" );
        ReadMessage( xis );
        xis >> xml::end;
    }
    else
    {
        xml::xifstream xis(commandFile);
        xis >> xml::start("messages")
                >> xml::list("message", *this, &App::ReadMessage)
            >> xml::end;
    }
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
    else if( dynamic_cast<MsgsAdminToLauncher::MsgAdminToLauncher*>(msg) )
        Send<MsgsAdminToLauncher::MsgAdminToLauncher>(*dynamic_cast<MsgsAdminToLauncher::MsgAdminToLauncher*>(msg));
}

namespace
{
    google::protobuf::Message* MessageFactory(const std::string& type)
    {
        if( type == "AdminToLauncher" )
            return new sword::AdminToLauncher();
        if( type == "MsgAdminToLauncher" )
            return new MsgsAdminToLauncher::MsgAdminToLauncher();
        return 0;
    }
}
// -----------------------------------------------------------------------------
// Name: App::ReadMessage
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void App::ReadMessage(xml::xistream& xis)
{
    std::string messageType;
    xis >> xml::attribute("type", messageType);
    google::protobuf::Message* retval = MessageFactory(messageType);
    if(retval != 0)
    {
        PBTools::FromXML(xis, *retval);
        messages_.push_back( boost::shared_ptr< google::protobuf::Message > ( retval ) );
    }
    else
        std::cerr << "No factory for message type " << messageType << std::endl;
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
