// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "LdapClient.h"
#include "MT/MT_Logger/MT_Logger_lib.h"


using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: LdapClient constructor
// Created: SLG 2008-06-09
// -----------------------------------------------------------------------------
LdapClient::LdapClient()
{
    pLdapConnection_ = NULL;
    pMyInfo_ = NULL;
    hostName = PWCHAR( "LDAP://localhost" );
    pDN_ = PWCHAR( "DC=localhost,DC=com" );

}

// -----------------------------------------------------------------------------
// Name: LdapClient destructor
// Created: SLG 2009-06-09
// -----------------------------------------------------------------------------
LdapClient::~LdapClient()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LdapConnection
// Created: SLG 2009-06-09
// -----------------------------------------------------------------------------
void LdapClient::LdapConnection()
{   
    unsigned long successOperation = 0;
    // Initialize a session. LDAP_PORT is the default port, 389.

    pLdapConnection_ = ldap_init( hostName, LDAP_PORT );
    if ( pLdapConnection_ == NULL )
        throw std::exception( "Session initialization failed" );

    // don't forget to set the session of LDAP
  
    // Connect to the server.
    successOperation = ldap_connect( pLdapConnection_, NULL );
    if ( successOperation != LDAP_SUCCESS )
        throw std::exception( "ConnectionFailed" );

    //Bind anonymously 
    successOperation = ldap_bind_s( pLdapConnection_, pDN_, NULL, LDAP_AUTH_NEGOTIATE );
    if(successOperation != LDAP_SUCCESS)
        throw std::exception( "ldap get option failed" );
    
}

// -----------------------------------------------------------------------------
// Name: ReadLdapContent
// Created: SLG 2009-06-09
// -----------------------------------------------------------------------------
void LdapClient::ReadLdapContent()
{   
    unsigned long errorCode = LDAP_SUCCESS;
    PWCHAR pMyFilter = PWCHAR ( "(&(objectCategory=person)(objectClass=user))" );
    PWCHAR pMyAttributes[4];

    pMyAttributes[0] = PWCHAR( "cn" );
    pMyAttributes[1] = PWCHAR( "groupe" );
    pMyAttributes[2] = PWCHAR( "role" );
    pMyAttributes[3] = NULL;

    errorCode = ldap_search_s( pLdapConnection_, pDN_, LDAP_SCOPE_SUBTREE, pMyFilter, pMyAttributes, 0, &pSearchResult_ );    

    if (errorCode != LDAP_SUCCESS)
        throw std::exception( "Ldap searched operation failed" );
    
    unsigned long numberOfEntries = GetNumberOfEntries();
    

    
    LdapDeconnection();
}

// -----------------------------------------------------------------------------
// Name: SendContentToLdap
// Created: SLG 2009-06-09
// -----------------------------------------------------------------------------
void LdapClient::SendContentToLdap()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: GetNumberOfEntries
// Created: SLG 2009-06-09
// -----------------------------------------------------------------------------
unsigned long LdapClient::GetNumberOfEntries()
{
    unsigned long numberOfEntries;

    numberOfEntries = ldap_count_entries( pLdapConnection_, pSearchResult_ );
    if(numberOfEntries == NULL)
        throw std::exception( "Ldap is empty" );
        
    return numberOfEntries;
}


// -----------------------------------------------------------------------------
// Name: LdapDeconnection
// Created: SLG 2009-06-09
// -----------------------------------------------------------------------------
void LdapClient::LdapDeconnection()
{   
    ldap_unbind( pLdapConnection_ );
    ldap_msgfree( pSearchResult_ );
}


// -----------------------------------------------------------------------------
// Name: IsTakenIntoAccount
// Created: SLG 2009-06-09
// -----------------------------------------------------------------------------
bool LdapClient::IsTakenIntoAccount( const std::string messageType )
{
    for( std::vector< std::string >::iterator it = ldapContent_.begin(); it != ldapContent_.end(); it++ )
    {
        if ( *it == messageType )
            return true;
    }
    return false;
}