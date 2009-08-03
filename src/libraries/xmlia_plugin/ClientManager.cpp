// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "ClientManager.h"
#include "dispatcher/ClientPublisher_ABC.h"
#include <xeumeuleu/xml.h>
#include "dispatcher/Profile_ABC.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "LdapClient.h"

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: ClientManager constructor
// Created: SLG 2008-06-09
// -----------------------------------------------------------------------------
ClientManager::ClientManager( xml::xistream& xis, LdapClient& ldap )
: ldap_( ldap )
, clientPublisher_( 0 )
, clientProfile_( 0 )
, isAuthorized_ ( false )
{
    xis >> xml::start( "profiles" )
        >> list( "profile", *this, &ClientManager::ReadProfile )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ClientManager destructor
// Created: SLG 2008-06-09
// -----------------------------------------------------------------------------
ClientManager::~ClientManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientManager::NotifyClientAuthenticated
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
void ClientManager::NotifyClient( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile )
{
  clientPublisher_ = &client;
  clientProfile_ = &profile;
	try
    {
        std::string message; 
        ldap_.LdapConnection();
        isAuthorized_ = ldap_.ReadLdapContent( profile.GetName() );
        ASN1T_MsgsPluginToClient msgToClient;
        msgToClient.msg.t = T_MsgsPluginToClient_msg_plugin_text_message;
        ASN1T_MsgPluginTextMessage asnMessage;
        msgToClient.msg.u.plugin_text_message = &asnMessage;
       
        if (isAuthorized_)
        {
          message = "<infoConnection>Authorization accepted from ldap</infoConnection>";
          MT_LOG_ERROR_MSG( "Ldap good connection" );
        }
        else
        {
          message = "<infoConnection>Authorization refused from ldap</infoConnection>";
          MT_LOG_ERROR_MSG( "Ldap error connection" );
        }
          
        std::string command = "xmlia '" + message + "'";
        asnMessage.message = command.c_str();
        asnMessage.destinataire = "supervisor";

        clientPublisher_->Send( msgToClient ); 
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Ldap error : " << e.what() );
    }  
}

// -----------------------------------------------------------------------------
// Name: ClientManager::NotifyClientLeft
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
void ClientManager::NotifyClientLeft( dispatcher::ClientPublisher_ABC& client )
{
  clientPublisher_ = 0;
  clientProfile_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ClientManager::GetClientProfile
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
dispatcher::Profile_ABC* ClientManager::GetClientProfile() const
{
  return clientProfile_;
}


// -----------------------------------------------------------------------------
// Name: ClientManager::GetClientPublisher
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
dispatcher::ClientPublisher_ABC* ClientManager::GetClientPublisher() const
{
  return clientPublisher_;
}

// -----------------------------------------------------------------------------
// Name: ClientManager::GetClientPublisher
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
bool ClientManager::HasNoClient()
{
    return clientProfile_ == 0;
}
// -----------------------------------------------------------------------------
// Name: ClientManager::ReadProfile
// Created: SLG 2009-02-21
// -----------------------------------------------------------------------------
void ClientManager::ReadProfile( xml::xistream& xis )
{
    std::string profileName;
    std::vector < std::string > roles; 
    xis >> xml::attribute( "name", profileName )
        >> list( "role", *this, &ClientManager::ReadRole, roles );
    rolesFilter_.insert( std::pair< std::string, std::vector< std::string > > ( profileName, roles) );
}

// -----------------------------------------------------------------------------
// Name: ClientManager::ReadRole
// Created: SLG 2009-02-21
// -----------------------------------------------------------------------------
void ClientManager::ReadRole( xml::xistream& xis, std::vector < std::string >& roles )
{   
    std::string profileRole = xml::value< std::string >( xis );
    roles.push_back( profileRole );
}

// -----------------------------------------------------------------------------
// Name: IsTakenIntoAccount
// Created: SLG 2009-06-09
// -----------------------------------------------------------------------------
bool ClientManager::IsTakenIntoAccount( const std::string messageType )
{   
  if ( isAuthorized_ )
  {
    rolesMap::iterator itFind = rolesFilter_.find( clientProfile_->GetName() );
    if (itFind!= rolesFilter_.end() )
    {
        std::vector< std::string >& roles = itFind->second;
    
        for( std::vector< std::string >::iterator it = roles.begin(); it != roles.end(); it++ )
        {
            if ( *it == messageType )
                return true;
        }
        return false;
    }
    return false;  
  }
  else 
    return false;
}