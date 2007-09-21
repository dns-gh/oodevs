// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Fire.h"
#include "MIL_RealObjectType.h"
#include "Tools/MIL_Tools.h"

BOOST_CLASS_EXPORT_GUID( MIL_Fire, "MIL_Fire" )

// -----------------------------------------------------------------------------
// Name: MIL_Fire constructor
// Created: JCR 2007-09-19
// -----------------------------------------------------------------------------
MIL_Fire::MIL_Fire( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_RealObject_ABC( type, nID, army )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_Fire constructor
// Created: JCR 2007-09-19
// -----------------------------------------------------------------------------
MIL_Fire::MIL_Fire()
    : MIL_RealObject_ABC()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_Fire destructor
// Created: JCR 2007-09-19
// -----------------------------------------------------------------------------
MIL_Fire::~MIL_Fire()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: template< typename Archive > void MIL_Fire::serialize
// Created: JCR 2007-09-19
// -----------------------------------------------------------------------------
template< typename Archive > void MIL_Fire::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Fire::Create
// Created: JCR 2007-09-19
// -----------------------------------------------------------------------------
MIL_RealObject_ABC& MIL_Fire::Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
{
    return *new MIL_Fire( type, nID, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_Fire::ProcessAgentInside
// Created: JCR 2007-09-19
// -----------------------------------------------------------------------------
void MIL_Fire::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentInside( agent );
}


// -----------------------------------------------------------------------------
// Name: MIL_Fire::ProcessAgentEntering
// Created: JCR 2007-09-19
// -----------------------------------------------------------------------------
void MIL_Fire::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentEntering( agent );
}