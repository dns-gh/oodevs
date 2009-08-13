// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_Polygon.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"
#include "MIL_MissionParameter_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Polygon constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_Polygon::MIL_ParameterType_Polygon()
    : MIL_ParameterType_ABC( "Polygon" )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Polygon destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_Polygon::~MIL_ParameterType_Polygon()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Polygon::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_Polygon::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.value.t         = T_MissionParameter_value_polygon;
    to.value.u.polygon = new ASN1T_Polygon();
    to.null_value      = !from.ToPolygon( *to.value.u.polygon );

    return !to.null_value || bIsOptional;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Polygon::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_Polygon::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_polygon );
    assert( to.value.u.polygon );
    NET_ASN_Tools::Delete( *to.value.u.polygon );
    delete to.value.u.polygon;    
}
