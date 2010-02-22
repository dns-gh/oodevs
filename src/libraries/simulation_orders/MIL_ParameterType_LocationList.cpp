// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_LocationList.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_LocationList constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_LocationList::MIL_ParameterType_LocationList()
    : MIL_ParameterType_ABC( "LocationList" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_LocationList destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_LocationList::~MIL_ParameterType_LocationList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_LocationList::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_LocationList::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;
    to.set_null_value( false );
    return from.ToLocationList( *to.mutable_value()->mutable_locationlist() );
}
