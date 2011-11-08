// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Chat_h_
#define __Chat_h_

#include "protocol/Protocol.h"

namespace dispatcher
{
    class Profile_ABC;
    class ClientPublisher_ABC;
}

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace plugins
{
namespace messenger
{

// =============================================================================
/** @class  Chat
    @brief  Chat
*/
// Created: AGE 2008-06-10
// =============================================================================
class Chat
{
public:
    //! @name Constructors/Destructor
    //@{
             Chat();
    virtual ~Chat();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client );

    void OnReceive( const sword::TextMessage& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Chat( const Chat& );            //!< Copy constructor
    Chat& operator=( const Chat& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< dispatcher::ClientPublisher_ABC*, dispatcher::Profile_ABC* > T_Clients;
    //@}

private:
    //! @name Member data
    //@{
    T_Clients clients_;
    //@}
};

}
}

#endif // __Chat_h_
