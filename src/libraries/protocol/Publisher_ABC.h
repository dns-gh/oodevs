// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __protocol_Publisher_ABC_h_
#define __protocol_Publisher_ABC_h_

namespace sword
{
    class ClientToAar;
    class ClientToAuthentication;
    class ClientToMessenger;
    class ClientToReplay;
    class ClientToSim;
}

// =============================================================================
/** @class  Publisher_ABC
    @brief  Publisher abstract base class
*/
// Created: SBO 2006-07-06
// =============================================================================
class Publisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Publisher_ABC() {}
    virtual ~Publisher_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Send( const sword::ClientToAar& message ) = 0;
    virtual void Send( const sword::ClientToAuthentication& message ) = 0;
    virtual void Send( const sword::ClientToSim& message ) = 0;
    virtual void Send( const sword::ClientToReplay& message ) = 0;
    virtual void Send( const sword::ClientToMessenger& message ) = 0;
    //@}
};

#endif // __protocol_Publisher_ABC_h_
