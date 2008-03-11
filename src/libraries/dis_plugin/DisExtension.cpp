// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "DisExtension.h"
#include "EntityStatePDU.h"
#include "UdpNetwork.h"
#include "Time_ABC.h"

using namespace dis;

// -----------------------------------------------------------------------------
// Name: DisExtension constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisExtension::DisExtension( const Time_ABC& time, UdpNetwork& network, dispatcher::Agent& holder, const EntityIdentifier& id, unsigned char exercise )
    : time_   ( time )
    , network_( network )
    , holder_ ( holder )
    , exercise_( exercise )
    , id_     ( id )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisExtension destructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisExtension::~DisExtension()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisExtension::DoUpdate
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
void DisExtension::DoUpdate( const ASN1T_MsgUnitAttributes& )
{
    EntityStatePDU pdu( holder_, id_, time_.GetTime(), exercise_ );
    network_.Send( pdu );
}
