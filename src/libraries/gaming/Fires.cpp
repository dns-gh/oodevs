// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Fires.h"
#include "FireFactory.h"
#include "Fire_ABC.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/SoundEvent.h"
#include "clients_kernel/Controller.h"
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
    const auto id = message.fire().id();
    if( !Find( id ) )
    {
        Register( id, *factory_.CreateFire( message, agentId_ ) );
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
    const auto id = message.fire().id();
    if( const Fire_ABC* fire = Find( id ) )
    {
        Remove( id );
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

void Fires::DoUpdate( const sword::StartUnitFireDetection& message )
{
    CreateFire( message );
}

void Fires::DoUpdate( const sword::StopUnitFireDetection& message )
{
    DestroyFire( message );
}

// -----------------------------------------------------------------------------
// Name: Fires::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void Fires::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
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
