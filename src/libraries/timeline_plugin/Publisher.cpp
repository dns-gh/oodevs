// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "timeline_plugin_pch.h"
#include "Publisher.h"
#include "RestClient.h"
#include "ResponseHandler_ABC.h"
//#include "MT_Tools/MT_Logger.h"
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.hpp>
#include "dispatcher/Logger_ABC.h"

using boost::asio::ip::tcp;
using namespace plugins::timeline;

// -----------------------------------------------------------------------------
// Name: Publisher constructor
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
Publisher::Publisher( xml::xistream& xis, dispatcher::Logger_ABC& logger )
    : log_   ( false )
    , useSsl_( false )
	, logger_( &logger )
{
    xis >> xml::attribute( "host", host_ )
        >> xml::optional >> xml::attribute( "log", log_ )
        >> xml::optional >> xml::attribute( "ssl", useSsl_ )
        >> xml::start( "scenario" )
            >> xml::attribute( "id", scenario_ )
        >> xml::end;
    getURI_ = "/scenario/" + boost::lexical_cast<std::string>( scenario_ );
    postURI_ = "/scenario/" + boost::lexical_cast<std::string>( scenario_ ) + "/update";
    if( log_ )
		this->logger_->LogInfo( "Timeline service loaded on : " + host_ );
    if( useSsl_ )
		this->logger_->LogInfo( " - connecting using SSL" );
}

// -----------------------------------------------------------------------------
// Name: Publisher destructor
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
Publisher::~Publisher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Publisher::PullSituation
// Created: JCR 2011-03-30
// -----------------------------------------------------------------------------
void Publisher::ExecuteGetRequest( const std::string& uri, const std::string& message, ResponseHandler_ABC& handler )
{
    std::string result;
    RestClient  client( host_, uri, useSsl_ );
        
    client.DoGet( message, result );
    if( client.GetStatus() != 200 || result.size() == 0 )
        throw std::exception( "Content of url reports from webService is empty" );
    {
        boost::recursive_mutex::scoped_lock locker( mutex_ );
        xml::xistringstream xis( result );
        handler.Handle( xis );
    }
    if( log_ )
		this->logger_->LogInfo( result );
}

// -----------------------------------------------------------------------------
// Name: Publisher::GetUri
// Created: JCR 2010-09-08
// -----------------------------------------------------------------------------
std::string Publisher::GetUri( const std::string& timestamp )
{
    if( timestamp.empty() )
        return getURI_;
    return getURI_ + "/poll/" + timestamp;
}

// -----------------------------------------------------------------------------
// Name: Publisher::PullSituation
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void Publisher::PullSituation( const std::string& message, const std::string& timestamp, ResponseHandler_ABC& handler )
{
    try
    {
        ExecuteGetRequest( GetUri( timestamp ), message, handler );
        if( log_ )
			this->logger_->LogInfo( "Pull situation from server succeeded." );
    }
    catch (std::exception& e)
    {
		this->logger_->LogWarning( std::string("Unable to pull situation: ") + e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: Publisher::PushReport
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void Publisher::PushReport( const std::string& message )
{
    try
    {
        if( !message.empty() )
        {
            std::string result;

            RestClient client( host_, postURI_, useSsl_ );

            boost::recursive_mutex::scoped_lock locker( mutex_ );
            client.DoPost( message, result );

            if( log_ )
				this->logger_->LogInfo( message );

            if( log_ )
				this->logger_->LogInfo( "Push report to server." );
        }
    }
    catch (std::exception& e)
    {
		this->logger_->LogWarning( std::string("Unable to push report: ") + e.what() );
    }
}

namespace 
{
    class VoidResponseHandler : public ResponseHandler_ABC 
    {
    public:
        VoidResponseHandler() {}
        ~VoidResponseHandler() {}

        void Handle( xml::xistream& /*response*/ )
        {
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Publisher::PushResetScenario
// Created: JCR 2011-03-30
// -----------------------------------------------------------------------------
void Publisher::RestartScenario()
{
    try
    {
        VoidResponseHandler handler;
        const std::string uri( "/initialize?servletid=" + boost::lexical_cast<std::string>( scenario_ ) );
        
        ExecuteGetRequest( uri, "", handler );
        if( log_ )
			this->logger_->LogInfo( "Scenario restarted successfully."  );
    }
    catch (std::exception& e)
    {
		this->logger_->LogWarning( std::string("Unable to restart scenario: ") + e.what() );
    }
}
