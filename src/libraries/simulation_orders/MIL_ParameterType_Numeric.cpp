// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_Numeric.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Numeric constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_Numeric::MIL_ParameterType_Numeric()
    : MIL_ParameterType_ABC( "Numeric" )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Numeric destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_Numeric::~MIL_ParameterType_Numeric()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Numeric::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_Numeric::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.set_null_value( false );
    float value = to.value().areal();
    return from.ToNumeric( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Numeric::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_Numeric::CleanAfterSerialization( Common::MsgMissionParameter& to ) const
{
    assert( to.value().has_areal() );
}
