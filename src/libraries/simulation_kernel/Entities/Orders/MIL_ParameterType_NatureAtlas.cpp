// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_NatureAtlas.h"
#include "MIL_MissionParameter_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_NatureAtlas constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_NatureAtlas::MIL_ParameterType_NatureAtlas()
    : MIL_ParameterType_ABC( "NatureAtlas" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_NatureAtlas destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_NatureAtlas::~MIL_ParameterType_NatureAtlas()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_NatureAtlas::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_NatureAtlas::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.null_value          = false;
    to.value.t             = T_MissionParameter_value_atlasNature;
    to.value.u.atlasNature = new ASN1T_AtlasNature();
    
    return from.ToNatureAtlas( *to.value.u.atlasNature );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_NatureAtlas::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_NatureAtlas::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_atlasNature );
    assert( to.value.u.atlasNature );
    delete to.value.u.atlasNature;
}
