// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DrawingFactory.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/LocationProxy.h"
#include "clients_gui/DrawingTemplate.h"
#include "clients_gui/DrawingHelper.h"
#include "DrawingPositions.h"
#include "Drawing.h"

// -----------------------------------------------------------------------------
// Name: DrawingFactory constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingFactory::DrawingFactory( kernel::Controllers& controllers, const gui::DrawingTypes& types, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter, const kernel::EntityResolver_ABC& resolver )
    : controllers_( controllers )
    , types_      ( types )
    , publisher_  ( publisher )
    , converter_  ( converter )
    , resolver_   ( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingFactory destructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingFactory::~DrawingFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingFactory::CreateShape
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
kernel::Drawing_ABC* DrawingFactory::CreateShape( const sword::ShapeCreation& message ) const
{
    const kernel::Entity_ABC* diffusionEntity = 0;
    if( message.shape().has_diffusion() )
    {
        if( message.shape().diffusion().has_automat() )
            diffusionEntity = resolver_.FindAutomat( message.shape().diffusion().automat().id() );
        else if( message.shape().diffusion().has_formation() )
            diffusionEntity = resolver_.FindFormation( message.shape().diffusion().formation().id() );
    }
    DrawingPositions* location = new DrawingPositions( converter_, message );
    Drawing* drawing = new Drawing( controllers_, message, diffusionEntity, types_, *location, publisher_, converter_ );
    drawing->Attach< kernel::Positions >( *location );
    drawing->Polish();
    return drawing;
}

namespace
{
    struct Serializer : public kernel::LocationVisitor_ABC
    {
        explicit Serializer( const kernel::CoordinateConverter_ABC& converter ) : converter_( &converter ) {}
        virtual void VisitLines( const T_PointVector& points )
        {
            for( T_PointVector::const_iterator it = points.begin(); it != points.end(); ++it )
            {
                sword::CoordLatLong latlong;
                converter_->ConvertToGeo( *it, latlong );
                points_.push_back( latlong );
            }
        }
        virtual void VisitRectangle( const T_PointVector& points )
        {
            VisitLines( points );
        }
        virtual void VisitPolygon( const T_PointVector& points )
        {
            VisitLines( points );
            points_.pop_back();
        }
        virtual void VisitCircle( const geometry::Point2f& center, float radius )
        {
            VisitPoint( center );
            const geometry::Point2f point( center + geometry::Vector2f( 0, 1.f ) * radius );
            VisitPoint( point );
        }
        virtual void VisitPoint( const geometry::Point2f& point )
        {
            VisitLines( T_PointVector( 1, point ) );
        }
        virtual void VisitPath( const geometry::Point2f& , const T_PointVector& points )
        {
            VisitLines( points );
        }
        virtual void VisitCurve( const T_PointVector& points )
        {
            VisitLines( points );
        }
        std::vector< sword::CoordLatLong > points_;
        const kernel::CoordinateConverter_ABC* converter_;
    };

    void SerializeLocation( sword::CoordLatLongList& list, const kernel::CoordinateConverter_ABC& converter, kernel::Location_ABC& location )
    {
        Serializer serializer( converter );
        location.Accept( serializer );
        for( auto i = 0; i < serializer.points_.size(); ++i )
            *list.add_elem() = serializer.points_[i];
    }
}

// -----------------------------------------------------------------------------
// Name: DrawingFactory::CreateShape
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
void DrawingFactory::CreateShape( const gui::DrawingTemplate& style, const QColor& color, const kernel::Entity_ABC* entity,
                                  gui::E_Dash_style dashStyle, kernel::Location_ABC& location ) const
{
    plugins::messenger::ShapeCreationRequest message;
    message().mutable_shape()->set_category( style.GetCategory().toStdString() );
    message().mutable_shape()->mutable_color()->set_red( color.red() );
    message().mutable_shape()->mutable_color()->set_green( color.green() );
    message().mutable_shape()->mutable_color()->set_blue( color.blue() );
    message().mutable_shape()->set_pattern( style.GetName().toStdString() );
    if( dashStyle != gui::eSolid )
        message().mutable_shape()->set_pen_style( sword::EnumPenStyle( dashStyle ) );
    if( entity )
    {
        if( entity->GetTypeName() == kernel::Automat_ABC::typeName_ )
            message().mutable_shape()->mutable_diffusion()->mutable_automat()->set_id( entity->GetId() );
        else if( entity->GetTypeName() == kernel::Formation_ABC::typeName_ )
            message().mutable_shape()->mutable_diffusion()->mutable_formation()->set_id( entity->GetId() );
    }
    SerializeLocation( *message().mutable_shape()->mutable_points(), converter_, location );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: DrawingFactory::CreateShape
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
void DrawingFactory::CreateShape( xml::xistream& xis, const kernel::Entity_ABC* entity ) const
{
    kernel::LocationProxy location;
    gui::ReadLocation( xis, location, converter_ );
    CreateShape( gui::ReadStyle( xis, types_ ), gui::ReadColor( xis ), entity,
        gui::ReadDashStyle( xis ) ,location );
}
