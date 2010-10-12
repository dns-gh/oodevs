// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MsgsSimToClientListener_h_
#define __MsgsSimToClientListener_h_

namespace MsgsSimToClient
{
    class MsgSimToClient;

// =============================================================================
/** @class  MsgsSimToClientListener
    @brief  MsgsSimToClientListener
*/
// Created: LDC 2010-10-11
// =============================================================================
class Listener
{

public:
    //! @name Constructors/Destructor
    //@{
             Listener() {}
    virtual ~Listener() {}
    //@}

    //! @name Operations
    //@{
    //@return true if listener can be removed from list after processing message
    virtual bool OnMessageReceived( const MsgSimToClient& message ) = 0;
    //@}
};

}

#endif // __MsgsSimToClientListener_h_
