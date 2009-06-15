// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "Publisher.h"
//#include "ResponseHandler_ABC.h"
#include "SerializationTools.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include <xeumeuleu/xml.h>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <boost/asio.hpp>
#include "TCP_Client.h"

using namespace plugins::xmlia;
using boost::asio::ip::tcp;


// -----------------------------------------------------------------------------
// Name: Publisher constructor
// Created: SBO 2008-04-02
// -----------------------------------------------------------------------------
Publisher::Publisher( xml::xistream& xis )
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
   // Récupération des données a envoyer (getReports)
   //Envoie des données au webService
    try
    {
        boost::asio::io_service io_service;
        TCP_Client client(io_service, "sending");
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
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
    //Récupération des ordres venant du webService 
}