// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LocationCreator.h"
#include "moc_LocationCreator.cpp"
#include "ParametersLayer.h"
#include "Tools.h"
#include "GlTools_ABC.h"
#include "ShapeHandler_ABC.h"

// -----------------------------------------------------------------------------
// Name: LocationCreator constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
LocationCreator::LocationCreator( QWidget* parent, const std::string& menu, ParametersLayer& layer, ShapeHandler_ABC& handler  )
    : QObject   ( parent )
    , layer_    ( layer )
    , handler_  ( handler )
    , menu_     ( menu )
{
    pPopupMenu_ = new QPopupMenu( parent );
}

// -----------------------------------------------------------------------------
// Name: LocationCreator constructor
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
LocationCreator::LocationCreator( QWidget* parent, ParametersLayer& layer, ShapeHandler_ABC& handler )
    : QObject   ( parent )
    , layer_    ( layer )
    , handler_  ( handler )
    , pPopupMenu_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationCreator destructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
LocationCreator::~LocationCreator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::AddLocationType
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void LocationCreator::AddLocationType( const QString& message, ASN1T_EnumTypeLocalisation type )
{
    if( !pPopupMenu_ )
        return;
    int n = pPopupMenu_->insertItem( message, this, SLOT( Start( int ) ) );
    pPopupMenu_->setItemParameter( n, (int)type );
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::NotifyContextMenu
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void LocationCreator::NotifyContextMenu( const geometry::Point2f& point, ContextMenu& menu )
{
    if( !pPopupMenu_ )
        return;
    popupPoint_ = point;
    menu.InsertItem( "Parametre", menu_.c_str(), pPopupMenu_ );
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::Start
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void LocationCreator::Start( int type )
{
    type_ = ASN1T_EnumTypeLocalisation( type );
    // Special case for point parameter.
    if( type == EnumTypeLocalisation::point )
    {
        if( !pPopupMenu_ )
            layer_.StartPoints( handler_, 1 );
        else
        {
            T_PointVector points;
            points.push_back( popupPoint_ );
            handler_.Handle( points );
        }
    }
    else
    {
        if( type == EnumTypeLocalisation::line )
            layer_.StartLine( handler_ );
        else if( type == EnumTypeLocalisation::circle )
            layer_.StartCircle( handler_ );
        else if( type == EnumTypeLocalisation::polygon )
            layer_.StartPolygon( handler_ );
    }
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::StartPoint
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void LocationCreator::StartPoint()
{
    Start( EnumTypeLocalisation::point ); // $$$$ SBO 2006-06-19: for magic move
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::Draw
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void LocationCreator::Draw( const T_PointVector& points, ASN1T_EnumTypeLocalisation type, const GlTools_ABC& tools )
{
    if( points.empty() )
        return;
    
    if( type == EnumTypeLocalisation::point )
        for( T_PointVector::const_iterator it = points.begin(); it != points.end(); ++it )
            tools.DrawCross( *it );
    else if( type == EnumTypeLocalisation::line )
        tools.DrawLines( points );
    else if( type == EnumTypeLocalisation::polygon )
    {
        tools.DrawLines( points );
        tools.DrawLine( points.back(), points.front() );
    }
    else if( type == EnumTypeLocalisation::circle && points.size() >= 2 )
        tools.DrawCircle( points.front(), points.front().Distance( points.back() ) );
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::GetCurrentType
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
ASN1T_EnumTypeLocalisation LocationCreator::GetCurrentType() const
{
    return type_;
}
