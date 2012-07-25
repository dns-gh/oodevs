// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "PerceptionWithLocation.h"

namespace sword
{
namespace perception
{
// -----------------------------------------------------------------------------
// Name: PerceptionWithLocation constructor
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
template< typename T >
PerceptionWithLocation< T >::PerceptionWithLocation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionWithLocation destructor
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
template< typename T >
PerceptionWithLocation< T >::~PerceptionWithLocation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionWithLocation::AddLocation
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
template< typename T >
void PerceptionWithLocation< T >::Add( std::auto_ptr< T > pLocation )
{
    recos_.push_back( T_Reco( pLocation.release() ) );
}

}
}
