// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_PolygonList.h"
#include "game_asn/ASN_Delete.h"
#include "MIL_MissionParameter_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_PolygonList constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_PolygonList::MIL_ParameterType_PolygonList()
    : MIL_ParameterType_ABC( "PolygonList" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_PolygonList destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_PolygonList::~MIL_ParameterType_PolygonList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_PolygonList::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_PolygonList::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.null_value          = false;
    to.value.t             = T_MissionParameter_value_polygonList;
    to.value.u.polygonList = new ASN1T_PolygonList();
    
    return from.ToPolygonList( *to.value.u.polygonList );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_PolygonList::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_PolygonList::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_polygonList );
    assert( to.value.u.polygonList );
    ASN_Delete::Delete( *to.value.u.polygonList );
    delete to.value.u.polygonList;    
}
