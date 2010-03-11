// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/PHY_Actor.cpp $
// $Author: Nld $
// $Modtime: 2/05/05 18:40 $
// $Revision: 42 $
// $Workfile: PHY_Actor.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Actor.h"
#include "PHY_Action_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_Actor constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
PHY_Actor::PHY_Actor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Actor destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
PHY_Actor::~PHY_Actor()
{
    actions_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_Actor::UpdateActions
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_Actor::UpdateActions()
{
    for( std::set< const boost::shared_ptr< PHY_Action_ABC > >::const_iterator it = actions_.begin(); it != actions_.end(); ++it )
        (*it)->Update();
}

// -----------------------------------------------------------------------------
// Name: PHY_Actor::RegisterAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
void PHY_Actor::RegisterAction( const boost::shared_ptr< PHY_Action_ABC > action )
{
    bool bOut = actions_.insert( action ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: PHY_Actor::UnregisterAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
void PHY_Actor::UnregisterAction( const boost::shared_ptr< PHY_Action_ABC > action )
{
    action->Stop();
    int nOut = actions_.erase( action );
    assert( nOut == 1 );
}

// -----------------------------------------------------------------------------
// Name: PHY_Actor::CancelAllActions
// Created: NLD 2005-03-18
// -----------------------------------------------------------------------------
void PHY_Actor::CancelAllActions()
{
    for( std::set< const boost::shared_ptr< PHY_Action_ABC > >::const_iterator it = actions_.begin(); it != actions_.end(); ++it )
        (*it)->Stop();
    actions_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_Actor::HasAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
bool PHY_Actor::HasAction( const boost::shared_ptr< PHY_Action_ABC > action ) const
{
    return actions_.find( action ) != actions_.end();
}
