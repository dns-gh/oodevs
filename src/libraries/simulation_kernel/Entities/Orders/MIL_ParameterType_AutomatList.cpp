// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ParameterType_AutomatList.h"

#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AutomatList constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_AutomatList::MIL_ParameterType_AutomatList()
    : MIL_ParameterType_ABC( "AutomateList" )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AutomatList destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_AutomatList::~MIL_ParameterType_AutomatList()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AutomatList::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_AutomatList::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.null_value || from.value.t != T_MissionParameter_value_listAutomate ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeListeAutomates( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyAutomateList( *from.value.u.listAutomate, to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AutomatList::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_AutomatList::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeListeAutomates( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeListeAutomates( to ) )
        return false;

    if( !NET_ASN_Tools::CopyAutomateList( from, to ) )
        return false;
    
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AutomatList::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_AutomatList::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeListeAutomates( from ) )
        return false;

    to.null_value           = false;
    to.value.t              = T_MissionParameter_value_listAutomate;
    to.value.u.listAutomate = new ASN1T_ListAutomate();
    
    if( !NET_ASN_Tools::CopyAutomateList( from, *to.value.u.listAutomate ) )
        return false;

    return true;    
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AutomatList::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_AutomatList::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_listAutomate );
    assert( to.value.u.listAutomate );
    NET_ASN_Tools::Delete( *to.value.u.listAutomate );
    delete to.value.u.listAutomate;
}
