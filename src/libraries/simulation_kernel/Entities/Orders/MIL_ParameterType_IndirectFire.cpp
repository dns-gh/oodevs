// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_IndirectFire.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_IndirectFire constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_IndirectFire::MIL_ParameterType_IndirectFire()
: MIL_ParameterType_ABC( "IndirectFire" )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_IndirectFire destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_IndirectFire::~MIL_ParameterType_IndirectFire()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_IndirectFire::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_IndirectFire::Copy( const ASN1T_MissionParameter& /*from*/, DIA_Variable_ABC& /*to*/, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_IndirectFire::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_IndirectFire::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*//*knowledgeResolver*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeTirIndirect( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeTirIndirect( to ) )
        return false;

    NET_ASN_Tools::CopyIndirectFire( from, to );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_IndirectFire::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_IndirectFire::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeTirIndirect( from ) )
        return false;

    to.null_value = false;
    to.value.t    = T_MissionParameter_value_tirIndirect;
    
    if( !NET_ASN_Tools::CopyIndirectFire( from, to.value.u.tirIndirect ) )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_IndirectFire::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_IndirectFire::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_tirIndirect );
}
