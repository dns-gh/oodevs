// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_TirIndirectParameter.h"
#include "simulation_orders/MIL_ParameterType_IndirectFire.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_TirIndirectParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_TirIndirectParameter::MIL_TirIndirectParameter( const Common::FireId& asn )
    : data_( asn.id() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_TirIndirectParameter destructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_TirIndirectParameter::~MIL_TirIndirectParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_TirIndirectParameter::IsOfType
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_TirIndirectParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_IndirectFire*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_TirIndirectParameter::ToIndirectFire
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_TirIndirectParameter::ToIndirectFire( Common::FireId& asn ) const
{
    asn.set_id( data_ );
    return true;
}
