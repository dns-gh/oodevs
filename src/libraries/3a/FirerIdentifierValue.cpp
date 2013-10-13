// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "FirerIdentifierValue.h"
#include "protocol/Simulation.h"

void FirerIdentifierValue::Receive( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_unit_damaged_by_unit_fire() )
        Set( wrapper.message().unit_damaged_by_unit_fire().firer().id() );
}

void TargetIdentifierValue::Receive( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_unit_damaged_by_unit_fire() )
        Set( wrapper.message().unit_damaged_by_unit_fire().unit().id() );
}
