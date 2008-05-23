// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "Publisher.h"
#include "soapDCSOperationsSoapBindingProxy.h"
#include "DCSOperationsSoapBinding.nsmap"
#include "ResponseHandler_ABC.h"
#include <xeumeuleu/xml.h>
#include <boost/lexical_cast.hpp>

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
// Name: Publisher::PushReport
// Created: SBO 2008-05-15
// -----------------------------------------------------------------------------
void Publisher::PushReport( const std::string& message )
{
    DCSOperationsSoapBindingProxy service;
    ConfigureService( service );
    std::string response;
    service.pushReport( message, response ); // $$$$ SBO 2008-05-16: make it non-blocking
    if( service.error )
        throw std::exception( service.soap_fault_string() );
}

// -----------------------------------------------------------------------------
// Name: Publisher::PullOrder
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
void Publisher::PullOrder( const std::string& message, ResponseHandler_ABC& handler )
{
    DCSOperationsSoapBindingProxy service;
    ConfigureService( service );
    std::string response;
    service.pullOrder( message, response ); // $$$$ SBO 2008-05-16: make it non-blocking
    if( service.error )
        throw std::exception( service.soap_fault_string() );
    handler.Handle( response );
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
