// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_PathList.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_PathList constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_PathList::MIL_ParameterType_PathList()
    : MIL_ParameterType_ABC( "PathList" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_PathList destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_PathList::~MIL_ParameterType_PathList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_PathList::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_PathList::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;
    to.set_null_value( false );
    return from.ToPathList( *to.mutable_value()->mutable_pathlist() );
}
