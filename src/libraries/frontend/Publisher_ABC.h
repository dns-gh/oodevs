// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Publisher_ABC_h_
#define __Publisher_ABC_h_

#include "network/Master_Asn.h"

namespace DIN
{
    class DIN_BufferedMessage;
}

namespace frontend
{

// =============================================================================
/** @class  Publisher_ABC
    @brief  Publisher_ABC
*/
// Created: SBO 2007-01-25
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
    virtual void Send( const ASN1T_MsgsInMaster& message ) = 0;
    virtual DIN::DIN_BufferedMessage BuildDinMsg() = 0;
    //@}
};

}

#endif // __Publisher_ABC_h_
