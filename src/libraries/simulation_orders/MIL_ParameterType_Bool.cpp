// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_Bool.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Bool constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_Bool::MIL_ParameterType_Bool()
    : MIL_ParameterType_ABC( "bool" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Bool destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_Bool::~MIL_ParameterType_Bool()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Bool::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_Bool::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    bool value;
    to.set_null_value( !from.ToBool( value ) );
    if( !to.null_value() )
        to.mutable_value()->Add()->set_booleanvalue( value );
    return !to.null_value() || bIsOptional;
}
