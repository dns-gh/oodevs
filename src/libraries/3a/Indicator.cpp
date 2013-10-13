// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathematiques Appliquees SA (MASA)
//
// *****************************************************************************

#include "Indicator.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/AarSenders.h"

void SendIndicator( dispatcher::ClientPublisher_ABC& pub, const std::string& name,
        float value )
{
    aar::Indicator result;
    result().set_name( name.c_str() );
    result().set_value( value );
    result.Send( pub );
}
