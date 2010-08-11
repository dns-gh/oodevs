// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TacticalLine_ABC.h"
#include "TacticalLineHierarchies.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Positions.h"
#include <geometry/Types.h>

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( const QString& baseName, unsigned long id, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , publisher_( publisher )
    , id_( id )
{
    RegisterSelf( *this );
    name_ = ( baseName + " %1" ).arg( id_ & 0x3FFFFF );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC destructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::~TacticalLine_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Create
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Create()
{
    UpdateToSim( eStateCreated );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Delete
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Delete()
{
    UpdateToSim( eStateDeleted );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetId
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
unsigned long TacticalLine_ABC::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetName
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
QString TacticalLine_ABC::GetName() const
{
    return name_;
}

namespace
{
    struct GeometrySerializer : public kernel::LocationVisitor_ABC
    {
        GeometrySerializer( Common::MsgLocation& loc, const kernel::CoordinateConverter_ABC& converter )
            : loc_( loc )
            , converter_( converter )
        {}

        virtual void VisitLines( const T_PointVector& points )
        {
            loc_.set_type( Common::MsgLocation_Geometry_line );
            for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
                converter_.ConvertToGeo( *it, *loc_.mutable_coordinates()->add_elem() );
        }

        virtual void VisitRectangle( const T_PointVector& ) {}
        virtual void VisitPolygon( const T_PointVector& ) {}
        virtual void VisitCircle( const geometry::Point2f&, float ) {}
        virtual void VisitPoint( const geometry::Point2f& ) {}
        virtual void VisitPath( const geometry::Point2f&, const T_PointVector& ) {}

    private:
        GeometrySerializer& operator=( const GeometrySerializer& );

        const kernel::CoordinateConverter_ABC& converter_;
        Common::MsgLocation& loc_;
    };
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::WriteGeometry
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::WriteGeometry( Common::MsgLocation& location ) const
{
    GeometrySerializer serializer( location, converter_ );
    Get< kernel::Positions >().Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::WriteDiffusion
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLine_ABC::WriteDiffusion( Common::MsgTacticalLine::Diffusion& diffusion ) const
{
    // $$$$ SBO 2006-11-06: visitor or something
    static_cast< const TacticalLineHierarchies& >( Get< kernel::TacticalHierarchies >() ).WriteTo( diffusion );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::DoUpdate
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLine_ABC::DoUpdate( const MsgsMessengerToClient::MsgLimaUpdate& message )
{
    name_ = message.tactical_line().name().c_str();
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::DoUpdate
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLine_ABC::DoUpdate( const MsgsMessengerToClient::MsgLimitUpdate& message )
{
    name_ = message.tactical_line().name().c_str();
}
