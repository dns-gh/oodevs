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

namespace MsgsClientToSim            { class MsgClientToSim; }
namespace MsgsClientToAuthentication { class MsgClientToAuthentication; }
namespace MsgsClientToReplay         { class MsgClientToReplay; }
namespace MsgsClientToAar            { class MsgClientToAar; }
namespace MsgsClientToMessenger      { class MsgClientToMessenger; }

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
    virtual void Send( const MsgsClientToSim::MsgClientToSim& message ) = 0;
    virtual void Send( const MsgsClientToAuthentication::MsgClientToAuthentication& message ) = 0;
    virtual void Send( const MsgsClientToReplay::MsgClientToReplay& message ) = 0;
    virtual void Send( const MsgsClientToAar::MsgClientToAar& message ) = 0;
    virtual void Send( const MsgsClientToMessenger::MsgClientToMessenger& message ) = 0;
    //@}
};

#endif // __protocol_Publisher_ABC_h_
