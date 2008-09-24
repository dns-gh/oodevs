// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "DetonationPDU.h"

using namespace plugins::dis;
using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: DetonationPDU constructor
// Created: AGE 2008-04-30
// -----------------------------------------------------------------------------
DetonationPDU::DetonationPDU( const EntityIdentifier& firer, unsigned long time, unsigned char exercise )
    : header_                        ( DisHeader::DetonationPDU( time, exercise ) )
    , firer_                         ( firer )
    , detonationResult_              ( 3 ) // Ground Impact
    , numberOfArticulationParameters_( 0 )
    , padding_                       ( 0 )
{
    ::memset( locInEntityCoords_, 0, sizeof( locInEntityCoords_ ) );
}

// -----------------------------------------------------------------------------
// Name: DetonationPDU destructor
// Created: AGE 2008-04-30
// -----------------------------------------------------------------------------
DetonationPDU::~DetonationPDU()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetonationPDU::SetBurst
// Created: AGE 2008-05-05
// -----------------------------------------------------------------------------
void DetonationPDU::SetBurst( unsigned short quantity, unsigned seconds, BurstDescriptor::warhead w )
{
    burst_ = BurstDescriptor( quantity, seconds, w );
}

// -----------------------------------------------------------------------------
// Name: DetonationPDU::SetPosition
// Created: AGE 2008-05-05
// -----------------------------------------------------------------------------
void DetonationPDU::SetPosition( double latitude, double longitude, float altitude )
{
    location_ = WorldLocation( latitude, longitude, altitude );
}
