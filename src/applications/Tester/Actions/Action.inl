// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-05 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

namespace TEST {

// -----------------------------------------------------------------------------
// Name: Action constructor
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
template< typename T >
Action< T >::Action( uint nExecutionTick /* = 0 */)
    : Action_ABC ( nExecutionTick )
    , pTarget_   ( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Action constructor
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
template< typename T >
Action< T >::Action( T& target, uint nExecutionTick /* = 0 */)
    : Action_ABC ( nExecutionTick )
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
bool Action< T >::Run()
{
    if( CheckParameters() )
    {
        Serialize();
        Send     ();
        Clean    ();
        return true;
    }
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