// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ParameterType_Point.h"

#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"
#include "MIL_MissionParameter_ABC.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Point constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_Point::MIL_ParameterType_Point()
: MIL_ParameterType_ABC( "Point" )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Point destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_Point::~MIL_ParameterType_Point()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Point::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_Point::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.value.t       = T_MissionParameter_value_point;   
    to.value.u.point = new ASN1T_Point();
    to.null_value    = !from.ToPoint( *to.value.u.point );
    return ( bIsOptional || !to.null_value );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Point::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_Point::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_point );
    assert( to.value.u.point );
    if( !to.null_value )
        NET_ASN_Tools::Delete( *to.value.u.point );
    delete to.value.u.point;
}
