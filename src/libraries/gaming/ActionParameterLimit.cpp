// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterLimit.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterLimit::ActionParameterLimit( const QString& name, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Line& line )
    : ActionParameter< QString >( name, true )
{
    points_.reserve( line.vecteur_point.n );
    for( unsigned int i = 0; i < line.vecteur_point.n; ++i )
    {
        const geometry::Point2f point = converter.ConvertToXY( line.vecteur_point.elem[i] );
        points_.push_back( point );
        boundingBox_.Incorporate( point );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit constructor
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
ActionParameterLimit::ActionParameterLimit( const QString& name, const T_PointVector& points )
    : ActionParameter< QString >( name, true )
    , points_( points )
{
    for( unsigned int i = 0; i < points_.size(); ++i )
        boundingBox_.Incorporate( points_[i] );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit destructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterLimit::~ActionParameterLimit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit::Draw
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
void ActionParameterLimit::Draw( const geometry::Point2f&, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( boundingBox_ ) && !points_.empty() )
    {
        glPushAttrib( GL_LINE_BIT );
        glLineWidth( 3.f );
        tools.DrawLines( points_ );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionParameterLimit::Serialize( xml::xostream& xos ) const
{
    for( CIT_PointVector it = points_.begin(); it != points_.end(); ++it )
        xos << start( "point" )
                << attribute( "x", it->X() )
                << attribute( "y", it->Y() )
            << end();
}
