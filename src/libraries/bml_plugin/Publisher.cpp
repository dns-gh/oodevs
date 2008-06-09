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
    try
    {
        std::string toSend = GetReports();
        if( toSend.empty() )
            return;

        DCSOperationsSoapBindingProxy service;
        ConfigureService( service );
        std::string response;
        service.pushReport( toSend, response ); // $$$$ SBO 2008-05-16: make it non-blocking
        if( service.error )
            throw std::exception( service.soap_fault_string() );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "BML error pushing reports: " << e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: Publisher::GetReports
// Created: AGE 2008-06-06
// -----------------------------------------------------------------------------
std::string Publisher::GetReports()
{
    boost::recursive_mutex::scoped_lock locker( mutex_ );
    std::string result;
    if( reports_.get() != 0 )
    {
        *reports_ << xml::end();
        result = reports_->str();
        reports_.release();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: Publisher::CreateReport
// Created: SBO 2008-05-26
// -----------------------------------------------------------------------------
xml::xostream& Publisher::CreateReport()
{
    boost::recursive_mutex::scoped_lock locker( mutex_ );
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
