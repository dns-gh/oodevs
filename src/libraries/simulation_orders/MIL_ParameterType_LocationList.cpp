// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_LocationList.h"
#include "game_asn/ASN_Delete.h"
#include "MIL_MissionParameter_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_LocationList constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_LocationList::MIL_ParameterType_LocationList()
    : MIL_ParameterType_ABC( "LocationList" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_LocationList destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_LocationList::~MIL_ParameterType_LocationList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_LocationList::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_LocationList::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.null_value               = false;
    to.value.t                  = T_MissionParameter_value_locationList;
    to.value.u.locationList = new ASN1T_LocationList();
    
    return from.ToLocationList( *to.value.u.locationList );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_LocationList::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_LocationList::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_locationList );
    assert( to.value.u.locationList );
    ASN_Delete::Delete( *to.value.u.locationList );
    delete to.value.u.locationList;    
}
