// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_Location.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Location constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_Location::MIL_ParameterType_Location()
    : MIL_ParameterType_ABC( "Location" )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Location destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_Location::~MIL_ParameterType_Location()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Location::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_Location::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;
    to.set_null_value( !from.ToLocation( *to.mutable_value()->mutable_location() ));
    return !to.null_value() || bIsOptional;
}
