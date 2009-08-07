// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_GDH.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"
#include "MIL_MissionParameter_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GDH constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_GDH::MIL_ParameterType_GDH()
    : MIL_ParameterType_ABC( "datetime" )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GDH destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_GDH::~MIL_ParameterType_GDH()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GDH::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_GDH::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.null_value  = false;
    to.value.t     = T_MissionParameter_value_dateTime;
    to.value.u.dateTime = new ASN1T_DateTime();

    return from.ToGDH( *to.value.u.dateTime );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GDH::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_GDH::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    delete to.value.u.dateTime;
}
