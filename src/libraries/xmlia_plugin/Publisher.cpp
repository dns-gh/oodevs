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
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
Publisher::Publisher( xml::xistream& xis )
{
  xis >> xml::attribute( "webServiceHost", webServiceHost_ )
      >> xml::attribute( "webservicePort", webservicePort_ );
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
// Name: Publisher::GetReports
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
std::string Publisher::GetUrlReports( const std::string& messageType, const std::string& lastRequestTime )
{
    boost::asio::io_service io_service;
    std::string webServicePath = "/ServiceXmlIa/xmlias/" + messageType + "?fromDate="+lastRequestTime;
    TCP_Client client(io_service, "reception", webServicePath, webServiceHost_, webservicePort_ );
    io_service.run();
    std::string content = client.GetContent();
    if( content == "")
        throw std::exception( "content of url reports from  webService is empty" );
    unsigned int indexDebut = content.find("<html");
    unsigned int indexFin = content.find("</html>")+7; 
    content = content.substr( indexDebut, indexFin - indexDebut );

    return content;
}

// -----------------------------------------------------------------------------
// Name: Publisher::GetMessage
// Created: SLG 2009-06-06
// -----------------------------------------------------------------------------
std::string Publisher::GetXmliaMessage( const std::string& url )
{
    
    std::string webServicePath = url.substr( 38/*36*/, url.length()-38 ); 
    boost::asio::io_service io_service;
    
    TCP_Client client(io_service, "reception", webServicePath, webServiceHost_, webservicePort_ );
    io_service.run();
    std::string content = client.GetContent();
    int indexDebut = -1;
    int indexFin = -1;
    indexDebut = content.find("<MPIA_Message");
    indexFin = content.find("</MPIA_Message>")+15;
    if ( indexDebut < 0 || indexFin < 0)
      throw std::exception( "content of xmliaMessage from webService is not conformed" );
    content = content.substr( indexDebut, indexFin - indexDebut );

    return content;

}

// -----------------------------------------------------------------------------
// Name: Publisher::GetUrlId
// Created: SLG 2008-05-26
// -----------------------------------------------------------------------------
std::string Publisher::GetUrlId( const std::string& messageType )
{
    boost::asio::io_service io_service;
    std::string webServicePath = "/ServiceXmlIa/xmlia/"+ messageType +"/create";
    TCP_Client client(io_service, "sending", webServicePath, webServiceHost_, webservicePort_ );
    io_service.run();
    std::string content = client.GetContent();
    if( content == "")
        throw std::exception( "content of url id from webService is empty" );
    return content;
}

// -----------------------------------------------------------------------------
// Name: Publisher::PushReport
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
void Publisher::PushReport( const std::string& xmliaMessage, const std::string& strPoe )
{
    try
    {
        std::string webServicePath = "/ServiceXmlIa/"+strPoe;
        boost::asio::io_service io_service;

        TCP_Client client(io_service, "POST", webServicePath, webServiceHost_, webservicePort_, xmliaMessage );
        io_service.run();
        std::string content = client.GetContent();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}
