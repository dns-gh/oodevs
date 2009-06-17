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


using boost::asio::ip::tcp;
using namespace plugins::xmlia;

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
    try
    {
       //Demande requête pour envoie des données au webService
        boost::asio::io_service io_service;
        std::string webServicePath = "/ServiceXmlIa/xmlias/type";
        std::string webServiceHost = "swiwas01.chb.fr.ibm.com";
        TCP_Client client(io_service, "sending", webServicePath, webServiceHost );
        io_service.run();
        std::string content = client.GetContent();
        xml::xistringstream streamContent( content );
        
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
std::string Publisher::GetUrlReports()
{
    try
    {
        //Demande requête pour envoie des données au webService
        boost::asio::io_service io_service;
        std::string webServicePath = "/ServiceXmlIa/xmlias/type";
        std::string webServiceHost = "swiwas01.chb.fr.ibm.com";
        TCP_Client client(io_service, "reception", webServicePath, webServiceHost );
        io_service.run();
        std::string content = client.GetContent();
        return content;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    
}

// -----------------------------------------------------------------------------
// Name: Publisher::GetMessage
// Created: SLG 2009-06-06
// -----------------------------------------------------------------------------
std::string Publisher::GetXmliaMessage( const std::string& url )
{
    try
    {
        std::string webServiceHost = url.substr( 7, 23);
        std::string webServicePath = url.substr( 36, 35); 
        boost::asio::io_service io_service;
        
        TCP_Client client(io_service, "reception", webServicePath, webServiceHost );
        io_service.run();
        std::string content = client.GetContent();
        return content;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

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
/*
// -----------------------------------------------------------------------------
// Name: Publisher::GetUrlId
// Created: SLG 2008-05-26
// -----------------------------------------------------------------------------
std::string Publisher::GetUrlId()
{
    try
    {
        //Demande requête pour envoie des données au webService
        boost::asio::io_service io_service;
        std::string webServicePath = "/ServiceXmlIa/xmlias/type";
        std::string webServiceHost = "swiwas01.chb.fr.ibm.com";
        TCP_Client client(io_service, "sending", webServicePath, webServiceHost );
        io_service.run();
        std::string content = client.GetContent();
        return content;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}*/
/*
// -----------------------------------------------------------------------------
// Name: Publisher::PushReports
// Created: SBO 2008-05-26
// -----------------------------------------------------------------------------
void Publisher::PushReports( const std::string& xmliaMessage, const std::string& urlId )
{
    try
    {
        std::string webServiceHost = url.substr( 7, 23);
        std::string webServicePath = url.substr( 36, 35); 
        boost::asio::io_service io_service;

        TCP_Client client(io_service, "sending", webServicePath, webServiceHost );
        io_service.run();
        std::string content = client.GetContent();
        return content;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}
*/