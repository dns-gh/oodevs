// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PropagationCapacity.h"
#include "MIL_Object_ABC.h"
#include "Adapters/Sink_ABC.h"
#include <core/Model.h>

using namespace sword::capacity;

BOOST_CLASS_EXPORT_IMPLEMENT( sword::capacity::PropagationCapacity )

// -----------------------------------------------------------------------------
// Name: PropagationCapacity constructor
// Created: LGY 2012-09-21
// -----------------------------------------------------------------------------
PropagationCapacity::PropagationCapacity()
    : pSink_  ( 0 )
    , command_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropagationCapacity constructor
// Created: LGY 2012-09-19
// -----------------------------------------------------------------------------
PropagationCapacity::PropagationCapacity( xml::xistream& /*xis*/ )
    : pSink_  ( 0 )
    , command_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropagationCapacity constructor
// Created: LGY 2012-09-21
// -----------------------------------------------------------------------------
PropagationCapacity::PropagationCapacity( sword::Sink_ABC& sink )
    : pSink_  ( &sink )
    , command_( 0 )
{
    core::Model parameters;
    command_ = pSink_->StartCommand( "propagation command", parameters );
}

// -----------------------------------------------------------------------------
// Name: PropagationCapacity destructor
// Created: LGY 2012-09-19
// -----------------------------------------------------------------------------
PropagationCapacity::~PropagationCapacity()
{
    if( pSink_ )
        pSink_->StopCommand( command_ );
}

// -----------------------------------------------------------------------------
// Name: PropagationCapacity::Register
// Created: LGY 2012-09-20
// -----------------------------------------------------------------------------
void PropagationCapacity::Register( MIL_Object_ABC& /*object*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropagationCapacity::Instanciate
// Created: LGY 2012-09-20
// -----------------------------------------------------------------------------
void PropagationCapacity::Instanciate( MIL_Object_ABC& /*object*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropagationCapacity::Instanciate
// Created: LGY 2012-09-21
// -----------------------------------------------------------------------------
void PropagationCapacity::Instanciate( MIL_Object_ABC& object, sword::Sink_ABC& sink ) const
{
    object.AddCapacity( new PropagationCapacity( sink ) );
}

// -----------------------------------------------------------------------------
// Name: PropagationCapacity::serialize
// Created: LGY 2012-09-21
// -----------------------------------------------------------------------------
template< typename Archive >
void PropagationCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & pSink_;
}
