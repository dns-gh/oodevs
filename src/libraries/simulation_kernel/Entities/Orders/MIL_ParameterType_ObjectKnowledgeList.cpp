// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ParameterType_ObjectKnowledgeList.h"

#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledgeList constructor
// Created: NLD 2006-11-ObjectList
//-----------------------------------------------------------------------------
MIL_ParameterType_ObjectKnowledgeList::MIL_ParameterType_ObjectKnowledgeList()
    : MIL_ParameterType_ABC( "ObjectKnowledgeList" )
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
void MIL_ParameterType_ObjectKnowledgeList::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const
{
    // Check source
    if( from.value.t != T_MissionParameter_value_listKnowledgeObject ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeListeConnaissancesObjet( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyObjectKnowledgeList( *from.value.u.listKnowledgeObject, to, knowledgeResolver ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledgeList::Copy
// Created: NLD 2006-11-ObjectList
//-----------------------------------------------------------------------------
bool MIL_ParameterType_ObjectKnowledgeList::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeListeConnaissancesObjet( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeListeConnaissancesObjet( to ) )
        return false;

    NET_ASN_Tools::CopyObjectKnowledgeList( from, to );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledgeList::Copy
// Created: NLD 2006-11-ObjectList
//-----------------------------------------------------------------------------
bool MIL_ParameterType_ObjectKnowledgeList::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeListeConnaissancesObjet( from ) )
        return false;

    to.null_value                  = false;
    to.value.t                     = T_MissionParameter_value_listKnowledgeObject;
    to.value.u.listKnowledgeObject = new ASN1T_ListKnowledgeObject();
    
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( from, *to.value.u.listKnowledgeObject, knowledgeResolver ) )
        return false;

    return true;    
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledgeList::CleanAfterSerialization
// Created: NLD 2006-11-ObjectList
//-----------------------------------------------------------------------------
void MIL_ParameterType_ObjectKnowledgeList::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_listKnowledgeObject );
    assert( to.value.u.listKnowledgeObject );
    NET_ASN_Tools::Delete( *to.value.u.listKnowledgeObject );
    delete to.value.u.listKnowledgeObject;
}
