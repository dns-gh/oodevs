// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Paths.h"
#include "GLOptions.h"
#include "GLView_ABC.h"
#include "Viewport_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/Protocol.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Paths constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Paths::Paths( const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , pendingMagicMove_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Paths destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Paths::~Paths()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Paths::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Paths::DoUpdate( const sword::UnitAttributes& message )
{
    static const float threshold = 30.f * 30.f;
    if( message.has_position()  )
    {
        const geometry::Point2f position = converter_.ConvertToXY( message.position() );
        if( previousPath_.empty() || previousPath_.back().SquareDistance( position ) > threshold )
        {
            if( pendingMagicMove_ )
            {
                previousPath_.clear();
                previousBox_ = geometry::Rectangle2f();
                plannedPath_.clear();
                plannedBox_ = geometry::Rectangle2f();
                pendingMagicMove_ = false;
            }
            previousPath_.push_back( position );
            previousBox_.Incorporate( position );
        }
        UpdatePathfind();
    }
}

// -----------------------------------------------------------------------------
// Name: Paths::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Paths::DoUpdate( const sword::UnitPathFind& message )
{
    plannedPath_.clear(); plannedPath_.reserve( message.path().location().coordinates().elem_size() );
    plannedBox_ = geometry::Rectangle2f();
    for( int i = 0; i < message.path().location().coordinates().elem_size(); ++i )
    {
        plannedPath_.push_back( converter_.ConvertToXY( message.path().location().coordinates().elem(i) ) );
        plannedBox_.Incorporate( plannedPath_.back() );
    }
    UpdatePathfind();
}

// -----------------------------------------------------------------------------
// Name: Paths::DoUpdate
// Created: AGE 2006-11-20
// -----------------------------------------------------------------------------
void Paths::DoUpdate( const sword::UnitMagicAction& /*message*/ )
{
    pendingMagicMove_ = true; // necessary to prevent a tick lag
}

// -----------------------------------------------------------------------------
// Name: Paths::UpdatePathfind
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void Paths::UpdatePathfind()
{
    if( plannedPath_.size() <= 1 || previousPath_.empty() )
        return;

    const geometry::Point2f position = previousPath_.back();
    float closestDistance = std::numeric_limits< float >::max();
    IT_PointVector closest = plannedPath_.begin();
    IT_PointVector previous = plannedPath_.begin();

    IT_PointVector itLastValidPoint = plannedPath_.end();
    for( IT_PointVector current = plannedPath_.begin() + 1;
         current != plannedPath_.end() && closestDistance > 0.1;
         ++current )
    {
        const geometry::Segment2f segment( *previous, *current );
        const float squareDist = segment.SquareDistance( position );

        if( squareDist < closestDistance )
        {
            closestDistance = squareDist;
            closest = previous;
        }
        previous = current;
    };

    std::copy( closest, plannedPath_.end(), plannedPath_.begin() );
    plannedPath_.resize( plannedPath_.end() - closest );
    if( ! plannedPath_.empty() )
        plannedPath_.front() = position;
}

// -----------------------------------------------------------------------------
// Name: Paths::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void Paths::Draw( const geometry::Point2f& /*where*/, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    const bool displayPath    = viewport.IsVisible( plannedBox_ )  && tools.GetOptions().ShouldDisplay( "Paths" );
    const bool displayOldPath = viewport.IsVisible( previousBox_ ) && tools.GetOptions().ShouldDisplay( "OldPaths" );

    if( displayPath || displayOldPath )
        glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT );
    if( displayPath )
    {
        static const float color[4] = { COLOR_PATH };
        glColor4f( color[0], color[1], color[2], tools.GetCurrentAlpha() );
        glLineWidth( 3 );
        glEnable( GL_LINE_STIPPLE );
        glLineStipple( 1, tools.StipplePattern() );
        tools.DrawLines( plannedPath_ );
        glDisable( GL_LINE_STIPPLE );
    }
    if( displayOldPath )
    {
        static const float colors[2][4] = { { COLOR_BLACK }, { COLOR_OLDPATH } };
        glColor4f( colors[0][0], colors[0][1], colors[0][2], tools.GetCurrentAlpha() );
        glLineWidth( 3 );
        tools.DrawLines( previousPath_ );
        glColor4f( colors[1][0], colors[1][1], colors[1][2], tools.GetCurrentAlpha() );
        glLineWidth( 2 );
        tools.DrawLines( previousPath_ );
    }
    if( displayPath || displayOldPath )
        glPopAttrib();
}

const T_PointVector& Paths::GetPath() const
{
    return plannedPath_;
}
