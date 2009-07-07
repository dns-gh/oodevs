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
    LdapClient();
    virtual ~LdapClient();
    //@}

    //! @name Operations
    //@{
    void LdapConnection();
    void LdapDeconnection();
    void ReadLdapContent();
    void SendContentToLdap();
    bool IsTakenIntoAccount( const std::string messageType );
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
    PWCHAR hostName;
    PWCHAR pDN_;
    LDAPMessage* pSearchResult_;

    T_LdapClientVector ldapContent_;

    //@}
};

}
}

#endif // __Ldap_h_
