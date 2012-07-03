// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FloodModelFactory.h"
#include "FloodModel.h"
#include "MIL.h"

using namespace sword;

BOOST_CLASS_EXPORT_IMPLEMENT( sword::FloodModelFactory )

// -----------------------------------------------------------------------------
// Name: FloodModelFactory constructor
// Created: LGY 2012-06-19
// -----------------------------------------------------------------------------
FloodModelFactory::FloodModelFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodModelFactory destructor
// Created: LGY 2012-06-19
// -----------------------------------------------------------------------------
FloodModelFactory::~FloodModelFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodModelFactory::CreateFloodModel
// Created: LGY 2012-06-19
// -----------------------------------------------------------------------------
std::auto_ptr< flood::FloodModel_ABC > FloodModelFactory::CreateFloodModel() const
{
    return std::auto_ptr< flood::FloodModel_ABC >( new FloodModel() );
}

// -----------------------------------------------------------------------------
// Name: FloodModelFactory::load
// Created: LGY 2012-06-20
// -----------------------------------------------------------------------------
void FloodModelFactory::load( MIL_CheckPointInArchive& archive, const unsigned int )
{
    archive >> boost::serialization::base_object< FloodModelFactory_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: FloodModelFactory::save
// Created: LGY 2012-06-20
// -----------------------------------------------------------------------------
void FloodModelFactory::save( MIL_CheckPointOutArchive& archive, const unsigned int ) const
{
    archive << boost::serialization::base_object< FloodModelFactory_ABC >( *this );
}
