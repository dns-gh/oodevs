// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "DisExtension.h"
#include "EntityStatePDU.h"
#include "Time_ABC.h"
#include "IdentifierFactory_ABC.h"
#include "DisTypeResolver.h"
#include "tic_plugin/TicExtension_ABC.h"
#include "tic_plugin/Platform_ABC.h"
#include "tic_plugin/PlatformAdapter.h"
#include "dispatcher/Agent.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/ComponentType.h"
#include "UdpNetwork.h"
#include <geocoord/MGRS.h>
#include <geocoord/Geodetic.h>
#include "DetonationPDU.h"

using namespace plugins::dis;

// -----------------------------------------------------------------------------
// Name: DisExtension constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisExtension::DisExtension( const Time_ABC& time, IdentifierFactory_ABC& id, const kernel::CoordinateConverter_ABC& converter, UdpNetwork& network, const DisTypeResolver& resolver, dispatcher::Agent& holder, unsigned char exercise, bool lagAFrame )
    : time_     ( time )
    , id_       ( id )
    , converter_( converter )
    , network_  ( network )
    , resolver_ ( resolver )
    , holder_   ( holder )
    , exercise_ ( exercise )
    , lagAFrame_( lagAFrame )
    , adapted_  ( new plugins::tic::PlatformAdapter( holder_, converter ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisExtension destructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisExtension::~DisExtension()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisExtension::DoUpdate
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
void DisExtension::DoUpdate( const ASN1T_MsgUnitAttributes& )
{
    if( tic::TicExtension_ABC* extension = holder_.Retrieve< tic::TicExtension_ABC >() )
        extension->Accept( *this );
    else
        AddPlatform( *adapted_ );
}

// -----------------------------------------------------------------------------
// Name: DisExtension::AddPlatform
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void DisExtension::AddPlatform( const tic::Platform_ABC& platform )
{
    IT_Identifiers it = ids_.find( &platform );
    if( it == ids_.end() )
        it = ids_.insert( std::make_pair( &platform, id_.CreateNewIdentifier() ) ).first;

    EntityStatePDU pdu( time_.GetTime(), exercise_, it->second );
    pdu.SetEntityName( holder_.GetName().ascii() );
    pdu.SetEntityType( resolver_.Find( platform.GetType() ) );
    switch( platform.GetState() )
    {
    default:
    case tic::Platform_ABC::okay:       pdu.SetAppearance( 0, false, false ); break;
    case tic::Platform_ABC::broken:     pdu.SetAppearance( 1, false, true ); break;
    case tic::Platform_ABC::destroyed:  pdu.SetAppearance( 3, true, true ); break;
    }
    const geometry::Point2f position( lagAFrame_ ? platform.GetLastPosition() : platform.GetPosition() );
    const geometry::Point2d geocoord = converter_.ConvertToGeo( position );
    pdu.SetPosition( geocoord.Y(), geocoord.X(), platform.GetAltitude(), platform.GetSpeed(), platform.GetHeading() );

    network_.Send( pdu );
}
