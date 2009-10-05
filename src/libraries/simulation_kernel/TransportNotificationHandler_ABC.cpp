/*
 * TransportNotificationHandler_ABC.cpp
 *
 *  Created on: 24 sept. 2009
 *      Author: ahc
 */
#include "simulation_kernel_pch.h"
#include "TransportNotificationHandler_ABC.h"

namespace transport
{

  TransportNotificationHandler_ABC::TransportNotificationHandler_ABC()
  {
  }

  TransportNotificationHandler_ABC::~TransportNotificationHandler_ABC()
  {
  }

// -----------------------------------------------------------------------------
// Name: TransportNotificationHandler_ABC::LoadForTransport
// Created: LDC 2009-10-05
// -----------------------------------------------------------------------------
void TransportNotificationHandler_ABC::LoadForTransport( const MIL_Agent_ABC& /*transporter*/, bool /*bTransportOnlyLoadable*/ )
{

}
// -----------------------------------------------------------------------------
// Name: TransportNotificationHandler_ABC::UnloadFromTransport
// Created: LDC 2009-10-05
// -----------------------------------------------------------------------------
void TransportNotificationHandler_ABC::UnloadFromTransport( const MIL_Agent_ABC& /*transporter*/, bool /*bTransportOnlyLoadable*/ )
{

}
// -----------------------------------------------------------------------------
// Name: TransportNotificationHandler_ABC::CancelTransport
// Created: LDC 2009-10-05
// -----------------------------------------------------------------------------
void TransportNotificationHandler_ABC::CancelTransport( const MIL_Agent_ABC& /*transporter*/ )
{

}

// -----------------------------------------------------------------------------
// Name: TransportNotificationHandler_ABC::MagicLoadPion
// Created: LDC 2009-10-05
// -----------------------------------------------------------------------------
void TransportNotificationHandler_ABC::MagicLoadPion( MIL_Agent_ABC& /*pion*/, bool /*bTransportOnlyLoadable*/ )
{

}
// -----------------------------------------------------------------------------
// Name: TransportNotificationHandler_ABC::MagicUnloadPion
// Created: LDC 2009-10-05
// -----------------------------------------------------------------------------
void TransportNotificationHandler_ABC::MagicUnloadPion( MIL_Agent_ABC& /*pion*/ )
{

}

// -----------------------------------------------------------------------------
// Name: TransportNotificationHandler_ABC::DamageTransported
// Created: LDC 2009-10-05
// -----------------------------------------------------------------------------
void TransportNotificationHandler_ABC::DamageTransported( double /*rWeight*/, const PHY_ComposanteState& /*state*/, bool /*bTransportOnlyLoadable*/ ) const
{

}
// -----------------------------------------------------------------------------
// Name: TransportNotificationHandler_ABC::NotifyComposanteChanged
// Created: LDC 2009-10-05
// -----------------------------------------------------------------------------
void TransportNotificationHandler_ABC::NotifyComposanteChanged( const PHY_ComposantePion& /*composante*/ )
{

}

// -----------------------------------------------------------------------------
// Name: TransportNotificationHandler_ABC::CheckConsistency
// Created: LDC 2009-10-05
// -----------------------------------------------------------------------------
void TransportNotificationHandler_ABC::CheckConsistency()
{

}

}
