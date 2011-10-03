// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Masagroup
//
// *****************************************************************************

#ifndef __ObjectMessageCallback_ABC_h_
#define __ObjectMessageCallback_ABC_h_

#include <string>

namespace tools
{
    class Message;
    class MessageCallback_ABC;

// =============================================================================
/** @class  ObjectMessageCallback_ABC
    @brief  ObjectMessageCallback_ABC
*/
// Created: AGE 2007-03-07
// =============================================================================
class ObjectMessageCallback_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectMessageCallback_ABC() {}
    virtual ~ObjectMessageCallback_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void OnMessage( const std::string& link, Message& message, MessageCallback_ABC& callback ) = 0;
    //@}
};

}

#endif // __ObjectMessageCallback_ABC_h_
