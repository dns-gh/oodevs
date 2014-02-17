// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "EllipticalArc.h"
#include "PathParser.h"
#include "PathContext.h"
#include "Tesselator_ABC.h"

using namespace svg;
using namespace path;

// -----------------------------------------------------------------------------
// Name: EllipticalArc constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
EllipticalArc::EllipticalArc( PathParser& data, PathContext& context )
    : radius_( data.GetCoordinate() )
    , phy_   ( data.GetFloat() * std::acos( -1.f ) / 180.f )
{
    bool largeArc = data.GetFlag();
    bool sweep = data.GetFlag();
    geometry::Point to = context.Translate( data.GetCoordinate() );

    EndpointToCenter( context.CurrentPoint(), to, largeArc, sweep );

    context.MoveTo( to );
    context.ResetControlPoint();
}

// -----------------------------------------------------------------------------
// Name: EllipticalArc destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
EllipticalArc::~EllipticalArc()
{
    // NOTHING
}

namespace
{
    float Angle( float ux, float uy, float vx, float vy )
    {
        const float dot   = ux * vx + uy * vy;
        const float norms = ( ux * ux + uy * uy ) * ( vx * vx + vy * vy );
        float ratio = dot / std::sqrt( norms );
        if( ratio < -1 ) ratio = -1;
        if( ratio >  1 ) ratio =  1;
        const float angle = std::acos( ratio );
        const float sign = ux * vy - uy * vx;
        return sign > 0 ? angle : -angle;
    };
}

// -----------------------------------------------------------------------------
// Name: EllipticalArc::EndpointToCenter
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
void EllipticalArc::EndpointToCenter( const geometry::Point& from, const geometry::Point& to, bool largeArc, bool sweep )
{
    const float cosPhy = std::cos( phy_ );
    const float sinPhy = std::sin( phy_ );
    float x1p, y1p;
    {
        const float xTrans =  ( from.X() - to.X() ) * 0.5f;
        const float yTrans =  ( from.Y() - to.Y() ) * 0.5f;
        x1p = cosPhy * xTrans + sinPhy * yTrans;
        y1p = cosPhy * yTrans - sinPhy * xTrans;
    }
    float cxp, cyp;
    {
        float rxs = radius_.X() * radius_.X();
        float rys = radius_.Y() * radius_.Y();
        const float x1ps = x1p * x1p;
        const float y1ps = y1p * y1p;
        float num   = rxs * rys - rxs * y1ps - rys * x1ps;
        {
            const float sigma = x1ps / rxs + y1ps / rys;
            if( sigma > 1 )
            {
                const float sigmaroot = std::sqrt( sigma );
                radius_ = geometry::Point( sigmaroot * radius_.X(), sigmaroot * radius_.Y() );
                rxs = radius_.X() * radius_.X();
                rys = radius_.Y() * radius_.Y();
                num = 0;
            }
        }
        const float denom = rxs * y1ps + rys * x1ps;
        const float multiplier = std::sqrt( num / denom ) * ( largeArc == sweep ? -1.f : 1.f);
        const float rRatio = radius_.X() / radius_.Y();
        cxp =   multiplier * y1p * rRatio;
        cyp = - multiplier * x1p / rRatio;
    }
    
    {
        const float mx = ( from.X() + to.X()  ) * 0.5f;
        const float my = ( from.Y() + to.Y()  ) * 0.5f;
        const float cx = mx + cosPhy * cxp - sinPhy * cyp;
        const float cy = my + sinPhy * cxp + cosPhy * cyp;
        center_ = geometry::Point( cx, cy );
    }

    {
        const float px = ( x1p - cxp ) / radius_.X();
        const float py = ( y1p - cyp ) / radius_.Y();
        thetaMin_   = Angle( 1, 0, px, py );
        const float qx = ( - x1p - cxp ) / radius_.X();
        const float qy = ( - y1p - cyp ) / radius_.Y();
        deltaTheta_ = Angle( px, py, qx, qy );
        if( ! sweep && deltaTheta_ > 0 )
            deltaTheta_ = deltaTheta_ - 2.f * std::acos( -1.f );
        if(   sweep && deltaTheta_ < 0 )
            deltaTheta_ = deltaTheta_ + 2.f * std::acos( -1.f );
    }

    length_ = ( radius_.X() + radius_.Y() ) * 0.5f * std::fabs( deltaTheta_ );
}

// -----------------------------------------------------------------------------
// Name: EllipticalArc::ComputeSegmentNumber
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
unsigned EllipticalArc::ComputeSegmentNumber( float expectedPrecision ) const
{
    unsigned result = unsigned( length_ / expectedPrecision );
    if( result < 2   ) result = 2;
    if( result > 100 ) result = 100;
    return result;
}

// -----------------------------------------------------------------------------
// Name: EllipticalArc::DrawLine
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void EllipticalArc::DrawLine( float expectedPrecision )
{
    const unsigned nSegment = ComputeSegmentNumber( expectedPrecision );
    const float increase = deltaTheta_ / float( nSegment );
    
    glPushMatrix();
    glTranslatef( center_.X(), center_.Y(), 0 );
    glRotatef( phy_ * 180 / std::acos( -1.f ), 0, 0, 1 );
    glBegin( GL_LINE_STRIP );
    for( unsigned i = 0; i <= nSegment; ++i )
    {
        const float theta = thetaMin_ + i * increase;
        glVertex2f( radius_.X() * std::cos( theta ), radius_.Y() * std::sin( theta ) );
    }
    glEnd();
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: EllipticalArc::DrawArea
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void EllipticalArc::DrawArea( Tesselator_ABC& tesselator, float expectedPrecision )
{
    const unsigned nSegment = ComputeSegmentNumber( expectedPrecision );
    const float increase = deltaTheta_ / float( nSegment );

    const float cosPhy = std::cos( phy_ );
    const float sinPhy = std::sin( phy_ );

    for( unsigned i = 0; i <= nSegment; ++i )
    {
        const float theta = thetaMin_ + i * increase;
        const float cx = radius_.X() * std::cos( theta );
        const float cy = radius_.Y() * std::sin( theta );
        const geometry::Point p( center_.X() + cosPhy * cx - sinPhy * cy
                                 , center_.Y() + sinPhy * cx + cosPhy * cy );
        tesselator.AddPoint( p );
    }
}
