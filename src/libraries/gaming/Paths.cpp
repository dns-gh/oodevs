// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Paths.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Paths constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Paths::Paths( const CoordinateConverter_ABC& converter )
    : converter_( converter ) 
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
void Paths::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    static const float threshold = 30.f * 30.f;
    if( message.m.positionPresent )
    {
        const Point2f position = converter_.ConvertToXY( message.position );
        if( previousPath_.empty() || previousPath_.back().SquareDistance( position ) > threshold )
        {
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
void Paths::DoUpdate( const ASN1T_MsgUnitPathFind& message )
{
    plannedPath_.clear(); plannedPath_.reserve( message.itineraire.vecteur_point.n );
    plannedBox_ = Rectangle2f();
    for( uint i = 0; i < message.itineraire.vecteur_point.n; ++i )
    {
        plannedPath_.push_back( converter_.ConvertToXY( message.itineraire.vecteur_point.elem[i] ) );
        plannedBox_.Incorporate( plannedPath_.back() );
    }
    UpdatePathfind();
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
void Paths::Draw( const Point2f& /*where*/, const Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    const bool displayPath    = ! viewport.Intersect( plannedBox_ ).IsEmpty()  && tools.ShouldDisplay( "Paths" );
    const bool displayOldPath = ! viewport.Intersect( previousBox_ ).IsEmpty() && tools.ShouldDisplay( "OldPaths" );

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
