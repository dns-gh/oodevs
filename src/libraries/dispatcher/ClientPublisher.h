 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClientPublisher_h_
#define __ClientPublisher_h_

#include "AsnTypes.h"

#include "Publisher_ABC.h"

namespace dispatcher
{
class Client;

// =============================================================================
/** @class  ClientPublisher
    @brief  ClientPublisher
*/
// Created: NLD 2006-09-19
// =============================================================================
class ClientPublisher : public Publisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     ClientPublisher( Client& client );
    ~ClientPublisher();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const ASN1T_MsgsInSim&    msg );
    virtual void Send( const ASN1T_MsgsInClient& msg );

    //$$ TMP DIN
    virtual void                     Send     ( unsigned int nMsgID, const DIN::DIN_BufferedMessage& dinMsg ); //$$$ TMP DIN
    virtual DIN::DIN_BufferedMessage GetDinMsg();
    //@}

private:
    Client& client_;
};

}

#endif // __ClientPublisher_h_
