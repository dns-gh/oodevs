// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "CompositeRegistrable.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: CompositeRegistrable constructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
CompositeRegistrable::CompositeRegistrable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompositeRegistrable destructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
CompositeRegistrable::~CompositeRegistrable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompositeRegistrable::Add
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void CompositeRegistrable::Add( Registrable_ABC* r )
{
    registrables_.push_back( r );
}

// -----------------------------------------------------------------------------
// Name: CompositeRegistrable::RegisterIn
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void CompositeRegistrable::RegisterIn( directia::brain::Brain& brain )
{
    std::for_each( registrables_.begin(), registrables_.end(), boost::bind( &Registrable_ABC::RegisterIn, _1, boost::ref( brain ) ) );
}
