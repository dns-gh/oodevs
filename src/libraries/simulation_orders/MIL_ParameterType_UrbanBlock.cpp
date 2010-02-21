// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_UrbanBlock.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_UrbanBlock constructor
// Created: MGD 2009-11-02
//-----------------------------------------------------------------------------
MIL_ParameterType_UrbanBlock::MIL_ParameterType_UrbanBlock( const std::string& name )
    : MIL_ParameterType_ABC( name )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_UrbanBlock destructor
// Created: MGD 2009-11-02
//-----------------------------------------------------------------------------
MIL_ParameterType_UrbanBlock::~MIL_ParameterType_UrbanBlock()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_UrbanBlock::Copy
// Created: MGD 2009-11-02
//-----------------------------------------------------------------------------
bool MIL_ParameterType_UrbanBlock::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;
    to.set_null_value( !from.ToUrbanBlock( *to.mutable_value()->mutable_urbanblock() ) );
    return !to.null_value() || bIsOptional;
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_UrbanBlock::CleanAfterSerialization
// Created: MGD 2009-11-02
//-----------------------------------------------------------------------------
void MIL_ParameterType_UrbanBlock::CleanAfterSerialization( Common::MsgMissionParameter& to ) const
{
    assert( to.value().has_urbanblock() );
    to.mutable_value()->clear_urbanblock();
}
