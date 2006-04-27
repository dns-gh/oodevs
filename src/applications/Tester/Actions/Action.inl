// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "Scheduler.h"

namespace TEST {

// -----------------------------------------------------------------------------
// Name: Action constructor
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
template< typename T >
Action< T >::Action( const std::string& name )
    : Action_ABC ()
    , pTarget_   ( 0 )
    , name_      ( name )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Action constructor
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
template< typename T >
Action< T >::Action( const std::string& name, T& target)
    : Action_ABC ()
    , pTarget_   ( &target )
    , name_      ( name )
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

// -----------------------------------------------------------------------------
// Name: Action::ToString
// Created: SBO 2006-04-27
// -----------------------------------------------------------------------------
template< typename T >
inline
std::string Action< T >::ToString() const
{
    std::stringstream ss;
    ss << name_ << " on [" << pTarget_->GetId() << "]";
    return ss.str();
}

}