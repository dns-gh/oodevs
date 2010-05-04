// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_DirectionParameter.h"
#include "simulation_orders/MIL_ParameterType_Direction.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_DirectionParameter::MIL_DirectionParameter( const Common::MsgHeading& heading )
    : heading_( heading.heading() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter destructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_DirectionParameter::~MIL_DirectionParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter::IsOfType
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_DirectionParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_Direction*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter::ToDirection
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_DirectionParameter::ToDirection( Common::MsgHeading& asn ) const
{
    asn.set_heading( heading_ );
    return true;
}


// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToDirection
// Created: MGD 2010-05-03
// -----------------------------------------------------------------------------
bool MIL_DirectionParameter::ToDirection( boost::shared_ptr< MT_Vector2D >& dir ) const
{
    MT_Vector2D direction( 0,1 );
    direction.Rotate( ( heading_ / 360.f ) * MT_PI * 2 );
    dir.reset( new MT_Vector2D( direction ) );
    return true;
}
