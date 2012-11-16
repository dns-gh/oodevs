// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "LogisticCapacity.h"
#include "MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( LogisticCapacity )

// -----------------------------------------------------------------------------
// Name: LogisticCapacity constructor
// Created: MMC 2012-11-16
// -----------------------------------------------------------------------------
LogisticCapacity::LogisticCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticCapacity constructor
// Created: MMC 2012-11-16
// -----------------------------------------------------------------------------
LogisticCapacity::LogisticCapacity( const LogisticCapacity& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticCapacity constructor
// Created: MMC 2012-11-16
// -----------------------------------------------------------------------------
LogisticCapacity::LogisticCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticCapacity destructor
// Created: MMC 2012-11-16
// -----------------------------------------------------------------------------
LogisticCapacity::~LogisticCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticCapacity::serialize
// Created: MMC 2012-11-16
// -----------------------------------------------------------------------------
template< typename Archive >
void LogisticCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticCapacity::Instanciate
// Created: MMC 2012-11-16
// -----------------------------------------------------------------------------
void LogisticCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    LogisticCapacity* capacity = new LogisticCapacity( *this );
    object.AddCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: LogisticCapacity::Register
// Created: MMC 2012-11-16
// -----------------------------------------------------------------------------
void LogisticCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}
