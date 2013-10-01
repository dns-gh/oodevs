// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MedicalCapacity.h"
#include "Object.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MedicalCapacity )

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::MedicalCapacity
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
MedicalCapacity::MedicalCapacity( xml::xistream& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::MedicalCapacity
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
MedicalCapacity::MedicalCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity constructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
MedicalCapacity::MedicalCapacity( const MedicalCapacity& /*from*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity destructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
MedicalCapacity::~MedicalCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::serialize
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void MedicalCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::Register
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void MedicalCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::Instanciate
// Created: RFT 2008-06-08
// -----------------------------------------------------------------------------
void MedicalCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new MedicalCapacity( *this ) );
}
