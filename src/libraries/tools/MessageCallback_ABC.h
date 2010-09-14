// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MessageCallback_ABC_h_
#define __MessageCallback_ABC_h_

namespace tools
{
    class Message;

// =============================================================================
/** @class  MessageCallback_ABC
    @brief  Message callback declaration
*/
// Created: AGE 2007-09-06
// =============================================================================
class MessageCallback_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MessageCallback_ABC() {}
    virtual ~MessageCallback_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void OnError( const std::string& endpoint, const std::string& error ) = 0;
    virtual void OnMessage( const std::string& endpoint, Message& message ) = 0;
    //@}

};

}

#endif // __MessageCallback_ABC_h_
