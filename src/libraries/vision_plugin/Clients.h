// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef plugins_vision_Clients_h
#define plugins_vision_Clients_h

#include <set>

namespace sword
{
    class SimToClient;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace plugins
{
namespace vision
{
// =============================================================================
/** @class  Clients
    @brief  List of clients subscribed to all unit vision cone updates
*/
// Created: MCO 2013-08-21
// =============================================================================
class Clients
{
public:
    void Register( dispatcher::ClientPublisher_ABC& publisher );
    void Unregister( dispatcher::ClientPublisher_ABC& publisher );

    void Send( const sword::SimToClient& message );

    bool IsRegistered( dispatcher::ClientPublisher_ABC& publisher ) const;

private:
    std::set< dispatcher::ClientPublisher_ABC* > clients_;
};

}
}

#endif // plugins_vision_Clients_h
