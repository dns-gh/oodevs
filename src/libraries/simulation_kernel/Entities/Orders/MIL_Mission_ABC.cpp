// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_Mission_ABC.h"

#include "MIL_MissionType_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver )
    : DIA_Thing         ( DIA_Thing::ThingType(), type.GetDIAType() )
    , type_             ( type )
    , context_          ()
    , knowledgeResolver_( knowledgeResolver )
{    
    // No parameters $$$
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const ASN1T_MissionParameters& parameters )
    : DIA_Thing         ( DIA_Thing::ThingType(), type.GetDIAType() )
    , type_             ( type )
    , context_          ()
    , knowledgeResolver_( knowledgeResolver )
{
    type_.Copy( parameters, *this, knowledgeResolver_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const ASN1T_MissionParameters& parameters, const ASN1T_OrderContext& context, const MT_Vector2D& refPosition )
    : DIA_Thing         ( DIA_Thing::ThingType(), type.GetDIAType() )
    , type_             ( type )
    , context_          ( context, refPosition )
    , knowledgeResolver_( knowledgeResolver )
{
    type_.Copy( parameters, *this, knowledgeResolver_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_Mission_ABC& parent )
    : DIA_Thing         ( DIA_Thing::ThingType(), type.GetDIAType() )
    , type_             ( type )
    , context_          ( parent.context_ )
    , knowledgeResolver_( knowledgeResolver )
{
    // Parameters will be filled by DIA $$$
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_Mission_ABC& rhs )
    : DIA_Thing         ( DIA_Thing::ThingType(), rhs.type_.GetDIAType() )
    , type_             ( rhs.type_ )
    , context_          ( rhs.context_ )
    , knowledgeResolver_( knowledgeResolver )
{
    type_.Copy( rhs, *this, knowledgeResolver_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC destructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_Mission_ABC::~MIL_Mission_ABC()
{
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetName
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
const std::string& MIL_Mission_ABC::GetName() const
{
    return type_.GetName();
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::Serialize
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::Serialize( ASN1T_OrderContext& asn ) const
{
    context_.Serialize( asn );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::CleanAfterSerialization
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::CleanAfterSerialization( ASN1T_OrderContext& asn ) const
{
    context_.CleanAfterSerialization( asn );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::Serialize
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::Serialize( ASN1T_MissionParameters& asn ) const
{
    type_.Copy( *this, asn, knowledgeResolver_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::CleanAfterSerialization
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::CleanAfterSerialization( ASN1T_MissionParameters& asn ) const
{
    type_.CleanAfterSerialization( asn );
}
