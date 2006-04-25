// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Scheduler.h"

namespace TEST {

// -----------------------------------------------------------------------------
// Name: Action constructor
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
template< typename T >
Action< T >::Action()
    : Action_ABC ()
    , pTarget_   ( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Action constructor
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
template< typename T >
Action< T >::Action( T& target)
    : Action_ABC ()
    , pTarget_   ( &target )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Action destructor
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
template< typename T >
Action< T >::~Action()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Action_Magic_Move::Run
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
template< typename T >
inline
bool Action< T >::Run( const Scheduler& scheduler )
{
    if( CheckParameters() )
    {
        Serialize();
        Send     ( scheduler );
        Clean    ();
        return true;
    }
    MT_LOG_ERROR_MSG( "Invalid parameters" );
    return false;
}

// -----------------------------------------------------------------------------
// Name: Action::SetTarget
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
template< typename T >
inline
void Action< T >::SetTarget( T& target )
{
    pTarget_ = &target;
}

}