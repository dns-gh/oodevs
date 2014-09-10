// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MaterialAttribute.h"
#include "MIL_Object_ABC.h"
#include "Urban/PHY_MaterialCompositionType.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MaterialAttribute )

// -----------------------------------------------------------------------------
// Name: MaterialAttribute constructor
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
MaterialAttribute::MaterialAttribute()
    : material_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaterialAttribute constructor
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
MaterialAttribute::MaterialAttribute( const PHY_MaterialCompositionType& material )
    : material_( &material )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaterialAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
MaterialAttribute::~MaterialAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaterialAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
MaterialAttribute& MaterialAttribute::operator=( const MaterialAttribute& rhs )
{
    material_ = rhs.material_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MaterialAttribute::load
// Created: JSR 2011-02-16
// -----------------------------------------------------------------------------
void MaterialAttribute::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    std::string name;
    file >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file >> name;
    material_ = PHY_MaterialCompositionType::Find( name );
}

// -----------------------------------------------------------------------------
// Name: MaterialAttribute::save
// Created: JSR 2011-02-16
// -----------------------------------------------------------------------------
void MaterialAttribute::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file << material_->GetName();
}

// -----------------------------------------------------------------------------
// Name: MaterialAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void MaterialAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< MaterialAttribute, MaterialAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: MaterialAttribute::GetMaterial
// Created: SLG 2010-07-06
// -----------------------------------------------------------------------------
const PHY_MaterialCompositionType& MaterialAttribute::GetMaterial() const
{
    return *material_;
}
