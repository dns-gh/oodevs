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

namespace sword
{
    class SimToClient;

// =============================================================================
/** @class  MsgsSimToClientListener
    @brief  MsgsSimToClientListener
*/
// Created: LDC 2010-10-11
// =============================================================================
class Listener // $$$$ MCO : missing _ABC + file and class must have the same name
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
    virtual bool OnMessageReceived( const SimToClient& message ) = 0;
    //@}
};

}

#endif // __MsgsSimToClientListener_h_
