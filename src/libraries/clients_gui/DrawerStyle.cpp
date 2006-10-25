// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerStyle.h"
#include "svgl/svgl.h"
#include "xeumeuleu/xml.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerStyle constructor
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
DrawerStyle::DrawerStyle( xml::xistream& input, kernel::GlTools_ABC& tools, svg::TextRenderer& renderer )
    : tools_       ( tools )
    , references_  ( new svg::References() )
    , renderer_    ( renderer )
    , line_        ( 0 )
    , markerStart_ ( 0 )
    , markerMiddle_( 0 )
    , markerEnd_   ( 0 )
{
    SVGFactory factory( renderer_ );
    
    std::string name, description;
    input >> xml::attribute( "name", name )
          >> xml::optional() >> xml::content( "description", description )
          >> xml::start( "segment" );

    name_ = name.c_str(); 
    if( ! description.empty() )
        description_ = description.c_str();
    else 
        description_ = name_;

    lineUnit = ReadUnit( input );
    line_ = factory.Compile( input, *references_, 1 ); // $$$$ AGE 2006-08-31: 

    input >> xml::end() // segment
          >> xml::list( "marker-start", *this, &DrawerStyle::ReadMarker, markerStart_, startUnit )
          >> xml::list( "marker-mid", *this, &DrawerStyle::ReadMarker, markerMiddle_, middleUnit )
          >> xml::list( "marker-end", *this, &DrawerStyle::ReadMarker, markerEnd_, endUnit );
}

// -----------------------------------------------------------------------------
// Name: DrawerStyle destructor
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
DrawerStyle::~DrawerStyle()
{
    delete references_;
}

// -----------------------------------------------------------------------------
// Name: DrawerStyle::GetName
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
QString DrawerStyle::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: DrawerStyle::GetDescription
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
QString DrawerStyle::GetDescription() const
{
    return description_;
}

// -----------------------------------------------------------------------------
// Name: DrawerStyle::ReadMarker
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawerStyle::ReadMarker( xml::xistream& input, svg::Node_ABC*& node, Unit& unit )
{
    SVGFactory factory( renderer_ );
    node = factory.Compile( input, *references_, 1 ); // $$$$ AGE 2006-08-31: 
    unit = ReadUnit( input );
}   

// -----------------------------------------------------------------------------
// Name: DrawerStyle::ReadUnit
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerStyle::Unit DrawerStyle::ReadUnit( xml::xistream& input )
{
    std::string unit( "m" );
    input >> xml::optional() >> xml::attribute( "unit", unit );
    if( unit == "m" )
        return eMeter;
    if( unit == "px" )
        return ePixel;
    if( unit == "%" )
        return ePercent;
    throw std::runtime_error( "Unknown unit " + unit );
}

// -----------------------------------------------------------------------------
// Name: DrawerStyle::Draw
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawerStyle::Draw( const T_PointVector& points, svg::RenderingContext_ABC& context ) const
{
    if( points.size() < 2 )
        return;

    if( line_ )
        for( CIT_PointVector it = points.begin(); it != points.end() - 1; ++it )
            DrawSegment( context, *it, *(it+1) );
    if( markerStart_ )
        DrawStartMarker( context, points[0], points[1] );
    if( markerMiddle_ )
        for( CIT_PointVector it = points.begin() + 1; it != points.end() - 1; ++it )
            DrawMiddleMarker( context, *it, *(it-1), *(it+1) );
    if( markerEnd_ )
        DrawEndMarker( context, points.back(), points[ points.size() - 2 ] );
}

// -----------------------------------------------------------------------------
// Name: DrawerStyle::DrawSegment
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawerStyle::DrawSegment( svg::RenderingContext_ABC& context, const geometry::Point2f& from, const geometry::Point2f& to ) const
{
    glPushMatrix();

    glTranslatef( from.X(), from.Y(), 0 );
    geometry::Vector2f u( from, to );
    Align( u );
    const float length = u.Length();
    const float yFactor = ComputeFactor( lineUnit, length );
    glScalef( length, yFactor, 1 );

    line_->Draw( context, *references_ );

    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: DrawerStyle::ComputeFactor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
float DrawerStyle::ComputeFactor( Unit u, float base ) const
{
    if( u == ePercent )
        return base;
    else if( u == ePixel )
        return tools_.Pixels();
    return 1;
}

// -----------------------------------------------------------------------------
// Name: DrawerStyle::DrawStartMarker
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawerStyle::DrawStartMarker( svg::RenderingContext_ABC& context, const geometry::Point2f& at, const geometry::Point2f& towards ) const
{
    DrawMarker( context, *markerStart_, startUnit, at, geometry::Vector2f( at, towards ) );
}

// -----------------------------------------------------------------------------
// Name: DrawerStyle::DrawMiddleMarker
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawerStyle::DrawMiddleMarker( svg::RenderingContext_ABC& context, const geometry::Point2f& at, const geometry::Point2f& from, const geometry::Point2f& to ) const
{
    const geometry::Vector2f u( from, at );
    const geometry::Vector2f v( at, to );
    const geometry::Vector2f n = u + v;
    const geometry::Vector2f t( n.Y(), -n.X() );
    DrawMarker( context, *markerMiddle_, middleUnit, at, t );
}

// -----------------------------------------------------------------------------
// Name: DrawerStyle::DrawEndMarker
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawerStyle::DrawEndMarker( svg::RenderingContext_ABC& context, const geometry::Point2f& at, const geometry::Point2f& from ) const
{
    DrawMarker( context, *markerEnd_, endUnit, at, geometry::Vector2f( from, at ) );
}

// -----------------------------------------------------------------------------
// Name: DrawerStyle::DrawMarker
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawerStyle::DrawMarker( svg::RenderingContext_ABC& context, svg::Node_ABC& node, Unit unit, const geometry::Point2f& at, geometry::Vector2f direction ) const
{
    glPushMatrix();
    glTranslatef( at.X(), at.Y(), 0 );
    Align( direction );
    const float ratio = ComputeFactor( unit, direction.Length() ); // $$$$ AGE 2006-09-01: 
    glScalef( ratio, ratio, 1 );
    node.Draw( context, *references_ );
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: DrawerStyle::Align
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawerStyle::Align( geometry::Vector2f u ) const
{
    static const float deg = 180.f / std::acos( -1.f );
    u.Normalize();
    float angle = std::acos( u.X() ) * deg;
    if( u.Y() < 0 )
        angle=-angle;
    glRotatef( angle, 0, 0, 1 );
}
