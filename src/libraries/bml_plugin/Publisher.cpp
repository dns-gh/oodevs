// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "Publisher.h"
#include "soapDCSOperationsSoapBindingProxy.h"
#include "DCSOperationsSoapBinding.nsmap"
#include "ResponseHandler_ABC.h"
#include "SerializationTools.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include <xeumeuleu/xml.h>
#include <boost/lexical_cast.hpp>
#include <fstream>

using namespace bml;

namespace
{
    std::string ReadProxyHost( xml::xistream& xis )
    {
        const std::string proxy = xml::attribute< std::string >( xis, "proxy", "" );
        return proxy.substr( 0, proxy.find_first_of( ':' ) );
    }

    unsigned short ReadProxyPort( xml::xistream& xis )
    {
        const std::string proxy = xml::attribute< std::string >( xis, "proxy", "" );
        if( proxy.empty() )
            return 0;
        return boost::lexical_cast< unsigned short >( proxy.substr( proxy.find_first_of( ':' ) + 1 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Publisher constructor
// Created: SBO 2008-04-02
// -----------------------------------------------------------------------------
Publisher::Publisher( xml::xistream& xis )
    : endpoint_ ( xml::attribute< std::string >( xis, "endpoint" ) )
    , proxyHost_( ReadProxyHost( xis ) )
    , proxyPort_( ReadProxyPort( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Publisher destructor
// Created: SBO 2008-04-02
// -----------------------------------------------------------------------------
Publisher::~Publisher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Publisher::PushReports
// Created: SBO 2008-05-26
// -----------------------------------------------------------------------------
void Publisher::PushReports()
{
    if( reports_.get() != 0 )
    {
        try
        {
            *reports_ << xml::end();
            DCSOperationsSoapBindingProxy service;
            ConfigureService( service );
            std::string response;
            service.pushReport( reports_->str(), response ); // $$$$ SBO 2008-05-16: make it non-blocking
            if( service.error )
                throw std::exception( service.soap_fault_string() );
        }
        catch( std::exception& e )
        {
			MT_LOG_ERROR_MSG( "BML error pushing reports: " << e.what() );
        }
        reports_.release();
    }
}

// -----------------------------------------------------------------------------
// Name: Publisher::CreateReport
// Created: SBO 2008-05-26
// -----------------------------------------------------------------------------
xml::xostream& Publisher::CreateReport()
{
    if( reports_.get() == 0 )
    {
        reports_.reset( new xml::xostringstream() );
        *reports_ << xml::start( "ReportPush" )
                    << Namespaces();
    }
    return *new xml::xosubstream( *reports_ );
}

// -----------------------------------------------------------------------------
// Name: Publisher::PullOrder
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
void Publisher::PullOrder( const std::string& message, ResponseHandler_ABC& handler )
{
    try
    {
        DCSOperationsSoapBindingProxy service;
        ConfigureService( service );
        std::string response;
        service.pullOrder( message, response ); // $$$$ SBO 2008-05-16: make it non-blocking
        if( service.error )
            throw std::exception( service.soap_fault_string() );
	    handler.Handle( response );
    }
    catch( std::exception& e )
    {
		MT_LOG_ERROR_MSG( "BML error pulling orders: " << e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: Publisher::ConfigureService
// Created: SBO 2008-05-20
// -----------------------------------------------------------------------------
void Publisher::ConfigureService( DCSOperationsSoapBindingProxy& service ) const
{
    service.soap_endpoint = endpoint_.c_str();
    if( !proxyHost_.empty() )
    {
        service.proxy_host = proxyHost_.c_str();
        service.proxy_port = proxyPort_;
    }
}
