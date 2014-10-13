// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationFlow.h"
#include "clients_gui/GLOptions.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationFlow constructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationFlow::PopulationFlow( const sword::CrowdFlowCreation& message, const CoordinateConverter_ABC& converter )
    : converter_          ( converter )
    , nID_                ( message.flow().id() )
    , flow_               ( 2, Point2f( 0, 0 ) )
    , nDirection_         ( 0 )
    , nSpeed_             ( 0 )
    , nHealthyHumans_     ( 0 )
    , nWoundedHumans_     ( 0 )
    , nContaminatedHumans_( 0 )
    , nDeadHumans_        ( 0 )
    , rDensity_           ( 0 )
    , attitude_           ( ePopulationAttitude_Calme )
{
    AddExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow destructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationFlow::~PopulationFlow()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetName
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
QString PopulationFlow::GetName() const
{
    static const QString flow( "Flow" );
    return flow;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetId
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
unsigned long PopulationFlow::GetId() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::DoUpdate
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
void PopulationFlow::DoUpdate( const sword::CrowdFlowUpdate& message )
{
    if( message.has_attitude() )
        attitude_ = static_cast< E_PopulationAttitude >( message.attitude() );
    if( message.has_healthy() )
        nHealthyHumans_ = message.healthy();
    if( message.has_wounded() )
        nWoundedHumans_ = message.wounded();
    if( message.has_contaminated() )
        nContaminatedHumans_ = message.contaminated();
    if( message.has_dead() )
        nDeadHumans_ = message.dead();
    if( message.has_speed() )
        nSpeed_ = message.speed();
    if( message.has_direction() )
        nDirection_ = message.direction().heading();
    if( message.has_parts() )
    {
        flow_.clear();
        flow_.reserve( message.parts().location().coordinates().elem_size() );
        boundingBox_ = Rectangle2f();
        for( int i = 0; i < message.parts().location().coordinates().elem_size(); ++i )
        {
            flow_.push_back( converter_.ConvertToXY( message.parts().location().coordinates().elem( i ) ) );
            boundingBox_.Incorporate( flow_.back() );
        }
        // Density
        float rLength = 0.;
        CIT_PointVector itCur = flow_.begin();
        CIT_PointVector itNext = itCur;
        ++itNext;
        for( ; itNext != flow_.end(); ++itNext, ++itCur )
            rLength += ( *itCur ).Distance( *itNext );
        if( rLength > 0. )
            rDensity_ = ( nHealthyHumans_ + nWoundedHumans_ + nContaminatedHumans_ ) / rLength;
        else
            rDensity_ = 0.;
    }
    if( message.has_path() )
    {
        path_.clear();
        for( int i = 0; i < message.path().location().coordinates().elem_size(); ++i )
            path_.push_back( converter_.ConvertToXY( message.path().location().coordinates().elem(i) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetDensity
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
float PopulationFlow::GetDensity() const
{
    return rDensity_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetHealthyHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int PopulationFlow::GetHealthyHumans() const
{
    return nHealthyHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetWoundedHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int PopulationFlow::GetWoundedHumans() const
{
    return nWoundedHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetContaminatedHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int PopulationFlow::GetContaminatedHumans() const
{
    return nContaminatedHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetDeadHumans
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
unsigned int PopulationFlow::GetDeadHumans() const
{
    return nDeadHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetAttitude
// Created: SBO 2006-08-23
// -----------------------------------------------------------------------------
QString PopulationFlow::GetAttitude() const
{
    return tools::ToString( attitude_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetHeight
// Created: SBO 2006-08-23
// -----------------------------------------------------------------------------
float PopulationFlow::GetHeight( bool ) const
{
    return 0.;
}

namespace
{
    void SelectColor( E_PopulationAttitude attitude, GLfloat alpha )
    {
        if( attitude == ePopulationAttitude_Agressive )
            glColor4f( COLOR_POPULATION_ATTITUDE_AGRESSIVE, alpha );
        else if( attitude == ePopulationAttitude_Excitee )
            glColor4f( COLOR_POPULATION_ATTITUDE_EXCITED, alpha );
        else if( attitude == ePopulationAttitude_Agitee )
            glColor4f( COLOR_POPULATION_ATTITUDE_AGITATED, alpha );
        else // ePopulationAttitude_Calme
            glColor4f( COLOR_POPULATION_ATTITUDE_CALM, alpha );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void PopulationFlow::Draw( const Point2f& /*where*/, const gui::Viewport_ABC& , gui::GLView_ABC& tools ) const
{
    const bool drawingMode = !tools.IsPickingMode();
    glPushAttrib( GL_LINE_BIT );
        glLineWidth( 10.f );
        glPushAttrib( GL_CURRENT_BIT );
            if( drawingMode )
            {
                GLfloat currentColor[ 4 ];
                glGetFloatv( GL_CURRENT_COLOR, currentColor );
                SelectColor( attitude_, currentColor[ 3 ] );
            }
            tools.DrawLines( flow_ );
        glPopAttrib();
        if( drawingMode )
        {
            glLineWidth( 8.f );
            tools.DrawLines( flow_ );
        }
    glPopAttrib();

    const bool displayPath = /*viewport.IsVisible( plannedBox_ )  && */tools.GetOptions().ShouldDisplay( "Paths" );
    if( displayPath )
    {
        glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT );
            static const float color[4] = { COLOR_PATH };
            if( drawingMode )
                glColor4f( color[0], color[1], color[2], tools.GetCurrentAlpha() );
            glLineWidth( 3 );
            glEnable( GL_LINE_STIPPLE );
            glLineStipple( 1, tools.StipplePattern() );
            tools.DrawLines( path_ );
            glDisable( GL_LINE_STIPPLE );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetPosition
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
Point2f PopulationFlow::GetPosition( bool ) const
{
    if( ! flow_.empty() )
        return flow_.front();
    return Point2f();
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::IsIn
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
bool PopulationFlow::IsIn( const Rectangle2f& rect ) const
{
    return ! boundingBox_.Intersect( rect ).IsEmpty();
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
Rectangle2f PopulationFlow::GetBoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void PopulationFlow::Accept( LocationVisitor_ABC& visitor ) const
{
    visitor.VisitLines( flow_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool PopulationFlow::CanAggregate() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetNearestPosition
// Created: ABR 2012-11-12
// -----------------------------------------------------------------------------
geometry::Point2f PopulationFlow::GetNearestPosition( const geometry::Point2f& position ) const
{
    geometry::Point2f result;
    float minDistance = std::numeric_limits< float >::max();
    for( auto it = flow_.begin(); it != flow_.end(); ++it )
    {
        float distance = it->Distance( position );
        if( distance < minDistance )
        {
            minDistance = distance;
            result = *it;
        }
    }
    return result;
}
