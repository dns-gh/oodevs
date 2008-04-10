// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "Lima.h"
#include <xeumeuleu/xml.h>

#include "ASN_Messages.h"

using namespace messenger;

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
Lima::Lima( unsigned int id, const ASN1T_MsgLimaCreationRequest& message )
    : TacticalLine_ABC( id, message )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: RDS 2008-04-03
// -----------------------------------------------------------------------------
Lima::Lima( unsigned int id, xml::xistream& xis, const ASN1T_TacticalLinesDiffusion& diffusion, const kernel::CoordinateConverter_ABC& converter )
    : TacticalLine_ABC( id, xis, diffusion, converter)
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lima destructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
Lima::~Lima()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lima::Update
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void Lima::Update( const ASN1T_MsgLimaUpdateRequest& message )
{
    TacticalLine_ABC::Update( message.tactical_line );
}

// -----------------------------------------------------------------------------
// Name: Lima::SendCreation
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void Lima::SendCreation( dispatcher::ClientPublisher_ABC& client) const
{
    ASN1T_MsgLimaCreation creation ;
    creation.oid = GetID();
    TacticalLine_ABC::Send(creation.tactical_line);
    ASN_MsgLimaCreation message(creation);
    message.Send(client);
}

// -----------------------------------------------------------------------------
// Name: Lima::SendUpdate
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void Lima::SendUpdate( dispatcher::ClientPublisher_ABC& client) const
{
    ASN1T_MsgLimaUpdate update;
    update.oid = GetID();
    TacticalLine_ABC::Send(update.tactical_line);
    ASN_MsgLimaUpdate message(update);
    message.Send(client);
}

// -----------------------------------------------------------------------------
// Name: Lima::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Lima::SendDestruction( dispatcher::ClientPublisher_ABC& client) const
{
    ASN1T_MsgLimaDestruction destruction = GetID();
    ASN_MsgLimaDestruction message(destruction);
    message.Send(client);
}

// -----------------------------------------------------------------------------
// Name: Lima::SendFullUpdate
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void Lima::SendFullState( dispatcher::ClientPublisher_ABC& client) const
{
    SendCreation(client);
}

// -----------------------------------------------------------------------------
// Name: Lima::Write
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void Lima::Write( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter )
{
    xos << xml::start("lima") ;
        TacticalLine_ABC::Write(xos,converter);
    xos << xml::end();
}
