// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "NetworkExerciseLister.h"
#include "Config.h"
#include "ExerciseList.h"
#include "frontend/commands.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/tokenizer.hpp>

using namespace boost::asio;

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister constructor
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
NetworkExerciseLister::NetworkExerciseLister( const Config& config, const std::string& subDir )
    : config_( config )
    , subDir_( subDir )
    , socket_( 0 )
    , thread_( 0 )
{
    try
    {
        socket_.reset( new boost::asio::ip::udp::socket( network_, ip::udp::endpoint( ip::udp::v4(), config.GetListClientPort() ) ) );
        thread_.reset( new boost::thread( boost::bind( &NetworkExerciseLister::RunNetwork, this ) ) );
        socket_->async_receive( buffer( answer_, 1024 ),
				                boost::bind( &NetworkExerciseLister::OnReceive, this, 
								              placeholders::error,
                                              placeholders::bytes_transferred ) );
    }
    catch( ... )
    {
        // $$$$ SBO 2008-10-31: throw exception to warn user
    }
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister destructor
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
NetworkExerciseLister::~NetworkExerciseLister()
{
    network_.stop();
    if( thread_.get() )
        thread_->join();
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister::AddList
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void NetworkExerciseLister::AddList( ExerciseList* list )
{
    lists_.push_back( list );
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister::DownloadExercises
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
void NetworkExerciseLister::DownloadExercises( const std::string& host, unsigned int port )
{
    Send( host, port );
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister::RunNetwork
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void NetworkExerciseLister::RunNetwork()
{
    network_.run();
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister::OnReceive
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
void NetworkExerciseLister::OnReceive( const boost::system::error_code& error, size_t bytes_received )
{
    ClearListeners();
    if( !error )
    {
        std::string exercises( answer_, bytes_received );
        typedef boost::tokenizer< boost::char_separator< char > > T_Tokenizer;
        boost::char_separator<char> separator("/");
        const T_Tokenizer tok( exercises, separator );
        for( T_Tokenizer::const_iterator it = tok.begin(); it != tok.end(); ++it )
        {
            std::string token = *it;
            int dotPosition = token.find( ":" );
            std::string scenario = token.substr( 0, dotPosition );
            std::string port = token.substr( dotPosition + 1, token.length() - dotPosition -1 );
            UpdateListeners( scenario, port );
        }
    }
    else
    {
        for( CIT_Lists it = lists_.begin(); it != lists_.end(); ++it )
            (*it)->Update();
    }
    socket_->async_receive( buffer( answer_, 1024 ),
						    boost::bind( &NetworkExerciseLister::OnReceive, this, 
							              placeholders::error,
									      placeholders::bytes_transferred ) );
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister::Send
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
void NetworkExerciseLister::Send( const std::string& host, unsigned int port )
{
    try
    {
        if( !socket_.get() )
            return;
        ip::udp::resolver resolver( network_ );
        ip::udp::resolver::query query( host, boost::lexical_cast< std::string >( port ) );
        ip::udp::resolver::iterator iterator = resolver.resolve(query);
        ip::udp::endpoint endpoint = *iterator;
        socket_->async_send_to( buffer( "exercises" ), endpoint, boost::bind( &NetworkExerciseLister::OnSendExercisesRequest, this, placeholders::error ) );
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister::OnSendExercisesRequest
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
void NetworkExerciseLister::OnSendExercisesRequest( const boost::system::error_code& /*error*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister::ClearListeners
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void NetworkExerciseLister::ClearListeners()
{
    exercises_.clear();
    for( CIT_Lists it = lists_.begin(); it != lists_.end(); ++it )
        (*it)->Clear();
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister::UpdateListeners
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void NetworkExerciseLister::UpdateListeners( const std::string& scenario, const std::string& port )
{
    exercises_[ scenario ] = boost::lexical_cast< unsigned short >( port );
    for( CIT_Lists it = lists_.begin(); it != lists_.end(); ++it )
        (*it)->Update();
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister::ListExercises
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void NetworkExerciseLister::ListExercises( QStringList& list ) const
{
    if( exercises_.empty() )
        list = frontend::commands::ListExercises( config_, subDir_ );
    else
    {
        list.clear();
        for ( CIT_PortDictionary it = exercises_.begin(); it != exercises_.end(); ++it )
            list.append( it->first.c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister::GetPort
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
unsigned short NetworkExerciseLister::GetPort( const QString& exercise ) const
{
    std::string exerciseStr = exercise.ascii();
    CIT_PortDictionary it = exercises_.find( exerciseStr );
    if( it != exercises_.end() )
        return it->second;
    return 0;
}
