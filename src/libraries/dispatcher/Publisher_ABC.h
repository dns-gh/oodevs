 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Publisher_ABC_h_
#define __Publisher_ABC_h_

#include "AsnTypes.h"

namespace dispatcher
{
// =============================================================================
/** @class  Publisher_ABC
    @brief  Publisher_ABC
*/
// Created: NLD 2006-09-19
// =============================================================================
class Publisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     Publisher_ABC();
    ~Publisher_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const ASN1T_MsgsInSim&    msg ) = 0;
    virtual void Send( const ASN1T_MsgsInClient& msg ) = 0;
    
    //$$ TMP DIN
    virtual void                     Send     ( unsigned int nMsgID, const DIN::DIN_BufferedMessage& dinMsg ) = 0; //$$$ TMP DIN
    virtual DIN::DIN_BufferedMessage GetDinMsg() = 0;
    //@}

private:
};

}

#endif // __Publisher_ABC_h_
