// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DrawingTools.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "protocol/MessengerSenders.h"
#include <boost/optional.hpp>

namespace
{
    struct Serializer : public kernel::LocationVisitor_ABC
    {
        explicit Serializer( const kernel::CoordinateConverter_ABC& converter )
            : converter_( converter ) {}
        virtual void VisitLines( const T_PointVector& points )
        {
            for( T_PointVector::const_iterator it = points.begin(); it != points.end(); ++it )
            {
                sword::CoordLatLong latlong;
                converter_.ConvertToGeo( *it, latlong );
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
        virtual void VisitText( const QString& text, const QFont& font, const geometry::Point2f& point )
        {
            VisitPoint( point );
            font_ = font.toString().toStdString();
            text_ = text.toStdString();
        }

        const kernel::CoordinateConverter_ABC& converter_;
        std::vector< sword::CoordLatLong > points_;
        boost::optional< std::string > font_;
        boost::optional< std::string > text_;
    };

    template< typename T >
    void SerializeLocation( T& message, const kernel::CoordinateConverter_ABC& converter,
                            kernel::Location_ABC& location )
    {
        sword::CoordLatLongList* points = message.mutable_points();
        Serializer serializer( converter );
        location.Accept( serializer );
        for( auto i = 0u; i < serializer.points_.size(); ++i )
            *points->add_elem() = serializer.points_[i];
        if( serializer.font_ )
            message.set_font( *serializer.font_ );
        if( serializer.text_ )
            message.set_text( *serializer.text_ );
    }
}

void tools::SerializeLocation( plugins::messenger::ShapeCreationRequest& message,
                               const kernel::CoordinateConverter_ABC& converter,
                               kernel::Location_ABC& location )
{
    ::SerializeLocation( *message().mutable_shape(), converter, location );
}

void tools::SerializeLocation( plugins::messenger::ShapeUpdateRequest& message,
                               const kernel::CoordinateConverter_ABC& converter,
                               kernel::Location_ABC& location )
{
    ::SerializeLocation( message(), converter, location );
}
