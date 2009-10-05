// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_ObjectKnowledge.h"
#include "MIL_MissionParameter_ABC.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledge constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_ObjectKnowledge::MIL_ParameterType_ObjectKnowledge()
    : MIL_ParameterType_ABC( "ObjectKnowledge" )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledge destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_ObjectKnowledge::~MIL_ParameterType_ObjectKnowledge()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledge::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_ObjectKnowledge::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.value.t    = T_MissionParameter_value_objectKnowledge;
    to.null_value = !from.ToObjectKnowledge( to.value.u.objectKnowledge );
    
    return !to.null_value || bIsOptional;
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledge::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_ObjectKnowledge::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_objectKnowledge );
}
