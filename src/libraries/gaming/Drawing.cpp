// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Drawing.h"
#include "clients_gui/DrawingTypes.h"
#include "clients_gui/DrawingCategory.h"
#include "clients_gui/DrawingTemplate.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/LocationProxy.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
Drawing::Drawing( kernel::Controllers& controllers, const sword::ShapeCreation& message, const kernel::Entity_ABC* entity, const gui::DrawingTypes& types, kernel::LocationProxy& proxy, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter )
    : gui::DrawerShape( controllers, message.id().id(), types.Get( message.shape().category().c_str() ).GetTemplate( message.shape().pattern() ), QColor( message.shape().color().red(), message.shape().color().green(), message.shape().color().blue() ), entity, proxy, converter )
    , publisher_    ( publisher )
    , converter_    ( converter )
    , publishUpdate_( true )
{
    SetLocation( message.shape().points() );
    DrawerShape::Create();
}

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
Drawing::Drawing( kernel::Controllers& controllers, const gui::DrawingTemplate& style, const QColor& color, const kernel::Entity_ABC* entity, kernel::LocationProxy& proxy, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter )
    : gui::DrawerShape( controllers, 0, style, color, entity, proxy, converter )
    , publisher_    ( publisher )
    , converter_    ( converter )
    , publishUpdate_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
Drawing::Drawing( kernel::Controllers& controllers, xml::xistream& xis, const kernel::Entity_ABC* entity, const gui::DrawingTypes& types, kernel::LocationProxy& proxy, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter )
    : gui::DrawerShape( controllers, 0, xis, entity, types, proxy, converter )
    , publisher_    ( publisher )
    , converter_    ( converter )
    , publishUpdate_( true )
{
    Create();
}

// -----------------------------------------------------------------------------
// Name: Drawing destructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
Drawing::~Drawing()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Drawing::SetLocation
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void Drawing::SetLocation( const sword::CoordLatLongList& list )
{
    std::auto_ptr< kernel::Location_ABC > location( style_.CreateLocation() );
    location_.SetLocation( location );
    location.release();
    for( int i = 0; i < list.elem_size(); ++i )
        location_.AddPoint( converter_.ConvertToXY( list.elem( i ) ) );
}

// -----------------------------------------------------------------------------
// Name: Drawing::NotifyDestruction
// Created: JSR 2011-06-30
// -----------------------------------------------------------------------------
void Drawing::NotifyDestruction() const
{
    plugins::messenger::ShapeDestructionRequest message;
    message().mutable_id()->set_id( GetId() );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: Drawing::Create
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void Drawing::Create()
{
    plugins::messenger::ShapeCreationRequest message;
    message().mutable_shape()->set_category( style_.GetCategory().toAscii().constData() );
    message().mutable_shape()->mutable_color()->set_red( color_.red() );
    message().mutable_shape()->mutable_color()->set_green( color_.green() );
    message().mutable_shape()->mutable_color()->set_blue( color_.blue() );
    message().mutable_shape()->set_pattern( style_.GetName().toAscii().constData() );
    if( entity_ )
    {
        if( entity_->GetTypeName() == kernel::Automat_ABC::typeName_ )
            message().mutable_shape()->mutable_diffusion()->mutable_automat()->set_id( entity_->GetId() );
        else if( entity_->GetTypeName() == kernel::Formation_ABC::typeName_ )
            message().mutable_shape()->mutable_diffusion()->mutable_formation()->set_id( entity_->GetId() );
    }
    SerializeLocation( *message().mutable_shape()->mutable_points() );
    message.Send( publisher_ );
    delete this;
}

// -----------------------------------------------------------------------------
// Name: Drawing::Update
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void Drawing::Update()
{
    DrawerShape::Update();
    if( publishUpdate_ )
    {
        plugins::messenger::ShapeUpdateRequest message;
        message().mutable_shape()->set_id( GetId() );
        message().set_category( style_.GetCategory().toAscii().constData() );
        message().mutable_color()->set_red( color_.red() );
        message().mutable_color()->set_green( color_.green() );
        message().mutable_color()->set_blue( color_.blue() );
        message().set_pattern( style_.GetName().toAscii().constData() );
        SerializeLocation( *message().mutable_points() );
        message.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: Drawing::DoUpdate
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void Drawing::DoUpdate( const sword::ShapeUpdate& message )
{
    publishUpdate_ = false;
     // $$$$ SBO 2008-06-09: can only change color and shape
    if( message.shape().has_color()  )
        ChangeColor( QColor( message.shape().color().red(), message.shape().color().green(), message.shape().color().blue() ) );
    if( message.shape().has_points()  )
        SetLocation( message.shape().points() );
    publishUpdate_ = true;
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
        virtual void VisitCircle( const geometry::Point2f& /*center*/, float /*radius*/ )
        {
            // $$$$ SBO 2008-06-05: TODO
        }
        virtual void VisitPoint( const geometry::Point2f& point )
        {
            VisitLines( T_PointVector( 1, point ) );
        }
        virtual void VisitPath( const geometry::Point2f& , const T_PointVector& points )
        {
            VisitLines( points );
        }
        std::vector< sword::CoordLatLong > points_;
        const kernel::CoordinateConverter_ABC* converter_;
    };
}

// -----------------------------------------------------------------------------
// Name: Drawing::SerializeLocation
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void Drawing::SerializeLocation( sword::CoordLatLongList& list ) const
{
    Serializer serializer( converter_ );
    location_.Accept( serializer );
    for( unsigned int i = 0; i < serializer.points_.size(); ++i )
        *list.add_elem() = serializer.points_[i];
}
