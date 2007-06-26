// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
DirectFire::DirectFire( const ASN1T_MsgStartUnitFire& message, const Resolver_ABC< Agent_ABC >& agentResolver, const Resolver_ABC< Population_ABC >& populationResolver )
    : Fire_ABC( agentResolver.Get( message.firer_oid ) )
    , id_( message.fire_oid )
{
    if( message.target.t == T_MsgStartUnitFire_target_unit )
        target_ = & agentResolver.Get( message.target.u.unit );
    else if( message.target.t == T_MsgStartUnitFire_target_population )
        target_ = & populationResolver.Get( message.target.u.population );
    else
        throw std::runtime_error( "DirectFire on position..." );

    if( message.m.ammunitionPresent )
        ; // $$$$ AGE 2006-03-10: 
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
