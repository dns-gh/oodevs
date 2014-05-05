// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AgentAdapter.h"
#include "PlatformProxy.h"

#include "clients_kernel/CoordinateConverter_ABC.h"
#include "tic/PlatformDelegate_ABC.h"
#include "tic/PlatformDelegateFactory_ABC.h"
#include "tic/PlatformVisitor_ABC.h"
#include "tic/Platform_ABC.h"
#include "rpr/Enums.h"

#include "dispatcher/Observer.h"
#include "dispatcher/Agent_ABC.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AgentAdapter constructor
// Created: AHC 2012-07-27
// -----------------------------------------------------------------------------
AgentAdapter::AgentAdapter( tic::PlatformDelegateFactory_ABC& factory, const kernel::CoordinateConverter_ABC& converter,
        dispatcher::Agent_ABC& agent, T_NotificationCallback cb )
    : dispatcher::Observer< sword::UnitAttributes >( agent )
    , converter_( converter )
    , agent_( agent )
    , callback_( cb )
    , childIndex_( 0 )
    , platforms_( std::move( factory.Create( agent ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentAdapter::Notify
// Created: AHC 2012-07-27
// -----------------------------------------------------------------------------
void AgentAdapter::Notify( const sword::UnitAttributes& )
{
    platforms_->Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentAdapter::AddPlatform
// Created: AHC 2012-07-27
// -----------------------------------------------------------------------------
void AgentAdapter::AddPlatform( const tic::Platform_ABC& platform )
{
    T_Proxies::iterator it( proxies_.find( &platform ) );
    if( proxies_.end() == it )
    {
        it = proxies_.insert( proxies_.end(), T_Proxies::value_type( &platform,  boost::shared_ptr< PlatformProxy >( new PlatformProxy ) ) );
        callback_( *(it->second), agent_, platform, childIndex_ );
        ++childIndex_;
    }
    PlatformProxy& proxy( *it->second );
    const geometry::Point2d geocoord = converter_.ConvertToGeo( platform.GetPosition() );
    proxy.SpatialChanged( geocoord.Y(), geocoord.X(), platform.GetAltitude(), platform.GetSpeed(), platform.GetHeading() );
    switch( platform.GetState() )
    {
    case tic::Platform_ABC::okay:
        proxy.StateChanged( rpr::damageState_NoDamage );
        break;
    case tic::Platform_ABC::broken:
        proxy.StateChanged( rpr::damageState_ModerateDamage );
        break;
    case tic::Platform_ABC::destroyed:
        proxy.StateChanged( rpr::damageState_Destroyed );
        break;
    }
    proxy.MountedChanged( platform.IsMounted() );
}
