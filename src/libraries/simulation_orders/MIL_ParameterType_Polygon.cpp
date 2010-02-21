// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_Polygon.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Polygon constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_Polygon::MIL_ParameterType_Polygon( const std::string& name )
    : MIL_ParameterType_ABC( name )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Polygon destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_Polygon::~MIL_ParameterType_Polygon()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Polygon::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_Polygon::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;
    to.set_null_value( !from.ToPolygon( *to.mutable_value()->mutable_polygon() ) );
    return !to.null_value() || bIsOptional;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Polygon::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_Polygon::CleanAfterSerialization( Common::MsgMissionParameter& to ) const
{
    assert( to.value().has_polygon() );
    assert( to.mutable_value()->mutable_polygon() );
    to.mutable_value()->clear_polygon();
    delete to.mutable_value()->mutable_polygon();    
}
