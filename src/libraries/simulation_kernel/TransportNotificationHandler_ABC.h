/*
 * TransportNotificationHandler_ABC.h
 *
 *  Created on: 24 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_TransportNotificationHandler_ABC_H__
#define __transport_TransportNotificationHandler_ABC_H__

class MIL_Agent_ABC;

namespace transport
{
class TransportNotificationHandler_ABC
{
public:
    TransportNotificationHandler_ABC();
    virtual  ~TransportNotificationHandler_ABC();

    virtual void LoadForTransport   ( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable ) = 0;
    virtual void UnloadFromTransport( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable ) = 0;
    virtual void CancelTransport    ( const MIL_Agent_ABC& transporter ) = 0;
};

}

#endif /* __transport_TransportNotificationHandler_ABC_H__ */
