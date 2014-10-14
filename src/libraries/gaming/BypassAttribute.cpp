// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "BypassAttribute.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: BypassAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
BypassAttribute::BypassAttribute( Controller& controller, bool hasSinglePointPos )
    : controller_( controller )
    , nBypassConstructionPercentage_ ( 0 )
    , hasSinglePointPos_( hasSinglePointPos )
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
void BypassAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
                .Display( tools::translate( "Object", "Bypass:" ), nBypassConstructionPercentage_ * Units::percentage );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void BypassAttribute::DisplayInSummary( Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::DisplayInTooltip
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void BypassAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::DoUpdate
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void BypassAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::DoUpdate
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void BypassAttribute::DoUpdate( const sword::ObjectUpdate& message )
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
    if( message.has_bypass() )
    {
        nBypassConstructionPercentage_ = message.bypass().percentage();
        controller_.Update( *(BypassAttribute_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Draw
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void BypassAttribute::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
    {
        // $$$$ SBO 2007-05-04: hard coded icon positions
        glPushAttrib( GL_CURRENT_BIT );
            glColor4f( 1, 1, 1, tools.GetCurrentAlpha() );
            tools.DrawLife( where - geometry::Vector2f( 0.f, tools.GetAdaptiveZoomFactor( !hasSinglePointPos_ ) * 200.f ), nBypassConstructionPercentage_ / 100.f, hasSinglePointPos_ );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::GetBypassConstructionPercentage
// Created: NPT 2012-09-19
// -----------------------------------------------------------------------------
int BypassAttribute::GetBypassConstructionPercentage() const
{
    return nBypassConstructionPercentage_;
}
