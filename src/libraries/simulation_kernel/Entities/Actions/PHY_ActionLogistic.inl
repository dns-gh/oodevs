// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_ActionLogistic constructor
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
template< typename T >
PHY_ActionLogistic< T >::PHY_ActionLogistic( T& entity )
    : PHY_Action_ABC()
    , entity_( entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLogistic destructor
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
template< typename T >
PHY_ActionLogistic< T >::~PHY_ActionLogistic()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLogistic::Execute
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
template< typename T >
void PHY_ActionLogistic< T >::Execute()
{
    entity_.UpdateLogistic();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLogistic::ExecuteSuspended
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
template< typename T >
void PHY_ActionLogistic< T >::ExecuteSuspended()
{
    Execute();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLogistic::Stop
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
template< typename T >
void PHY_ActionLogistic< T >::Stop()
{
    // NOTHING
}
