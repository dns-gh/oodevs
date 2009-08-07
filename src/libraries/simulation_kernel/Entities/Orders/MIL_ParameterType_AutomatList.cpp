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
#include "MIL_MissionParameter_ABC.h"

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
bool MIL_ParameterType_AutomatList::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.null_value           = false;
    to.value.t              = T_MissionParameter_value_automatList;
    to.value.u.automatList = new ASN1T_AutomatList();
    
    return from.ToAutomatList( *to.value.u.automatList );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AutomatList::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_AutomatList::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_automatList );
    assert( to.value.u.automatList );
    NET_ASN_Tools::Delete( *to.value.u.automatList );
    delete to.value.u.automatList;
}
