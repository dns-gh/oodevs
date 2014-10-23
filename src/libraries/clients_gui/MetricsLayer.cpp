// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "MetricsLayer.h"
#include "GLView_ABC.h"
#include "InformationToolTip.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MetricsLayer constructor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
MetricsLayer::MetricsLayer( kernel::Controllers& controllers,
                            const kernel::DetectionMap& elevation,
                            GLView_ABC& tools )
    : Layer( controllers, tools, eLayerTypes_Metric )
    , elevation_( elevation )
    , multiRulingMode_( false )
    , tooltip_( new InformationToolTip() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MetricsLayer destructor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
MetricsLayer::~MetricsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MetricsLayer::Paint
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void MetricsLayer::Paint( Viewport_ABC& )
{
    if( !multiRulingMode_ || metricPoints_.empty() )
    {
        tooltip_->Hide();
        return;
    }
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    for( auto it = metricPoints_.begin() + 1; it != metricPoints_.end(); ++it )
    {
        if( metricPoints_.size() > 1 )
        {
            geometry::Point2f start = *(it - 1 );
            geometry::Point2f end = *it ;
                glLineWidth( 4 );
                glColor4f( COLOR_WHITE );
                view_.DrawLine( start, end );
                glLineWidth( 2 );
                glColor4f( COLOR_BLACK );
                view_.DrawLine( start, end );
        }
    }
    start_ =  metricPoints_.front();
    glColor4f( COLOR_WHITE );
    glLineWidth( 2 );
    DrawAngle();
    glLineWidth( 4 );
    view_.DrawLine( metricPoints_.back(), end_ );
    glLineWidth( 2 );
    glColor4f( COLOR_BLACK );
    const QString message =
        tools::translate( "R�gle GL", "2D: %L1m\n3D: %L2m\n%L3�" )
            .arg( ComputeRuleDistance( false ), 0, 'f', 1 )
            .arg( ComputeRuleDistance( true ), 0, 'f', 1 )
            .arg( ComputeAngle(), 0, 'f', 1 );
    tooltip_->Start( Styles::bold ).Add( message.toStdString() ).End();
    tooltip_->Draw();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: MetricsLayer::DrawAngle
// Created: SBO 2009-09-29
// -----------------------------------------------------------------------------
void MetricsLayer::DrawAngle() const
{
    const geometry::Point2f north( start_ + geometry::Vector2f( 0, 1.f ) * start_.Distance( end_ ) );
    const geometry::Vector2f halfRuler( geometry::Vector2f( start_, end_ ) / 2 );
    view_.DrawLine( north, start_ );
    if( geometry::Vector2f( start_, north ).CrossProduct( halfRuler ) > 0 )
        view_.DrawArc( start_, start_ + geometry::Vector2f( start_, north ) / 2, start_ + halfRuler );
    else
        view_.DrawArc( start_, start_ + halfRuler, start_ + geometry::Vector2f( start_, north ) / 2 );
}

// -----------------------------------------------------------------------------
// Name: MetricsLayer::ComputeAngle
// Created: SBO 2009-09-29
// -----------------------------------------------------------------------------
float MetricsLayer::ComputeAngle() const
{
    static const float pi = std::acos( -1.f );
    float angle = std::atan( std::abs( end_.X() - start_.X() ) / ( end_.Y() - start_.Y() ) );
    if( angle <= 0 )
        angle += pi;
    return angle * 180.f / pi;
}

// -----------------------------------------------------------------------------
// Name: MetricsLayer::HandleMousePress
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
bool MetricsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( ( event->button() & Qt::LeftButton ) && event->modifiers() == Qt::ShiftModifier )
    {
        metricPoints_.push_back( point );
        return false;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MetricsLayer::HandleMouseMove
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
bool MetricsLayer::HandleMouseMove( QMouseEvent* event, const geometry::Point2f& point )
{
    if( event->modifiers() == Qt::ShiftModifier )
    {
        multiRulingMode_ = true;
        end_ = point;
    }
    else
    {
        metricPoints_.clear();
        multiRulingMode_ = false;
    }
    return multiRulingMode_;
}

// -----------------------------------------------------------------------------
// Name: MetricsLayer::ComputeRuleDistance
// Created: SLG 2010-03-01
// -----------------------------------------------------------------------------
float MetricsLayer::ComputeRuleDistance( bool b3dComputation )
{
    float distance = 0;
    if( b3dComputation )
    {
        for( auto it = ( metricPoints_.begin() + 1 ); it != metricPoints_.end(); ++it )
        {
            geometry::Point2f start2d = *(it - 1 );
            geometry::Point2f end2d = *(it );
            geometry::Point3f start3d = geometry::Point3f( start2d.X(), start2d.Y(), static_cast< float >( elevation_.ElevationAt( start2d ) ) ) ;
            geometry::Point3f end3d = geometry::Point3f( end2d.X(), end2d.Y(), static_cast< float >( elevation_.ElevationAt( end2d ) ) );
            distance += start3d.Distance( end3d );
        }
        geometry::Point3f start3dPoint = geometry::Point3f( metricPoints_.back().X(), metricPoints_.back().Y(), static_cast< float >( elevation_.ElevationAt( metricPoints_.back() ) ) );
        geometry::Point3f end3dPoint = geometry::Point3f( end_.X(), end_.Y(), static_cast< float >( elevation_.ElevationAt( end_ ) ) );
        distance += start3dPoint.Distance( end3dPoint );
        return distance;
    }
    else
    {
        for( auto it = ( metricPoints_.begin() + 1 ); it != metricPoints_.end(); ++it )
        {
            geometry::Point2f start = *(it - 1 );
            geometry::Point2f end = *(it );
            distance += start.Distance( end );
        }
        distance += metricPoints_.back().Distance( end_ );
        return distance;
    }
}
