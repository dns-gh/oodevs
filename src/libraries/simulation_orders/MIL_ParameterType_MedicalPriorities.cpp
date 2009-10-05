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
#include "game_asn/ASN_Delete.h"
#include "MIL_MissionParameter_ABC.h"

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
bool MIL_ParameterType_MedicalPriorities::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.value.t                      = T_MissionParameter_value_logMedicalPriorities;
    to.value.u.logMedicalPriorities = new ASN1T_LogMedicalPriorities();
    to.null_value                   = !from.ToMedicalPriorities( *to.value.u.logMedicalPriorities );
    
    return !to.null_value || bIsOptional;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MedicalPriorities::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_MedicalPriorities::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_logMedicalPriorities );
    assert( to.value.u.logMedicalPriorities );
    ASN_Delete::Delete( *to.value.u.logMedicalPriorities );
    delete to.value.u.logMedicalPriorities;    
}
