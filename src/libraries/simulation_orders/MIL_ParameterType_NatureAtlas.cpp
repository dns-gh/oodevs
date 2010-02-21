// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_NatureAtlas.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_NatureAtlas constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_NatureAtlas::MIL_ParameterType_NatureAtlas()
    : MIL_ParameterType_ABC( "NatureAtlas" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_NatureAtlas destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_NatureAtlas::~MIL_ParameterType_NatureAtlas()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_NatureAtlas::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_NatureAtlas::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;
    to.set_null_value( false );
    return from.ToNatureAtlas( *to.mutable_value()->mutable_atlasnature() );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_NatureAtlas::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_NatureAtlas::CleanAfterSerialization( Common::MsgMissionParameter& to ) const
{
    assert( to.value().has_atlasnature() );
    assert( to.mutable_value()->mutable_atlasnature() );
    delete to.mutable_value()->mutable_atlasnature();
}
