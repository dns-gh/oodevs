// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __transport_TransportNotificationHandler_ABC_H__
#define __transport_TransportNotificationHandler_ABC_H__

class MIL_Agent_ABC;
class PHY_ComposantePion;
class PHY_ComposanteState;
class MT_Vector2D;

namespace transport
{

// =============================================================================
/** @class  TransportNotificationHandler_ABC
    @brief  TransportNotificationHandler_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================
class TransportNotificationHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
              TransportNotificationHandler_ABC();
    virtual  ~TransportNotificationHandler_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void LoadForTransport   ( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable, bool& bTransportedByAnother );
    virtual void UnloadFromTransport( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable, MT_Vector2D* position );
    virtual void CancelTransport    ( const MIL_Agent_ABC& transporter );

    virtual void ForceUnloadedState();

    virtual void MagicLoadPion   ( MIL_Agent_ABC& pion, bool bTransportOnlyLoadable );
    virtual void MagicUnloadPion ( MIL_Agent_ABC& pion );

    virtual void DamageTransported( double rWeight, const PHY_ComposanteState& state, bool bTransportOnlyLoadable ) const;
    virtual void NotifyComposanteChanged( const PHY_ComposantePion& composante );

    virtual void CheckConsistency();
    //@}
};

}

#endif /* __transport_TransportNotificationHandler_ABC_H__ */
