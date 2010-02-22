// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_MedicalPriorities.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MedicalPriorities constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_MedicalPriorities::MIL_ParameterType_MedicalPriorities()
    : MIL_ParameterType_ABC( "MedicalPriorities" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MedicalPriorities destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_MedicalPriorities::~MIL_ParameterType_MedicalPriorities()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MedicalPriorities::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_MedicalPriorities::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;
    to.set_null_value( !from.ToMedicalPriorities( *to.mutable_value()->mutable_logmedicalpriorities() ) );
    return !to.null_value() || bIsOptional;
}
