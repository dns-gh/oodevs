// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DirectFire.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Positions.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DirectFire constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
DirectFire::DirectFire( const MsgsSimToClient::MsgStartUnitFire& message, const tools::Resolver_ABC< Agent_ABC >& agentResolver, const tools::Resolver_ABC< Population_ABC >& populationResolver )
    : Fire_ABC( agentResolver.Get( message.firing_unit().id() ) )
    , id_( message.fire().id() )
{
    if( message.target().has_unit() )
        target_ = & agentResolver.Get( message.target().unit().id() );
    else if( message.target().has_crowd() )
        target_ = & populationResolver.Get( message.target().crowd().id() );
    else
        throw std::runtime_error( "DirectFire on position..." );

//    if( message.has_ammunition()  )
//        ; // $$$$ AGE 2006-03-10:
    targetPosition_ = target_->Retrieve< Positions >();
}

// -----------------------------------------------------------------------------
// Name: DirectFire destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
DirectFire::~DirectFire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFire::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void DirectFire::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( targetPosition_ )
    {
        const geometry::Point2f pos = targetPosition_->GetPosition();
        if( viewport.IsVisible( geometry::Rectangle2f( where, pos ) ) )
            tools.DrawArrow( where, pos );
    }
}
