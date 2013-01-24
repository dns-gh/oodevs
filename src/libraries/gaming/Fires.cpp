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
#include "IndirectFire.h"
#include "DirectFire.h"
#include "FireFactory.h"
#include "Fire_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/GlTools_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Fires constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Fires::Fires( Controller& controller, FireFactory& factory, unsigned long agentId )
    : controller_( controller )
    , factory_   ( factory )
    , agentId_   ( agentId )
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
    if( ! Find( message.fire().id() ) )
    {
        Fire_ABC* fire = factory_.CreateFire( message, agentId_ );
        Register( message.fire().id(), *fire );
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
    Fire_ABC* fire = Find( message.fire().id() );
    if( fire )
    {
        Remove( message.fire().id() );
        delete fire;
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Fires::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Fires::DoUpdate( const sword::StartUnitFire& message )
{
    CreateFire( message );
}

// -----------------------------------------------------------------------------
// Name: Fires::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Fires::DoUpdate( const sword::StopUnitFire& message )
{
    DestroyFire( message );
}

// -----------------------------------------------------------------------------
// Name: Fires::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Fires::DoUpdate( const sword::StartCrowdFire& message )
{
    CreateFire( message );
}

// -----------------------------------------------------------------------------
// Name: Fires::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Fires::DoUpdate( const sword::StopCrowdFire& message )
{
    DestroyFire( message );
}

// -----------------------------------------------------------------------------
// Name: Fires::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void Fires::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( ! elements_.empty() )
    {
        glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
        glLineWidth( 3.f );
        glColor4f( COLOR_RED );
        for( auto it = elements_.begin(); it != elements_.end(); ++it )
            it->second->Draw( where, viewport, tools );
        glPopAttrib();
    }
}
