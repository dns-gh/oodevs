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
#include "tic_plugin/TicExtension_ABC.h"
#include "tic_plugin/Platform_ABC.h"
#include "dispatcher/Agent.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "UdpNetwork.h"
#include <geocoord/MGRS.h>
#include <geocoord/Geodetic.h>

using namespace dis;

// -----------------------------------------------------------------------------
// Name: DisExtension constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisExtension::DisExtension( const Time_ABC& time, IdentifierFactory_ABC& id, const kernel::CoordinateConverter_ABC& converter, UdpNetwork& network, dispatcher::Agent& holder, unsigned char exercise )
    : time_     ( time )
    , id_       ( id )
    , myId_     ( id_.CreateNewIdentifier() )
    , converter_( converter )
    , network_  ( network )
    , holder_   ( holder )
    , exercise_ ( exercise )
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
        SendUnitState();
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
    pdu.SetEntityName( holder_.GetName() );

    const geometry::Point2d position( platform.GetPosition().X(), platform.GetPosition().Y() );
    const geometry::Point2d geocoord = converter_.ConvertToGeo( position );
    pdu.SetPosition( geocoord.Y(), geocoord.X(), platform.GetAltitude(), platform.GetSpeed(), platform.GetHeading() );

    network_.Send( pdu );
}

// -----------------------------------------------------------------------------
// Name: DisExtension::SendUnitState
// Created: AGE 2008-04-02
// -----------------------------------------------------------------------------
void DisExtension::SendUnitState() const
{
    EntityStatePDU pdu( time_.GetTime(), exercise_, myId_ );
    pdu.SetEntityName( holder_.GetName() );

    pdu.SetPosition( holder_.position_.latitude, holder_.position_.longitude,
        (float)holder_.nHeight_, (float)holder_.nSpeed_, (float)holder_.nDirection_ );

    network_.Send( pdu );
}
