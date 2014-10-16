// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ConstructionAttribute.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( kernel::Controller& controller, const tools::Resolver_ABC< DotationType >& resolver, bool hasSinglePointPos )
    : controller_( controller )
    , resolver_ ( resolver )
    , construction_ ( 0 )
    , nDotationConstruction_ ( 0 )
    , nConstructionPercentage_ ( 0 )
    , hasSinglePointPos_( hasSinglePointPos )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ConstructionAttribute::~ConstructionAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ConstructionAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Construction:" ), nConstructionPercentage_ * Units::percentage );
    if( !construction_ )
        return;
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Item( tools::translate( "Object", "Construction resource:" ) )
                .Start( nDotationConstruction_ )
                .Add( " " ).Add( construction_ ).End();
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ConstructionAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::DisplayInTooltip
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ConstructionAttribute::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
     Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::DoUpdate
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ConstructionAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::DoUpdate
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ConstructionAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::UpdateData
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
template< typename T >
void ConstructionAttribute::UpdateData( const T& message )
{
    if( message.has_construction()  )
    {
        if( ! construction_ && message.construction().has_resource()  )
            construction_ = resolver_.Find( message.construction().resource().id() );
        if( message.construction().has_dotation()  )
            nDotationConstruction_ = message.construction().dotation();
        if( message.construction().has_percentage()  )
            nConstructionPercentage_ = message.construction().percentage();
        controller_.Update( *(ConstructionAttribute_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Draw
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void ConstructionAttribute::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( nConstructionPercentage_.IsSet() && viewport.IsVisible( where ) )
    {
        // $$$$ SBO 2007-05-04: hard coded icon positions
        glPushAttrib( GL_CURRENT_BIT );
            glColor3f( 1, 1, 1 );
            tools.DrawLife( where - geometry::Vector2f( 0.f, tools.GetAdaptiveZoomFactor( !hasSinglePointPos_ ) * 250.f ), nConstructionPercentage_ / 100.f, 1.f, hasSinglePointPos_ );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::GetConstructionPercentage
// Created: NPT 2012-09-19
// -----------------------------------------------------------------------------
int ConstructionAttribute::GetConstructionPercentage() const
{
    return nConstructionPercentage_;
}
