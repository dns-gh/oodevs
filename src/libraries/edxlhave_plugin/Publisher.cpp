// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "edxlhave_plugin_pch.h"
#include "Publisher.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include <xeumeuleu/xml.hpp>
#include <boost/lexical_cast.hpp>
#include "RestClient.h"
#include "ResponseHandler_ABC.h"
#include "MT/MT_Logger/MT_Logger_lib.h"


#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

using boost::asio::ip::tcp;
using namespace plugins::edxl;

// -----------------------------------------------------------------------------
// Name: Publisher constructor
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
Publisher::Publisher( xml::xistream& xis )
    : log_ ( false )
    , useSsl_ ( false )
{
	std::string type;
    xis >> xml::start( "services" ) >> xml::attribute( "host", host_ ) 
                                    >> xml::optional >> xml::attribute( "log", log_ )
                                    >> xml::optional >> xml::attribute( "type", type )
            >> xml::start( "initialization" ) >> xml::attribute( "serviceURI", getURI_ ) >> xml::end
            >> xml::start( "update" )
                >> xml::attribute( "serviceURI", postURI_ )
                >> xml::attribute( "frequency", frequency_ )
            >> xml::end
        >> xml::end;
    if ( log_ )
        MT_LOG_INFO_MSG( "Edxl-Have service loaded on : " << host_ << std::endl 
                          << " - initialize : " << getURI_ << std::endl
                          << " - update : " << postURI_ )
	if ( type == "https" )
	{
		MT_LOG_INFO_MSG( " - connecting using SSL" )
		useSsl_ = true;
	}
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
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void Publisher::PullSituation( const std::string& message, ResponseHandler_ABC& handler )
{
    try
    {
        std::string result;

        RestClient client( host_, getURI_, useSsl_ );
            
		client.DoGet( message, result );
		if( client.GetStatus() != 200 || result.size() == 0 )
			throw std::exception( "Content of url reports from webService is empty" );

        boost::recursive_mutex::scoped_lock locker( mutex_ );
        handler.Handle( xml::xistringstream( result ) );
        if ( log_ )
            MT_LOG_INFO_MSG( "Pull situation from server succeeded." )
    }
    catch (std::exception& e)
    {
        MT_LOG_WARNING_MSG( "Unable to pull situation: " << e.what() )
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

            if ( log_ )
                MT_LOG_INFO_MSG( message )

            if ( log_ )
                MT_LOG_INFO_MSG( "Push report to server." )
        }
    }
    catch (std::exception& e)
    {
        MT_LOG_WARNING_MSG( "Unable to push report: " << e.what() )
    }
}
