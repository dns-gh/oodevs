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
#include <xeumeuleu/xml.h>


using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: LdapClient constructor
// Created: SLG 2008-06-09
// -----------------------------------------------------------------------------
LdapClient::LdapClient( xml::xistream& xis )
: pDN_( 0 )
, pLdapConnection_( 0 )
, pMyInfo_( 0 )
{
    std::string strHostName,strPassword, strUser;
    size_t charConverted;

    xis >> xml::start( "ldap" )
        >> xml::attribute( "hostname", strHostName )
        >> xml::attribute( "password", strPassword )
        >> xml::attribute( "user", strUser )
        >> xml::end;
  
    pHostName_ = new WCHAR[ strHostName.length() ];
    mbstowcs_s(&charConverted, pHostName_,strHostName.length()+1, strHostName.c_str(), strHostName.length()+1 );

    pUser_ = new WCHAR[strUser.length()];
    mbstowcs_s(&charConverted, pUser_,strUser.length()+1, strUser.c_str(), strUser.length()+1 );

    pPassword_ = new WCHAR[strPassword.length()];
    mbstowcs_s(&charConverted, pPassword_,strPassword.length()+1, strPassword.c_str(), strPassword.length()+1 );

    //IdentificationConfigure();
}

// -----------------------------------------------------------------------------
// Name: LdapClient destructor
// Created: SLG 2009-06-09
// -----------------------------------------------------------------------------
LdapClient::~LdapClient()
{
    for( std::vector< PWCHAR* >::iterator it = pAttributesValues_.begin(); it != pAttributesValues_.end(); it++ )
    {
        ldap_value_free( *it );
    }
    delete[] pHostName_;
    delete[] pPassword_;
    delete[] pUser_;
    delete[] pDN_;

}

// -----------------------------------------------------------------------------
// Name: LdapConnection
// Created: SLG 2009-06-09
// -----------------------------------------------------------------------------
void LdapClient::LdapConnection()
{   
    unsigned long successOperation = LDAP_SUCCESS;
    
    pLdapConnection_ = ldap_init( pHostName_, 62389 );
    if ( pLdapConnection_ == NULL )
        throw std::exception( "Session initialization failed" );

    unsigned long lRtn = 0;
    unsigned long version = LDAP_VERSION3;
    lRtn = ldap_set_option( pLdapConnection_, LDAP_OPT_PROTOCOL_VERSION, (void*) &version );
    if ( lRtn != LDAP_SUCCESS )
        throw std::exception( "ConnectionFailed" );
  
    successOperation = ldap_connect( pLdapConnection_, NULL );
    if ( successOperation != LDAP_SUCCESS )
         throw std::exception( "ConnectionFailed" );

    successOperation = ldap_simple_bind_s( pLdapConnection_, pUser_, pPassword_ );
    if(successOperation != LDAP_SUCCESS)
        throw std::exception( "ldap bind failed" );
}

// -----------------------------------------------------------------------------
// Name: ReadLdapContent
// Created: SLG 2009-06-09
// -----------------------------------------------------------------------------
bool LdapClient::ReadLdapContent( const std::string& userProfile )
{   
  unsigned long errorCode = LDAP_SUCCESS;
  size_t charConverted;
  PWCHAR pMyFilter = PWCHAR ( L"(&(objectClass=nccoinetOrgPerson))" );
  PWCHAR pMyAttributes[2];
  PWCHAR pAttribute;
  PWCHAR* ppValue;
  BerElement* pBer;
  LDAPMessage* pEntry;

  pMyAttributes[0] = PWCHAR( L"uid" );
  pMyAttributes[1] = NULL;

  std::string strDN =  "uid=" + userProfile + ",ou=temporaire,ou=users,dc=ncco,ou=cad,o=dga,c=fr";

  pDN_ = new WCHAR[ strDN.length() ];
  mbstowcs_s(&charConverted, pDN_,strDN.length()+1, strDN.c_str(), strDN.length()+1 );

  errorCode = ldap_search_s( pLdapConnection_, pDN_, LDAP_SCOPE_SUBTREE, pMyFilter, pMyAttributes, 0, &pSearchResult_ );    

  if (errorCode != LDAP_SUCCESS)
  {
    LdapDeconnection();
    return false;
  }
  unsigned long numberOfEntries = GetNumberOfEntries();
  if( numberOfEntries != 1 )
    throw std::exception( "Ldap searched operation failed" );
  else
  {
    pEntry = ldap_first_entry( pLdapConnection_, pSearchResult_ );
    if ( pEntry == NULL )
    {
      LdapDeconnection();
      return false;
    }
    pAttribute = ldap_first_attribute( pLdapConnection_, pEntry, &pBer );
    if( pAttribute != NULL )
    {
      ppValue = ldap_get_values( pLdapConnection_, pEntry, pAttribute );
      if ( ppValue!= NULL )
      {
        LdapDeconnection();
        return true;
      }
      else
      {
        LdapDeconnection();
        return false;

      }
    }
  }
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
bool LdapClient::IsTakenIntoAccount( const std::string& messageType )
{
    for( std::vector< std::string >::iterator it = ldapContent_.begin(); it != ldapContent_.end(); it++ )
    {
        if ( *it == messageType )
            return true;
    }
    return false;
}
/*
// -----------------------------------------------------------------------------
// Name: IdentificationConfigure
// Created: SLG 2009-06-09
// -----------------------------------------------------------------------------
void LdapClient::IdentificationConfigure()
{
    secIdent_.User = (unsigned short*)L"cn=ldapadm";
    secIdent_.UserLength = strlen( "cn=ldapadm" );
    secIdent_.Password = (unsigned short*)L"P@ssw0rd";
    secIdent_.PasswordLength = strlen( "P@ssw0rd" );
    secIdent_.Domain = (unsigned short*)L"swpsot01.prod.ncco.cad.dga";
    secIdent_.DomainLength = strlen( "swpsot01.prod.ncco.cad.dga" );
    secIdent_.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
}
*/