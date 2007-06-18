// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AtlasNature.h"
#include "game_asn/Asn.h"

// -----------------------------------------------------------------------------
// Name: AtlasNature constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AtlasNature::AtlasNature()
    : value_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AtlasNature constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AtlasNature::AtlasNature( const QString& name, unsigned short value )
    : name_( name )
    , value_( value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AtlasNature destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AtlasNature::~AtlasNature()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AtlasNature::GetName
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
QString AtlasNature::GetName() const
{
    return name_.join( "," );
}

// -----------------------------------------------------------------------------
// Name: AtlasNature::GetValue
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
unsigned short AtlasNature::GetValue() const
{
    return value_;
}

// -----------------------------------------------------------------------------
// Name: AtlasNature::IsSet
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
bool AtlasNature::IsSet( unsigned short value ) const
{
    return value_ & value;
}

// -----------------------------------------------------------------------------
// Name: AtlasNature::Toggle
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AtlasNature::Toggle( const AtlasNature& nature )
{
    value_ ^= nature.value_;
    if( name_.find( nature.GetName() ) != name_.end() )
        name_.remove( nature.GetName() );
    else
        name_.append( nature.GetName() );
}

// -----------------------------------------------------------------------------
// Name: AtlasNature::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AtlasNature::CommitTo( ASN1T_AtlasNature& asn ) const
{
    asn.numbits = 11;
    std::memcpy( asn.data, (const unsigned char*)&value_, sizeof( asn.data ) );
}
