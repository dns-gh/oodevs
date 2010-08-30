// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Fires.h"
#include "clients_kernel/Controller.h"
#include "IndirectFire.h"
#include "DirectFire.h"
#include "FireFactory.h"
#include "Fire_ABC.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Fires constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Fires::Fires( Controller& controller, FireFactory& factory )
    : controller_( controller )
    , factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Fires destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Fires::~Fires()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Fires::CreateFire
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
template< typename T >
void Fires::CreateFire( const T& message )
{
    if( ! Find( message.id().id() ) )
    {
        Fire_ABC* fire = factory_.CreateFire( message );
        Register( message.id().id(), *fire );
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Fires::DestroyFire
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
template< typename T >
void Fires::DestroyFire( const T& message )
{
    Fire_ABC* fire = Find( message.id().id() );
    if( fire )
    {
        Remove( message.id().id() );
        delete fire;
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Fires::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Fires::DoUpdate( const MsgsSimToClient::MsgStartUnitFire& message )
{
    CreateFire( message );
}

// -----------------------------------------------------------------------------
// Name: Fires::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Fires::DoUpdate( const MsgsSimToClient::MsgStopUnitFire& message )
{
    DestroyFire( message );
}

// -----------------------------------------------------------------------------
// Name: Fires::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Fires::DoUpdate( const MsgsSimToClient::MsgStartPopulationFire& message )
{
    CreateFire( message );
}

// -----------------------------------------------------------------------------
// Name: Fires::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Fires::DoUpdate( const MsgsSimToClient::MsgStopPopulationFire& message )
{
    DestroyFire( message );
}

// -----------------------------------------------------------------------------
// Name: Fires::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void Fires::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( ! elements_.empty() )
    {
        glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
        glLineWidth( 3.f );
        glColor4f( COLOR_RED );
        for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
            it->second->Draw( where, viewport, tools );
        glPopAttrib();
    }
}
