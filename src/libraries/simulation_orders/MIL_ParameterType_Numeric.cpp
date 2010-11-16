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
// Created: RPD 2010-11-16
// -----------------------------------------------------------------------------
MIL_ParameterType_Numeric::MIL_ParameterType_Numeric( const std::string& name )
    : MIL_ParameterType_ABC( name )
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
bool MIL_ParameterType_Numeric::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    if( !from.IsOfType( *this ) )
        return false;
    float value;
    to.set_null_value( !from.ToNumeric( value ) );
    to.mutable_value()->Add()->set_areal( value );
    if( to.null_value() )
        to.clear_value();
    return !to.null_value() || bIsOptional;
}
