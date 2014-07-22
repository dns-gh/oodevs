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

#include "DrawingCategory.h"
#include "DrawingHelper.h"
#include "DrawingTemplate.h"
#include "DrawingTypes.h"
#include "GlTools_ABC.h"
#include "ParametersLayer.h"
#include "SvgLocationDrawer.h"

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LocationProxy.h"

#include <xeumeuleu/xml.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerShape constructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerShape::DrawerShape( kernel::Controllers& controllers, unsigned long id, const QString& name, const DrawingTemplate& style, const QColor& color,
                          const kernel::Entity_ABC* entity, kernel::LocationProxy& location, const kernel::CoordinateConverter_ABC& coordinateConverter,
                          E_Dash_style dashStyle )
    : Drawing( controllers.controller_, id, name, dashStyle )
    , style_              ( style )
    , location_           ( location )
    , color_              ( color )
    , template_           ( style_.GetName() )
    , entity_             ( controllers, entity )
    , drawer_             ( new SvgLocationDrawer( style ) )
    , coordinateConverter_( coordinateConverter )
    , isEditing_          ( false )
{
    AddExtension( *this );
    CreateDictionary();
}

namespace
{
    QString ReadName( xml::xistream& xis, const DrawingTypes& types )
    {
        if( !xis.has_attribute( "name" ) )
            return gui::ReadStyle( xis, types ).GetName();
        return xis.attribute< std::string >( "name" ).c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerShape constructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerShape::DrawerShape( kernel::Controllers& controllers, unsigned long id, xml::xistream& xis, const kernel::Entity_ABC* entity, const DrawingTypes& types,
                          kernel::LocationProxy& proxy, const kernel::CoordinateConverter_ABC& coordinateConverter )
    : Drawing( controllers.controller_, id, ReadName( xis, types ), gui::ReadDashStyle( xis ) )
    , style_              ( gui::ReadStyle( xis, types ) )
    , location_           ( proxy )
    , color_              ( gui::ReadColor( xis ) )
    , template_           ( style_.GetName() )
    , entity_             ( controllers, entity )
    , drawer_             ( new SvgLocationDrawer( style_ ) )
    , coordinateConverter_( coordinateConverter )
    , isEditing_          ( false )
{
    std::unique_ptr< kernel::Location_ABC > location( style_.CreateLocation() );
    location_.SetLocation( location );
    gui::ReadLocation( xis, location_, coordinateConverter );
    if( !location_.IsValid() )
        throw MASA_EXCEPTION( "shape '" + GetName().toStdString() + "' geometry is invalid" );
    AddExtension( *this );
    CreateDictionary();
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
// Name: DrawerShape::Update
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void DrawerShape::Update()
{
    Touch();
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
void DrawerShape::Draw( const geometry::Rectangle2f& viewport, const GlTools_ABC& tools, bool overlined ) const
{
    if( !isEditing_ )
        drawer_->Draw( location_, viewport, tools, tools.IsPickingMode() ? tools.GetPickingColor() : color_,
            overlined, dashStyle_, tools.GetAdaptiveZoomFactor() );
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::Draw
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
void DrawerShape::Draw( const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    // $$$$ SBO 2008-06-03: check viewport
    drawer_->Draw( location, viewport, tools, color_, true, dashStyle_, 1.f );
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::ChangeColor
// Created: AGE 2008-01-16
// -----------------------------------------------------------------------------
void DrawerShape::ChangeColor( const QColor& color )
{
    if( color_ != color )
    {
        color_ = color;
        Update();
    }
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
            for( auto it = points.begin(); it != points.end(); ++it )
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
        virtual void VisitCircle( const geometry::Point2f& center, float radius )
        {
            VisitPoint( center );
            const geometry::Point2f point( center + geometry::Vector2f( 0, 1.f ) * radius );
            VisitPoint( point );
        }
        virtual void VisitPoint( const geometry::Point2f& point )
        {
            const geometry::Point2d wgs = converter_.ConvertToGeo( point );
            *xos_ << xml::start( "point" )
                    << xml::attribute( "longitude", wgs.X() )
                    << xml::attribute( "latitude", wgs.Y() )
                  << xml::end;
        }
        virtual void VisitCurve( const T_PointVector& points )
        {
            for( auto it = points.begin(); it != points.end(); ++it )
                VisitPoint( *it );
        }
        virtual void VisitText( const QString& text, const QFont& font, const geometry::Point2f& point )
        {
            VisitPoint( point );
            *xos_ << xml::start( "text" )
                    << xml::attribute( "font", font.toString() )
                    << text
                  << xml::end;
        }
        xml::xostream* xos_;
        const kernel::CoordinateConverter_ABC& converter_;
    };

    std::string SerializeDash( E_Dash_style style )
    {
        if( style == eDashed )
            return "dashed";
        if( style == eDash_dot )
            return "dash_dot";
        return "solid";
    }
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
                << xml::attribute( "color", color_.name() )
                << xml::attribute( "name", name_ );
        if( dashStyle_ != eSolid )
            xos << xml::attribute( "style", SerializeDash( dashStyle_ ) );
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
        std::unique_ptr< kernel::Location_ABC > ptr( &location );
        location_.SetLocation( ptr );
        Update();
        isEditing_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::Reset
// Created: LGY 2014-05-14
// -----------------------------------------------------------------------------
void DrawerShape::Reset()
{
    isEditing_ = false;
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::Edit
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
void DrawerShape::Edit( ParametersLayer& parameters )
{
    parameters.Start( *this, location_, color_ );
    isEditing_ = true;
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::GetLocation
// Created: AGE 2008-08-19
// -----------------------------------------------------------------------------
const kernel::Location_ABC& DrawerShape::GetLocation() const
{
    return location_;
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::GetDrawingTemplate
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
const DrawingTemplate& DrawerShape::GetDrawingTemplate() const
{
    return style_;
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::CreateDictionary
// Created: LGY 2014-05-16
// -----------------------------------------------------------------------------
void DrawerShape::CreateDictionary()
{
    PropertiesDictionary& dictionary = Get< gui::PropertiesDictionary >();
    const kernel::Entity_ABC& constEntity = *static_cast< const kernel::Entity_ABC* >( this );
    dictionary.Register( constEntity, tools::translate( "Info", "Info/Template" ), template_ );
}
