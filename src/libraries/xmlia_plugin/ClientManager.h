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


namespace plugins
{
namespace xmlia
{
    
// =============================================================================
/** @class  ClientManager
@brief  ClientManager
*/
// Created: SLG 2009-06-09
// =============================================================================
class ClientManager
{

public:
    //! @name Constructors/Destructor
    //@{
    ClientManager();
    virtual ~ClientManager();
    //@}

    //! @name Operations
    //@{
    void SetClientProfile( dispatcher::Profile_ABC& profile );
    void SetClientPublisher( dispatcher::ClientPublisher_ABC& clientPublisher );
    void SetClientParameters( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    dispatcher::Profile_ABC* GetClientProfile() const;
    dispatcher::ClientPublisher_ABC* GetClientPublisher() const;
    void NotifyClient( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client );
    //@}

    struct clientAttributes {
        dispatcher::ClientPublisher_ABC* clientPublisher;
        dispatcher::Profile_ABC* clientProfile;
    };

private:
    //! @name Member data
    //@{
    dispatcher::Profile_ABC* clientProfile_;
    dispatcher::ClientPublisher_ABC* clientPublisher_;
    std::map < std::string, clientAttributes > clientParameters_;
    //@}
 
};

}
}

#endif // __Simulation_h_
