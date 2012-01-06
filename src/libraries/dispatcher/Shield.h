// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef dispatcher_Shield_h
#define dispatcher_Shield_h

#include <boost/noncopyable.hpp>
#include <memory>

namespace shield
{
    class Server;
    class Model_ABC;
    class Listener_ABC;
    class ClientHandler_ABC;
}

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace dispatcher
{
    class Model_ABC;
    class Config;

// =============================================================================
/** @class  Shield
    @brief  Shield
*/
// Created: MCO 2010-09-30
// =============================================================================
class Shield : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Shield( const Config& config, Model_ABC& model, tools::MessageDispatcher_ABC& dispatcher, shield::ClientHandler_ABC& handler );
    virtual ~Shield();
    //@}

    //! @name Operations
    //@{
    void Update( bool allowConnections );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< shield::Listener_ABC > logger_;
    std::auto_ptr< shield::Model_ABC > model_;
    std::auto_ptr< shield::Server > server_;
    //@}
};

}

#endif // dispatcher_Shield_h
