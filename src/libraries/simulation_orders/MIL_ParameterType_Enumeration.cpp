// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_Enumeration.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Enumeration constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_Enumeration::MIL_ParameterType_Enumeration()
    : MIL_ParameterType_ABC( "enumeration" )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Enumeration destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_Enumeration::~MIL_ParameterType_Enumeration()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Enumeration::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_Enumeration::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    int id;
    to.set_null_value( !from.ToId( id ) );
    to.mutable_value()->Add()->set_enumeration( id );
    if( to.null_value() )
        to.clear_value();
    return !to.null_value() || bIsOptional;
}
