// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Plugin_ABC_h_
#define __Plugin_ABC_h_

#include "MessageHandler_ABC.h"

namespace dispatcher
{
    class Profile_ABC;
    class ClientPublisher_ABC;
    class Services;

// =============================================================================
/** @class  Plugin_ABC
    @brief  Plugin_ABC
*/
// Created: AGE 2007-08-24
// =============================================================================
class Plugin_ABC : public MessageHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Plugin_ABC() {};
    virtual ~Plugin_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Receive                  ( const ASN1T_MsgsSimToClient& message ) = 0;
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile ) = 0;
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client ) = 0;
    virtual void Register                 ( dispatcher::Services& ) {}
    virtual void Update() {}
    //@}
};

}

#endif // __Plugin_ABC_h_
