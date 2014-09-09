// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "BorderCapacity.h"
#include "MIL_Object_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( BorderCapacity )

// -----------------------------------------------------------------------------
// Name: BorderCapacity constructor
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
BorderCapacity::BorderCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BorderCapacity constructor
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
BorderCapacity::BorderCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BorderCapacity destructor
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
BorderCapacity::~BorderCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BorderCapacity constructor
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
BorderCapacity::BorderCapacity( const BorderCapacity& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BorderCapacity::serialize
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
template< typename Archive >
void BorderCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
    // $$$ RC LDC 2012-03-07 Quid populationsInside_ populationsNotified_;
}

// -----------------------------------------------------------------------------
// Name: BorderCapacity::Instanciate
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
void BorderCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    BorderCapacity* capacity = new BorderCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: BorderCapacity::Update
// Created: JSR 2011-09-16
// -----------------------------------------------------------------------------
void BorderCapacity::Update( MIL_Object_ABC& /*object*/, unsigned int /*time*/ )
{
    tools::Set< MIL_Population* > toErase;
    for( auto it = populationsNotified_.begin(); it != populationsNotified_.end(); ++it )
        if( populationsInside_.find( *it ) == populationsInside_.end() )
            toErase.insert( *it );
    for( auto it = toErase.begin(); it != toErase.end(); ++it )
        populationsNotified_.erase( *it );
    for( auto it = populationsInside_.begin(); it != populationsInside_.end(); ++it )
        if( populationsNotified_.find( *it ) == populationsNotified_.end() )
        {
            MIL_Report::PostEvent( **it, report::eRC_BorderCrossed );
            populationsNotified_.insert( *it );
        }
    populationsInside_.clear();
}

// -----------------------------------------------------------------------------
// Name: BorderCapacity::Register
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
void BorderCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: BorderCapacity::ProcessAgentEntering
// Created: JSR 2011-09-02
// -----------------------------------------------------------------------------
void BorderCapacity::ProcessAgentEntering( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& agent )
{
    MIL_Report::PostEvent( agent, report::eRC_BorderCrossed );
}

// -----------------------------------------------------------------------------
// Name: BorderCapacity::ProcessPopulationInside
// Created: JSR 2011-09-01
// -----------------------------------------------------------------------------
void BorderCapacity::ProcessPopulationInside( MIL_Object_ABC& /*object*/, MIL_PopulationElement_ABC& population )
{
    populationsInside_.insert( &population.GetPopulation() );
}
