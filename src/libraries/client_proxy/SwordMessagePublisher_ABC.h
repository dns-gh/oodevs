// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __SwordMessagePublisher_ABC_h_
#define __SwordMessagePublisher_ABC_h_

namespace MsgsClientToSim
{
    class MsgClientToSim;
}

// =============================================================================
/** @class  SwordMessagePublisher_ABC
    @brief  SwordMessagePublisher_ABC
*/
// Created: SEB 2010-10-14
// =============================================================================
class SwordMessagePublisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SwordMessagePublisher_ABC() {}
    virtual ~SwordMessagePublisher_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Send( const MsgsClientToSim::MsgClientToSim& message ) const = 0;
    //@}
};

#endif // __SwordMessagePublisher_ABC_h_
