// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TacticalLine_ABC.h"
#include "TacticalLineHierarchies.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Positions.h"
#include "protocol/Protocol.h"
#include <geometry/Types.h>

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( kernel::Controller& controller, const QString& baseName, unsigned long id,
                                    Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter,
                                    bool readOnly )
    : gui::EntityImplementation< kernel::TacticalLine_ABC >( controller, id, baseName, readOnly )
    , controller_( controller )
    , converter_ ( converter )
    , publisher_ ( publisher )
    , id_        ( id )
{
    AddExtension( *this );
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
// Name: TacticalLine_ABC::NotifyDestruction
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void TacticalLine_ABC::NotifyDestruction()
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

namespace
{
    struct GeometrySerializer : public kernel::LocationVisitor_ABC
    {
        GeometrySerializer( sword::Location& loc, const kernel::CoordinateConverter_ABC& converter )
            : loc_( loc )
            , converter_( converter )
        {}

        virtual void VisitLines( const T_PointVector& points )
        {
            loc_.set_type( sword::Location_Geometry_line );
            for( auto it = points.begin(); it != points.end(); ++it )
                converter_.ConvertToGeo( *it, *loc_.mutable_coordinates()->add_elem() );
        }

        virtual void VisitRectangle( const T_PointVector& ) {}
        virtual void VisitPolygon( const T_PointVector& ) {}
        virtual void VisitCircle( const geometry::Point2f&, float ) {}
        virtual void VisitPoint( const geometry::Point2f& ) {}
        virtual void VisitPath( const geometry::Point2f&, const T_PointVector& ) {}
        virtual void VisitCurve( const T_PointVector& ) {}

    private:
        GeometrySerializer& operator=( const GeometrySerializer& );

        const kernel::CoordinateConverter_ABC& converter_;
        sword::Location& loc_;
    };
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::WriteGeometry
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::WriteGeometry( sword::Location& location ) const
{
    GeometrySerializer serializer( location, converter_ );
    Get< kernel::Positions >().Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::WriteDiffusion
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLine_ABC::WriteDiffusion( sword::Diffusion& diffusion ) const
{
    // $$$$ SBO 2006-11-06: visitor or something
    static_cast< const TacticalLineHierarchies& >( Get< kernel::TacticalHierarchies >() ).WriteTo( diffusion );
}

void TacticalLine_ABC::UpdateName( const std::string& name )
{
    name_ = name.c_str();
    controller_.Update( gui::DictionaryUpdated( *this, tools::translate( "EntityImplementation", "Info" ) ) );
    Touch();
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::DoUpdate
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLine_ABC::DoUpdate( const sword::PhaseLineUpdate& message )
{
    UpdateName( message.tactical_line().name() );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::DoUpdate
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLine_ABC::DoUpdate( const sword::LimitUpdate& message )
{
    UpdateName( message.tactical_line().name() );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Rename
// Created: LGY 2014-05-19
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Rename( const QString& name )
{
    name_ = name;
    UpdateToSim( eStateModified );

}
