// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClientManager_h_
#define __ClientManager_h_

#include "dispatcher/Plugin_ABC.h"

namespace xml
{
    class xistream;
    class xostream;
}

namespace plugins
{
namespace xmlia
{
    class LdapClient;
    
// =============================================================================
/** @class  ClientManager
@brief  ClientManager
*/
// Created: SLG 2009-06-09
// =============================================================================
class ClientManager
{

public:
    typedef std::map < std::string, std::vector< std::string > > rolesMap;

    //! @name Constructors/Destructor
    //@{
    ClientManager( xml::xistream& xis, LdapClient& ldap );
    virtual ~ClientManager();
    //@}

    //! @name Operations
    //@{
    dispatcher::Profile_ABC* GetClientProfile() const;
    dispatcher::ClientPublisher_ABC* GetClientPublisher() const;
    void NotifyClient( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client );
    bool HasNoClient();
    bool IsTakenIntoAccount( const std::string messageType );
    //@}

    //! @name Helpers
    //@{
    void ReadProfile( xml::xistream& xis );
    void ReadRole( xml::xistream& xis, std::vector < std::string >& roles );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::ClientPublisher_ABC* clientPublisher_;
    dispatcher::Profile_ABC* clientProfile_;

    rolesMap rolesFilter_;
    LdapClient& ldap_;

    bool isAuthorized_;
    //@}
 
};

}
}

#endif // __Simulation_h_
