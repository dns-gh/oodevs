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

#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;


using boost::asio::ip::tcp;
using namespace plugins::xmlia;

namespace
{
  std::string CurrentTime()
  {
    std::string timeTemp = bpt::to_iso_extended_string( bpt::second_clock::local_time() );
    unsigned int separator = timeTemp.find( 'T' );
    std::string hour = timeTemp.substr( separator+1 );
    std::string date = timeTemp.substr ( 0, separator );
    unsigned int indexFirstTiret = timeTemp.find( '-' );
    std::string month = date.substr( indexFirstTiret+1, 2 ); 
    std::string day = timeTemp.substr( 8, 2 );
    std::string year = timeTemp.substr( 0, indexFirstTiret );
    return day+'/' + month + '/' + year + "%20" + hour;
  }
}

// -----------------------------------------------------------------------------
// Name: Publisher constructor
// Created: SBO 2008-04-02
// -----------------------------------------------------------------------------
Publisher::Publisher( xml::xistream& xis )
{
  lastRequestTime_ =CurrentTime();
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
        std::string webServicePath = "/ServiceXmlIa/xmlia/type/create";
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
        //std::string webServicePath = "/ServiceXmlIa/xmlias/type?fromDate=17/06/2009%2013:54:32"; // + lastRequestTime_;
        std::string webServicePath = "/ServiceXmlIa/xmlias/type?fromDate="+lastRequestTime_;
        std::string webServiceHost = "swiwas01.chb.fr.ibm.com";
        TCP_Client client(io_service, "reception", webServicePath, webServiceHost );
        io_service.run();
        std::string content = client.GetContent();
        lastRequestTime_ = CurrentTime();
        if( content == "")//@Hack
          return  content;
        unsigned int indexDebut = content.find("<html");
        unsigned int indexFin = content.find("</html>")+7;
        content = content.substr( indexDebut, indexFin - indexDebut );
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
        int indexDebut = -1;
        int indexFin = -1;
        indexDebut = content.find("<mpia:MPIA_Message");
        indexFin = content.find("</mpia:MPIA_Message>")+20;
        if ( indexDebut < 0 || indexFin < 0)
          return content = "";
        content = content.substr( indexDebut, indexFin - indexDebut );

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
    std::string webServicePath = "/ServiceXmlIa/xmlia/type/create";
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
}

// -----------------------------------------------------------------------------
// Name: Publisher::PushReports
// Created: SBO 2008-05-26
// -----------------------------------------------------------------------------
void Publisher::PushReport( const std::string& xmliaMessage, const std::string& strPoe )
{
    try
    {
        std::string webServiceHost = "swiwas01.chb.fr.ibm.com";
        std::string webServicePath = "/ServiceXmlIa/"+strPoe;
        boost::asio::io_service io_service;

        TCP_Client client(io_service, "POST", webServicePath, webServiceHost, xmliaMessage );
        io_service.run();
        std::string content = client.GetContent();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}
