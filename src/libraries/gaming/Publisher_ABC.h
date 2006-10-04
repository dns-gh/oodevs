// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Publisher_ABC_h_
#define __Publisher_ABC_h_

#include "ASN_Types.h"

namespace kernel
{
    class Agent_ABC;
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
    virtual void Send( ASN1T_MsgsOutClient& message ) = 0;
    virtual void SendMagicDestruction( const kernel::Agent_ABC& agent ) = 0; // $$$$ SBO 2006-07-06: 
    //@}
};

#endif // __Publisher_ABC_h_
