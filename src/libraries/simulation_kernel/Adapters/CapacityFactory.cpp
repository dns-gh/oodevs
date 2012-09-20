// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "CapacityFactory.h"
#include "Entities/Objects/CapacityFactory.h"
#include "Entities/Objects/ObjectPrototype.h"
#include "Entities/Objects/PropagationCapacity.h"

using namespace sword;

namespace
{
    template< typename T >
    struct AddBuilder
    {
        static void Add( ObjectPrototype& prototype, xml::xistream& xis )
        {
            prototype.AddCapacity( new T( xis ) );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory constructor
// Created: LGY 2012-09-19
// -----------------------------------------------------------------------------
CapacityFactory::CapacityFactory()
{
    DoRegister( "disaster", boost::bind( &AddBuilder< sword::capacity::PropagationCapacity >::Add, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory destructor
// Created: LGY 2012-09-19
// -----------------------------------------------------------------------------
CapacityFactory::~CapacityFactory()
{
    // NOTHING
}
