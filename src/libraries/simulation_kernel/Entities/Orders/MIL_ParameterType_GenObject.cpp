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

#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

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
void MIL_ParameterType_GenObject::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.null_value || from.value.t != T_MissionParameter_value_plannedWork ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeGenObjet( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    NET_ASN_Tools::CopyGenObject( *from.value.u.plannedWork, to );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_GenObject::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_GenObject::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeGenObjet( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeGenObjet( to ) )
        return false;

    NET_ASN_Tools::CopyGenObject( from, to );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_GenObject::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_GenObject::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeGenObjet( from ) )
        return false;

    to.null_value               = false;
    to.value.t                  = T_MissionParameter_value_plannedWork;
    to.value.u.plannedWork = new ASN1T_PlannedWork();
    
    if( !NET_ASN_Tools::CopyGenObject( from, *to.value.u.plannedWork ) )
        return false;

    return true;    
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
