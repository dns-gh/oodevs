// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "RenderingContext.h"
#include "Color.h"
#include "ColorNone.h"
#include "FillRule.h"
#include "Property_ABC.h"
#include "Opacity.h"
#include "References_ABC.h"
#include "Length.h"
#include "DashArray.h"
#include "FontFamily.h"
#include "FontWeight.h"
#include "FontSize.h"
#include "TextAnchor.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: RenderingContext constructor
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
RenderingContext::RenderingContext()
    : viewport_()
    , pixels_( 1 )
    , expectedPrecision_( 10 )
    , pickingMode_( false )
    , lineWithFactor_( 1.f )
{
    PushDefaultValues();
}

// -----------------------------------------------------------------------------
// Name: RenderingContext constructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
RenderingContext::RenderingContext( float expectedPrecision )
    : expectedPrecision_( expectedPrecision )
    , pixels_           ( 1 )
    , pickingMode_( false )
    , lineWithFactor_( 1.f )
{
    PushDefaultValues();
}

namespace
{
    void DeleteFront( const std::vector< Property_ABC* >& properties )
    {
        if( ! properties.empty() )
            delete properties.front();
    };
};

// -----------------------------------------------------------------------------
// Name: RenderingContext destructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
RenderingContext::~RenderingContext()
{
    DeleteFront( properties_[ fill ] );
    DeleteFront( properties_[ fillRule ] );
    DeleteFront( properties_[ fillOpacity ] );
    DeleteFront( properties_[ stroke ] );
    DeleteFront( properties_[ strokeWidth ] );
    DeleteFront( properties_[ strokeOpacity ] );
    DeleteFront( properties_[ strokeDasharray ] );
    DeleteFront( properties_[ fontFamily ] );
    DeleteFront( properties_[ fontWeight ] );
    DeleteFront( properties_[ fontSize ] );
    DeleteFront( properties_[ textAnchor ] );
}

// -----------------------------------------------------------------------------
// Name: RenderingContext::SetViewport
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
void RenderingContext::SetViewport( const geometry::BoundingBox& viewport, unsigned w, unsigned h )
{
    viewport_ = viewport;
    pixels_ =   std::sqrt( ( viewport.Width() * viewport.Width() + viewport.Height() * viewport.Height() ) )
              / std::sqrt( ( float( w ) * w + float( h ) * h ) );
    expectedPrecision_ = 10 * pixels_;
}

// -----------------------------------------------------------------------------
// Name: RenderingContext::PushDefaultValues
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
void RenderingContext::PushDefaultValues()
{
    properties_.resize( nbrProperty );
    PushProperty( fill,            *new Color    ( "#000000" ) );
    PushProperty( fillRule,        *new FillRule () );
    PushProperty( fillOpacity,     *new Opacity  () );
    PushProperty( stroke,          *new ColorNone() );
    PushProperty( strokeWidth,     *new Length( "1px" ) );
    PushProperty( strokeOpacity,   *new Opacity  () );
    PushProperty( strokeDasharray, *new DashArray("none") );
    PushProperty( fontFamily,      *new FontFamily("Arial") );
    PushProperty( fontWeight,      *new FontWeight("medium") );
    PushProperty( fontSize,        *new FontSize("16") );
    PushProperty( textAnchor,      *new TextAnchor("start") );
}

// -----------------------------------------------------------------------------
// Name: RenderingContext::PushProperty
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
void RenderingContext::PushProperty( E_Properties name, Property_ABC& value )
{
    T_Properties& properties = properties_[ name ];
    if( ! properties.empty() )
        properties.back()->LeaveTop( *this );
    properties.push_back( &value );
    properties.back()->ReachTop( *this );
}

// -----------------------------------------------------------------------------
// Name: RenderingContext::PopProperty
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
void RenderingContext::PopProperty( E_Properties name )
{
    T_Properties& properties = properties_[ name ];
    if( properties.empty() )
        throw std::runtime_error( __FUNCTION__ );
    properties.back()->LeaveTop( *this );
    properties.pop_back();
    if( ! properties.empty() )
        properties.back()->ReachTop( *this );
}

// -----------------------------------------------------------------------------
// Name: RenderingContext::FindProperty
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
const Property_ABC* RenderingContext::FindProperty( E_Properties name ) const
{
    const T_Properties& properties = properties_[ name ];
    if( properties.empty() )
        throw std::runtime_error( __FUNCTION__ );
    return properties.back();
}

// -----------------------------------------------------------------------------
// Name: RenderingContext::ExpectedPrecision
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
float RenderingContext::ExpectedPrecision() const
{
    return expectedPrecision_;
}

// -----------------------------------------------------------------------------
// Name: RenderingContext::Pixels
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
float RenderingContext::Pixels() const
{
    return pixels_;
}

// -----------------------------------------------------------------------------
// Name: RenderingContext::LineWidthFactor
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
float RenderingContext::LineWidthFactor() const
{
    return lineWithFactor_;
}

// -----------------------------------------------------------------------------
// Name: RenderingContext::SetupFill
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
bool RenderingContext::SetupFill( References_ABC& references ) const
{
    return GetProperty< Paint_ABC >( fill ).Setup( references, GetProperty< Opacity >( fillOpacity ).GetValue() );
}

// -----------------------------------------------------------------------------
// Name: RenderingContext::SetupStroke
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
bool RenderingContext::SetupStroke( References_ABC& references ) const
{
    const bool result = GetProperty< Paint_ABC >( stroke ).Setup( references, GetProperty< Opacity >( strokeOpacity ).GetValue() );
    if( result )
        GetProperty< Length >( strokeWidth ).SetupLineWidth();
    return result;
}

// -----------------------------------------------------------------------------
// Name: RenderingContext::IsPickingMode
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
bool RenderingContext::IsPickingMode() const
{
    return pickingMode_;
}

// -----------------------------------------------------------------------------
// Name: RenderingContext::EnablePickingMode
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
void RenderingContext::EnablePickingMode( float lineWithFactor )
{
    pickingMode_ = true;
    lineWithFactor_ = lineWithFactor;
}

// -----------------------------------------------------------------------------
// Name: RenderingContext::DisablePickingMode
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
void RenderingContext::DisablePickingMode()
{
    pickingMode_ = false;
    lineWithFactor_ = 1.f;
}
