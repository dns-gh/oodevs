// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionWithLocation.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionWithLocation constructor
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
PHY_PerceptionWithLocation<T>::PHY_PerceptionWithLocation( PHY_RoleInterface_Perceiver& perceiver )
    : PHY_Perception_ABC( perceiver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionWithLocation destructor
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
PHY_PerceptionWithLocation<T>::~PHY_PerceptionWithLocation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionWithLocation::Remove
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
void PHY_PerceptionWithLocation<T>::Remove( int id )
{
    for( auto it = recos_.begin(); it != recos_.end(); ++it )
    {    
        if ( it->Id() == id )
        {
            recos_.erase( it );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionWithLocation::AddLocation
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
int PHY_PerceptionWithLocation<T>::Add( T* pLocation )
{
    recos_.push_back( pLocation );
    return pLocation->Id();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionWithLocation::FinalizePerception
// Created: LDC 2010-05-05
// -----------------------------------------------------------------------------
template< class T >
void PHY_PerceptionWithLocation<T>::FinalizePerception()
{
    // NOTHING
}
