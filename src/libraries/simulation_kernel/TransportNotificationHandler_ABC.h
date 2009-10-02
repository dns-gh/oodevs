/*
 * TransportNotificationHandler_ABC.h
 *
 *  Created on: 24 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_TransportNotificationHandler_ABC_H__
#define __transport_TransportNotificationHandler_ABC_H__

class MIL_Agent_ABC;
class PHY_ComposanteState;
class PHY_ComposantePion;

namespace transport
{
class TransportNotificationHandler_ABC
{
public:
    TransportNotificationHandler_ABC();
    virtual  ~TransportNotificationHandler_ABC();

    virtual void LoadForTransport   ( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable ) {};
    virtual void UnloadFromTransport( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable ) {};
    virtual void CancelTransport    ( const MIL_Agent_ABC& transporter ) {};

    virtual void MagicLoadPion   ( MIL_Agent_ABC& pion, bool bTransportOnlyLoadable ) {};
    virtual void MagicUnloadPion ( MIL_Agent_ABC& pion ) {};

    virtual void DamageTransported( double rWeight, const PHY_ComposanteState& state, bool bTransportOnlyLoadable ) const {};
    virtual void NotifyComposanteChanged( const PHY_ComposantePion& composante ) {};

    virtual void CheckConsistency() {};
};

}

#endif /* __transport_TransportNotificationHandler_ABC_H__ */
