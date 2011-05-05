// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "edxlhave_app_pch.h"
#include "Publisher.h"
#include "RestClient.h"
#include "ResponseHandler_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/lexical_cast.hpp>
#pragma warning( push, 0 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

using boost::asio::ip::tcp;
using namespace edxl;

// -----------------------------------------------------------------------------
// Name: Publisher constructor
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
Publisher::Publisher( const boost::program_options::variables_map& options )
: host_( options[ "edxl.host" ].as< std::string >() )
, postURI_( options[ "edxl.update.serviceURI" ].as< std::string >() )
, getURI_( options[ "edxl.initialization.serviceURI" ].as< std::string >() )
, frequency_( options[ "edxl.update.frequency" ].as< std::string >() )
, useSsl_ ( options[ "edxl.ssl" ].as< bool >() )
, log_ ( options[ "log" ].as< bool >() )
{
    if ( log_ )
    {
        std::clog <<  "Edxl-Have service loaded on : " << host_ << std::endl
            << " - initialize : " << getURI_ << std::endl
            << " - update : " << postURI_;
        if ( useSsl_ )
            std::clog << " - connecting using SSL";
        std::clog << std::endl;
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
            std::clog << "Pull situation from server succeeded." << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Unable to pull situation: " << e.what() << std::endl;
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
            {
                std::clog << message << std::endl;
                std::clog << "Push report to server." << std::endl;
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Unable to push report: " << e.what() << std::endl;
    }
}
