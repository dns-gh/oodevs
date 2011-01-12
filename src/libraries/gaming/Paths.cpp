// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Paths.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "protocol/Protocol.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Paths constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Paths::Paths( const CoordinateConverter_ABC& converter )
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
    static const float threshold      = 30.f * 30.f;
    static const float magicThreshold = 1000.f * 1000.f;
    if( message.has_position()  )
    {
        const Point2f position = converter_.ConvertToXY( message.position() );
        if( previousPath_.empty() || previousPath_.back().SquareDistance( position ) > threshold )
        {
            if( pendingMagicMove_ && previousPath_.back().SquareDistance( position ) > magicThreshold )
            {
                previousPath_.clear(); previousBox_ = Rectangle2f();
                plannedPath_.clear(); plannedBox_ = Rectangle2f();
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
    plannedBox_ = Rectangle2f();
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
    pendingMagicMove_ = true;
}

// -----------------------------------------------------------------------------
// Name: Paths::UpdatePathfind
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void Paths::UpdatePathfind()
{
    if( plannedPath_.size() <= 1 || previousPath_.empty() )
        return;

    const Point2f position = previousPath_.back();
    float closestDistance = std::numeric_limits< float >::max();
    IT_PointVector closest = plannedPath_.begin();
    IT_PointVector previous = plannedPath_.begin();

    IT_PointVector itLastValidPoint = plannedPath_.end();
    for( IT_PointVector current = plannedPath_.begin() + 1;
         current != plannedPath_.end() && closestDistance > 0.1;
         ++current )
    {
        const Segment2f segment( *previous, *current );
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
void Paths::Draw( const Point2f& /*where*/, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    const bool displayPath    = viewport.IsVisible( plannedBox_ )  && tools.ShouldDisplay( "Paths" );
    const bool displayOldPath = viewport.IsVisible( previousBox_ ) && tools.ShouldDisplay( "OldPaths" );

    if( displayPath || displayOldPath )
        glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT );
    if( displayPath )
    {
        glColor4f( COLOR_PATH );
        glLineWidth( 3 );
        glEnable( GL_LINE_STIPPLE );
        glLineStipple( 1, tools.StipplePattern() );
        tools.DrawLines( plannedPath_ );
        glDisable( GL_LINE_STIPPLE );
    }
    if( displayOldPath )
    {
        glColor4f( COLOR_BLACK );
        glLineWidth( 3 );
        tools.DrawLines( previousPath_ );
        glColor4f( COLOR_OLDPATH );
        glLineWidth( 2 );
        tools.DrawLines( previousPath_ );
    }
    if( displayPath || displayOldPath )
        glPopAttrib();
}
