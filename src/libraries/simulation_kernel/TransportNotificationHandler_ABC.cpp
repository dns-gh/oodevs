// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "TransportNotificationHandler_ABC.h"

using namespace transport;

// -----------------------------------------------------------------------------
// Name: TransportNotificationHandler_ABC constructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
TransportNotificationHandler_ABC::TransportNotificationHandler_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TransportNotificationHandler_ABC destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
TransportNotificationHandler_ABC::~TransportNotificationHandler_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TransportNotificationHandler_ABC::LoadForTransport
// Created: LDC 2009-10-05
// -----------------------------------------------------------------------------
void TransportNotificationHandler_ABC::LoadForTransport( const MIL_Agent_ABC& /*transporter*/, bool /*bTransportOnlyLoadable*/, bool& /*bTransportedByAnother*/ )
{

}
// -----------------------------------------------------------------------------
// Name: TransportNotificationHandler_ABC::UnloadFromTransport
// Created: LDC 2009-10-05
// -----------------------------------------------------------------------------
void TransportNotificationHandler_ABC::UnloadFromTransport( const MIL_Agent_ABC& /*transporter*/, bool /*bTransportOnlyLoadable*/, MT_Vector2D* /*position*/ )
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
// Name: TransportNotificationHandler_ABC::ForceUnloadedState
// Created: MGD 2009-10-25
// -----------------------------------------------------------------------------
void TransportNotificationHandler_ABC::ForceUnloadedState()
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
