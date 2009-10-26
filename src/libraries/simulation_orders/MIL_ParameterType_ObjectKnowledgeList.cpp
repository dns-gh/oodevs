// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_ObjectKnowledgeList.h"
#include "game_asn/ASN_Delete.h"
#include "MIL_MissionParameter_ABC.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledgeList constructor
// Created: NLD 2006-11-ObjectList
//-----------------------------------------------------------------------------
MIL_ParameterType_ObjectKnowledgeList::MIL_ParameterType_ObjectKnowledgeList( const std::string& name )
    : MIL_ParameterType_ABC( name )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledgeList destructor
// Created: NLD 2006-11-ObjectList
//-----------------------------------------------------------------------------
MIL_ParameterType_ObjectKnowledgeList::~MIL_ParameterType_ObjectKnowledgeList()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledgeList::Copy
// Created: NLD 2006-11-ObjectList
//-----------------------------------------------------------------------------
bool MIL_ParameterType_ObjectKnowledgeList::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.null_value                  = false;
    to.value.t                     = T_MissionParameter_value_objectKnowledgeList;
    to.value.u.objectKnowledgeList = new ASN1T_ObjectKnowledgeList();
    
    return from.ToObjectKnowledgeList( *to.value.u.objectKnowledgeList );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledgeList::CleanAfterSerialization
// Created: NLD 2006-11-ObjectList
//-----------------------------------------------------------------------------
void MIL_ParameterType_ObjectKnowledgeList::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_objectKnowledgeList );
    assert( to.value.u.objectKnowledgeList );
    ASN_Delete::Delete( *to.value.u.objectKnowledgeList );
    delete to.value.u.objectKnowledgeList;
}
