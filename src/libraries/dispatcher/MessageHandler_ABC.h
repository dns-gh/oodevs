// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MessageHandler_ABC_h_
#define __MessageHandler_ABC_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class SimToClient;
    class AarToClient;
    class MessengerToClient;
}

namespace dispatcher
{
// =============================================================================
/** @class  MessageHandler_ABC
    @brief  Simulation message handler definition
    // $$$$ SBO 2009-04-29: messages which can be handled by dispatcher plugins
*/
// Created: AGE 2007-07-05
// =============================================================================
class MessageHandler_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MessageHandler_ABC() {}
    virtual ~MessageHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& /*message*/ ) {}
    virtual void Receive( const sword::AarToClient& /*message*/ ) {}
    virtual void Receive( const sword::MessengerToClient& /*message*/ ) {}
    //@}
};

}

#endif // __MessageHandler_ABC_h_
