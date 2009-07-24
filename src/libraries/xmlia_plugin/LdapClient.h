// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Ldap_h_
#define __Ldap_h_

#include <Windows.h>
#include <Winldap.h>
#include <stdio.h>

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace xmlia
{

// =============================================================================
/** @class  Ldap
@brief  Ldap
*/
// Created: SLG 2009-06-09
// =============================================================================
class LdapClient
{

public:
    //! @name Constructors/Destructor
    //@{
    LdapClient( xml::xistream& xis );
    virtual ~LdapClient();
    //@}

    //! @name Operations
    //@{
    void LdapConnection();
    void LdapDeconnection();
    bool ReadLdapContent( const std::string& userProfile );
    void SendContentToLdap();
    bool IsTakenIntoAccount( const std::string& messageType );
    void IdentificationConfigure();
    //@}


private:
    //! @name Copy/Assignment
    //@{
    LdapClient( const LdapClient& );            //!< Copy constructor
    LdapClient& operator=( const LdapClient& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    unsigned long GetNumberOfEntries();
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::string > T_LdapClientVector;
    //@}

private:
    //! @name Member data
    //@{
    LDAP* pLdapConnection_;
    LDAPAPIInfo* pMyInfo_;
    PWCHAR pHostName_;
    PWCHAR pDN_;
    PWCHAR pUser_;
    PWCHAR pPassword_;
    SEC_WINNT_AUTH_IDENTITY secIdent_;
    LDAPMessage* pSearchResult_;
    std::vector< PWCHAR* > pAttributesValues_;

    T_LdapClientVector ldapContent_;

    //@}
};

}
}

#endif // __Ldap_h_
