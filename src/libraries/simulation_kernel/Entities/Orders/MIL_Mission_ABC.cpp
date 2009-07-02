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
    , context_          ( true ) // $$$$ SBO 2008-12-11: Context must be present!
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
    type_.Copy( parameters, *this, knowledgeResolver_, context_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const ASN1T_MissionParameters& parameters, const MT_Vector2D& refPosition )
    : DIA_Thing         ( DIA_Thing::ThingType(), type.GetDIAType() )
    , type_             ( type )
    , context_          ( parameters, refPosition )
    , knowledgeResolver_( knowledgeResolver )
{
    type_.Copy( parameters, *this, knowledgeResolver_, context_ );
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetName
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
const std::string& MIL_Mission_ABC::GetName() const
{
    return type_.GetName();
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::Serialize
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::Serialize( ASN1T_MissionParameters& asn ) const
{
    type_.Copy( *this, asn, knowledgeResolver_, context_ );
    context_.Serialize( asn );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::CleanAfterSerialization
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::CleanAfterSerialization( ASN1T_MissionParameters& asn ) const
{
    context_.CleanAfterSerialization( asn );
    type_.CleanAfterSerialization( asn, context_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::Accept
// Created: SBO 2007-11-13
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::Accept( MIL_IntelligenceOrdersVisitor_ABC& visitor ) const
{
    context_.Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetType
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
const MIL_MissionType_ABC& MIL_Mission_ABC::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetDirDanger
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
const MT_Vector2D& MIL_Mission_ABC::GetDirDanger() const
{
    return context_.GetDirDanger();
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetFuseau
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
const MIL_Fuseau& MIL_Mission_ABC::GetFuseau() const
{
    return context_.GetFuseau();
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::AffectFuseau
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::AffectFuseau( const MIL_Fuseau& fuseau )
{
    context_.AffectFuseau( fuseau );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::AffectDirection
// Created: SBO 2008-01-04
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::AffectDirection( const MT_Vector2D& direction )
{
    context_.AffectDirection( direction );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetLimas
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
const T_LimaVector& MIL_Mission_ABC::GetLimas() const
{
    return context_.GetLimas();
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::FindLima
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_Mission_ABC::FindLima( uint nID )
{
    return context_.FindLima( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::FindLima
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_Mission_ABC::FindLima( const MIL_LimaFunction& function )
{
    return context_.FindLima( function );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::FindNextScheduledLima
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_Mission_ABC::FindNextScheduledLima()
{
    return context_.FindNextScheduledLima();
}
