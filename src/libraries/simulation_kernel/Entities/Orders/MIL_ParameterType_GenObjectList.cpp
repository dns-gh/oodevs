// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_GenObjectList.h"
#include "Network/NET_ASN_Tools.h"
#include "MIL_MissionParameter_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GenObjectList constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_GenObjectList::MIL_ParameterType_GenObjectList()
    : MIL_ParameterType_ABC( "GenObjectList" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GenObjectList destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_GenObjectList::~MIL_ParameterType_GenObjectList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GenObjectList::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_GenObjectList::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.null_value                   = false;
    to.value.t                      = T_MissionParameter_value_plannedWorkList;
    to.value.u.plannedWorkList = new ASN1T_PlannedWorkList();
    
    return from.ToGenObjectList( *to.value.u.plannedWorkList );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GenObjectList::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_GenObjectList::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_plannedWorkList );
    assert( to.value.u.plannedWorkList );
    NET_ASN_Tools::Delete( *to.value.u.plannedWorkList );
    delete to.value.u.plannedWorkList;
}
