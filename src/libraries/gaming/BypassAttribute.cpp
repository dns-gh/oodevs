// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "BypassAttribute.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: BypassAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
BypassAttribute::BypassAttribute( kernel::Controller& controller )
    : controller_( controller )
    , rBypassConstructionPercentage_ ( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: BypassAttribute destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
BypassAttribute::~BypassAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void BypassAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
                .Display( tools::translate( "Object", "Bypass:" ), rBypassConstructionPercentage_ * Units::percentage );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void BypassAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::DisplayInTooltip
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void BypassAttribute::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::DoUpdate
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void BypassAttribute::DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::DoUpdate
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void BypassAttribute::DoUpdate( const MsgsSimToClient::MsgObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::DoUpdate
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void BypassAttribute::DoUpdate( const MsgsSimToClient::MsgObjectCreation& message )
{
    UpdateData( message.attributes() );
}
   
// -----------------------------------------------------------------------------
// Name: BypassAttribute::UpdateData
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
template< typename T >
void BypassAttribute::UpdateData( const T& message )
{
    if( message.has_bypass()  )
    {
        rBypassConstructionPercentage_ = static_cast< float >( message.bypass().percentage() );
        controller_.Update( *(BypassAttribute_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Draw
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void BypassAttribute::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
    {
        // $$$$ SBO 2007-05-04: hard coded icon positions
        glPushAttrib( GL_CURRENT_BIT );
            glColor3f( 1, 1, 1 );    
            tools.DrawLife( where - geometry::Vector2f( 0.f, 200.f ), rBypassConstructionPercentage_ / 100.f );
        glPopAttrib();
    }
}
