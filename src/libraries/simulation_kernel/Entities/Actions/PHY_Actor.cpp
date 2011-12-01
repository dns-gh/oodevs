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
#include "MT_Tools/MT_Logger.h"

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
    for( std::map< unsigned int, boost::shared_ptr< PHY_Action_ABC > >::const_iterator it = actions_.begin(); it != actions_.end(); ++it )
    {
        try
        {
            (*it).second->Update();

        }
        catch( std::runtime_error& e )
        {
            MT_LOG_ERROR_MSG( "Action failure : " << e.what() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Actor::RegisterAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
void PHY_Actor::RegisterAction( boost::shared_ptr< PHY_Action_ABC > action )
{
    actions_.insert( std::pair< unsigned int, boost::shared_ptr< PHY_Action_ABC > >( action->GetId(), action ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_Actor::UnregisterAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
void PHY_Actor::UnregisterAction( unsigned int actionId )
{
    std::map< unsigned int, boost::shared_ptr< PHY_Action_ABC > >::iterator it = actions_.find( actionId );
    if( it != actions_.end() )
    {
        it->second->Stop();
        actions_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Actor::CancelAllActions
// Created: NLD 2005-03-18
// -----------------------------------------------------------------------------
void PHY_Actor::CancelAllActions()
{
    for( std::map< unsigned int, boost::shared_ptr< PHY_Action_ABC > >::const_iterator it = actions_.begin(); it != actions_.end(); ++it )
        it->second->Stop();
    actions_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_Actor::HasAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
bool PHY_Actor::HasAction( unsigned int actionId ) const
{
    return actions_.find( actionId ) != actions_.end();
}

// -----------------------------------------------------------------------------
// Name: PHY_Actor::HasAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
boost::shared_ptr< PHY_Action_ABC > PHY_Actor::GetAction( unsigned int actionId ) const
{
    std::map< unsigned int, boost::shared_ptr< PHY_Action_ABC > >::const_iterator it = actions_.find( actionId );
    if( it != actions_.end() )
        return it->second;
    return boost::shared_ptr< PHY_Action_ABC >();
}