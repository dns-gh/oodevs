// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"

#include "MIL_ParameterType_Lima.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Lima constructor
// Created: MGD 2010-10-27
//-----------------------------------------------------------------------------
MIL_ParameterType_Lima::MIL_ParameterType_Lima()
    : MIL_ParameterType_ABC( "PhaseLine" )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Lima destructor
// Created: MGD 2010-10-27
//-----------------------------------------------------------------------------
MIL_ParameterType_Lima::~MIL_ParameterType_Lima()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Lima::Copy
// Created: MGD 2010-10-27
//-----------------------------------------------------------------------------
bool MIL_ParameterType_Lima::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;
    to.set_null_value( !from.ToLima( *to.mutable_value()->mutable_limasorder() ) );
    if( to.null_value() )
        to.clear_value();
    return !to.null_value() || bIsOptional;
}
