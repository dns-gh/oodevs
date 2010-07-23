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
#include "MIL.h"
#include <urban/MaterialCompositionType.h>
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_IMPLEMENT( MaterialAttribute )

// -----------------------------------------------------------------------------
// Name: MaterialAttribute constructor
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
MaterialAttribute::MaterialAttribute()
: material_ ( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MaterialAttribute constructor
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
MaterialAttribute::MaterialAttribute( urban::MaterialCompositionType& material )
    : material_ ( &material )
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
// Name: template< typename Archive > void MaterialAttribute::serialize
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
template< typename Archive >
void MaterialAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    //file & material_; // TODO
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
// Name: MaterialAttribute::urban::MaterialCompositionType::GetMaterial
// Created: SLG 2010-07-06
// -----------------------------------------------------------------------------
urban::MaterialCompositionType& MaterialAttribute::GetMaterial() const
{
    return *material_;
}
