// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "SealOffAttribute.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SealOffAttribute constructor
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
SealOffAttribute::SealOffAttribute( kernel::Controller& controller )
    : controller_( controller )
    , rSealOffLevel_ ( 0.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute destructor
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
SealOffAttribute::~SealOffAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute::Display
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
void SealOffAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Decisionnal" ) )
             .Display( tools::translate( "Object", "Sealing level:" ), rSealOffLevel_ * Units::percentage );
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute::DisplayInSummary
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
void SealOffAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute::DisplayInTooltip
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
void SealOffAttribute::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute::DoUpdate
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
void SealOffAttribute::DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute::DoUpdate
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
void SealOffAttribute::DoUpdate( const MsgsSimToClient::MsgObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute::DoUpdate
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
void SealOffAttribute::DoUpdate( const MsgsSimToClient::MsgObjectCreation& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute::UpdateData
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
template< typename T >
void SealOffAttribute::UpdateData( const T& message )
{
    if( message.has_bypass()  )
    {
        rSealOffLevel_ = static_cast< float >( message.seal_off().level() );
        controller_.Update( *(SealOffAttribute_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute::Draw
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
void SealOffAttribute::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
    {
        // $$$$ SBO 2007-05-04: hard coded icon positions
        glPushAttrib( GL_CURRENT_BIT );
            glColor3f( 0, 1, 1 );
            tools.DrawLife( where - geometry::Vector2f( 0.f, 200.f ), rSealOffLevel_ / 100.f );
        glPopAttrib();
    }
}
