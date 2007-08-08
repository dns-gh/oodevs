// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Simulation.h"

#include "Network_Def.h"
#include "game_asn/Asn.h"
#include "tools/AsnMessageEncoder.h"
#include "DIN/MessageService/DIN_MessageService_ABC.h"
#include "DIN/DIN_Link.h"
#include "MessageHandler_ABC.h"

using namespace dispatcher;
using namespace tools;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Simulation constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
Simulation::Simulation( MessageHandler_ABC& handler, DIN_MessageService_ABC& messageService, DIN_Link& link )
    : Server_ABC ( messageService, link )
    , handler_   ( handler )
{
    AsnMsgMiddleToSimCtrlClientAnnouncement().Send( *this );    
}

//-----------------------------------------------------------------------------
// Name: Simulation destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
Simulation::~Simulation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Simulation::OnReceive
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
void Simulation::OnReceive( const ASN1T_MsgsSimToClient& asnMsg )
{
    handler_.Receive( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Simulation::OnReceive
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void Simulation::OnReceive( const ASN1T_MsgsSimToMiddle& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Simulation::Send
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void Simulation::Send( const ASN1T_MsgsMiddleToSim& asnMsg )
{
    AsnMessageEncoder< ASN1T_MsgsMiddleToSim, ASN1C_MsgsMiddleToSim > asnEncoder( messageService_, asnMsg );
    messageService_.Send( link_, eMsgMiddleToSim, asnEncoder.GetDinMsg() );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Simulation::Send( const ASN1T_MsgsClientToSim& asnMsg )
{
    AsnMessageEncoder< ASN1T_MsgsClientToSim, ASN1C_MsgsClientToSim > asnEncoder( messageService_, asnMsg );
    messageService_.Send( link_, eMsgClientToSim, asnEncoder.GetDinMsg() );
}
