// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerShape.h"
#include "SvgLocationDrawer.h"
#include "DrawingTypes.h"
#include "DrawingCategory.h"
#include "DrawingTemplate.h"
#include "ParametersLayer.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LocationProxy.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerShape constructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerShape::DrawerShape( kernel::Controllers& controllers, unsigned long id, const DrawingTemplate& style, const QColor& color,
                          const kernel::Entity_ABC* entity, kernel::LocationProxy& location, const kernel::CoordinateConverter_ABC& coordinateConverter )
    : kernel::EntityImplementation< Drawing_ABC >( controllers.controller_, id, style.GetName() )
    , controller_         ( controllers.controller_ )
    , style_              ( style )
    , location_           ( location )
    , color_              ( color )
    , entity_             ( controllers, entity )
    , drawer_             ( new SvgLocationDrawer( style ) )
    , coordinateConverter_( coordinateConverter )
{
    RegisterSelf( *this );
}

namespace
{
    const DrawingTemplate& ReadStyle( xml::xistream& xis, const DrawingTypes& types )
    {
        std::string category, style;
        xis >> xml::attribute( "category", category )
            >> xml::attribute( "template", style );
        return types.Get( category.c_str() ).GetTemplate( style );
    }

    QColor ReadColor( xml::xistream& xis )
    {
        const std::string name = xis.attribute< std::string >( "color" );
        return QColor( name.c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerShape constructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerShape::DrawerShape( kernel::Controllers& controllers, unsigned long id, xml::xistream& xis, const kernel::Entity_ABC* entity, const DrawingTypes& types,
                          kernel::LocationProxy& proxy, const kernel::CoordinateConverter_ABC& coordinateConverter )
    : kernel::EntityImplementation< Drawing_ABC >( controllers.controller_, id, ReadStyle( xis, types ).GetName() )
    , controller_         ( controllers.controller_ )
    , style_              ( ReadStyle( xis, types ) )
    , location_           ( proxy )
    , color_              ( ReadColor( xis ) )
    , entity_             ( controllers, entity )
    , drawer_             ( new SvgLocationDrawer( style_ ) )
    , coordinateConverter_( coordinateConverter )
{
    std::auto_ptr< kernel::Location_ABC > location( style_.CreateLocation() );
    location_.SetLocation( location );
    location.release();
    xis >> xml::list( "point", *this, &DrawerShape::ReadPoint );
    RegisterSelf( *this );
    Create();
}

// -----------------------------------------------------------------------------
// Name: DrawerShape destructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerShape::~DrawerShape()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::GetColor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
QColor DrawerShape::GetColor() const
{
    return color_;
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::GetDiffusionEntity
// Created: JSR 2011-06-30
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* DrawerShape::GetDiffusionEntity() const
{
    return entity_;
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::Create
// Created: AGE 2008-05-19
// -----------------------------------------------------------------------------
void DrawerShape::Create()
{
    controller_.Create( *static_cast< Drawing_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::Update
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void DrawerShape::Update()
{
    Touch();
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::ReadPoint
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
void DrawerShape::ReadPoint( xml::xistream& xis )
{
    geometry::Point2f point;
    if( xis.has_attribute( "x" ) && xis.has_attribute( "y" ) )
    {
        float x, y;
        xis >> xml::attribute( "x", x )
            >> xml::attribute( "y", y );
        point = geometry::Point2f( x, y );
    }
    else
    {
        double latitude, longitude;
        xis >> xml::attribute( "latitude", latitude )
            >> xml::attribute( "longitude", longitude );
        point = coordinateConverter_.ConvertFromGeo( geometry::Point2d( longitude, latitude ) );
    }
    location_.AddPoint( point );
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::Translate
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
void DrawerShape::Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision )
{
    location_.Translate( from, translation, precision );
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::Draw
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void DrawerShape::Draw( const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools, bool overlined ) const
{
    drawer_->Draw( location_, viewport, tools, color_, overlined );
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::Draw
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
void DrawerShape::Draw( const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const
{
    // $$$$ SBO 2008-06-03: check viewport
    drawer_->Draw( location, viewport, tools, color_, true );
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::ChangeColor
// Created: AGE 2008-01-16
// -----------------------------------------------------------------------------
void DrawerShape::ChangeColor( const QColor& color )
{
    color_ = color;
    Update();
}

namespace
{
    struct XmlSerializer : public kernel::LocationVisitor_ABC
    {
        XmlSerializer( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter )
            : xos_      ( &xos )
            , converter_( converter )
        {}
        virtual void VisitLines( const T_PointVector& points )
        {
            for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
                VisitPoint( *it );
        }
        virtual void VisitRectangle( const T_PointVector& points )
        {
            VisitLines( points );
        }
        virtual void VisitPolygon( const T_PointVector& points )
        {
            T_PointVector copy( points );
            copy.pop_back();
            VisitLines( copy );
        }
        virtual void VisitPath( const geometry::Point2f& start, const T_PointVector& points )
        {
            VisitPoint( start );
            VisitLines( points );
        }
        virtual void VisitCircle( const geometry::Point2f& /*center*/, float /*radius*/ )
        {
            // $$$$ SBO 2008-05-30: TODO
        }
        virtual void VisitPoint( const geometry::Point2f& point )
        {
            const geometry::Point2d wgs = converter_.ConvertToGeo( point );
            *xos_ << xml::start( "point" )
                    << xml::attribute( "longitude", wgs.X() )
                    << xml::attribute( "latitude", wgs.Y() )
                  << xml::end;
        }
        xml::xostream* xos_;
        const kernel::CoordinateConverter_ABC& converter_;
    };
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::Serialize
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
void DrawerShape::Serialize( xml::xostream& xos ) const
{
    if( location_.IsValid() )
    {
        xos << xml::start( "shape" )
                << xml::attribute( "color", color_.name() );
        style_.Serialize( xos );
        XmlSerializer serializer( xos, coordinateConverter_ );
        location_.Accept( serializer );
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::Handle
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void DrawerShape::Handle( kernel::Location_ABC& location )
{
    if( location.IsValid() )
    {
        std::auto_ptr< kernel::Location_ABC > ptr( &location );
        location_.SetLocation( ptr );
        Create();
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::Edit
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
void DrawerShape::Edit( ParametersLayer& parameters )
{
    controller_.Delete( *static_cast< Drawing_ABC* >( this ) );
    parameters.Start( *this, location_ );
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::GetLocation
// Created: AGE 2008-08-19
// -----------------------------------------------------------------------------
const kernel::Location_ABC& DrawerShape::GetLocation() const
{
    return location_;
}
