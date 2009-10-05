// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ParameterType_GenObject.h"

#include "Network/NET_ASN_Tools.h"
#include "MIL_MissionParameter_ABC.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_GenObject constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_GenObject::MIL_ParameterType_GenObject()
    : MIL_ParameterType_ABC( "GenObject" )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_GenObject destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_GenObject::~MIL_ParameterType_GenObject()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_GenObject::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_GenObject::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.null_value               = false;
    to.value.t                  = T_MissionParameter_value_plannedWork;
    to.value.u.plannedWork = new ASN1T_PlannedWork();
    
    return from.ToGenObject( *to.value.u.plannedWork );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_GenObject::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_GenObject::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_plannedWork );
    assert( to.value.u.plannedWork );
    NET_ASN_Tools::Delete( *to.value.u.plannedWork );

    delete to.value.u.plannedWork;    
}
