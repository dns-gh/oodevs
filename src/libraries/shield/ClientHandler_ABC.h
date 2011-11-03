// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_ClientHandler_ABC_h
#define shield_ClientHandler_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace tools
{
    class MessageSender_ABC;
}

namespace shield
{
// =============================================================================
/** @class  ClientHandler_ABC
    @brief  Client handler declaration
*/
// Created: MCO 2011-10-28
// =============================================================================
class ClientHandler_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientHandler_ABC() {}
    virtual ~ClientHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Register( const std::string& endpoint, tools::MessageSender_ABC& sender ) = 0;
    virtual void Unregister( const std::string& endpoint ) = 0;
    //@}
};

}

#endif // shield_ClientHandler_ABC_h
