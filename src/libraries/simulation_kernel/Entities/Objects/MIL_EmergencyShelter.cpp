// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_EmergencyShelter.h"
#include "MIL_RealObjectType.h"
#include "Tools/MIL_Tools.h"

BOOST_CLASS_EXPORT_GUID( MIL_EmergencyShelter, "MIL_EmergencyShelter" )

// -----------------------------------------------------------------------------
// Name: MIL_EmergencyShelter constructor
// Created: JCR 2007-09-19
// -----------------------------------------------------------------------------
MIL_EmergencyShelter::MIL_EmergencyShelter( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_RealObject_ABC( type, nID, army )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_EmergencyShelter constructor
// Created: JCR 2007-09-19
// -----------------------------------------------------------------------------
MIL_EmergencyShelter::MIL_EmergencyShelter()
    : MIL_RealObject_ABC()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_EmergencyShelter destructor
// Created: JCR 2007-09-19
// -----------------------------------------------------------------------------
MIL_EmergencyShelter::~MIL_EmergencyShelter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template < typename Archive > void MIL_EmergencyShelter::serialize
// Created: JCR 2007-09-19
// -----------------------------------------------------------------------------
template < typename Archive > 
void MIL_EmergencyShelter::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}
    

// -----------------------------------------------------------------------------
// Name: MIL_EmergencyShelter::Create
// Created: JCR 2007-09-19
// -----------------------------------------------------------------------------
MIL_RealObject_ABC& MIL_EmergencyShelter::Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
{
    return *new MIL_EmergencyShelter( type, nID, army );
}
