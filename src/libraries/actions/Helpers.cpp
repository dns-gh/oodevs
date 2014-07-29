// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "Helpers.h"

#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Equipments_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Entity_ABC.h"
#include "protocol/Protocol.h"

#include <boost/algorithm/string.hpp>

void actions::parameters::FillFromPointList( T_PointVector& vector,
                                             const sword::PointList& list,
                                             const kernel::CoordinateConverter_ABC& converter )
{
    for( int i = 0; i < list.elem_size(); ++i )
    {
        const sword::Location& location = list.elem( i ).location();
        if( location.type() != sword::Location_Geometry_point ||
            !location.has_coordinates() ||
            location.coordinates().elem_size() != 1 )
            throw MASA_EXCEPTION( "Invalid location type" );
        vector.push_back( converter.ConvertToXY( location.coordinates().elem( 0 ) ) );
    }
}

void actions::parameters::FillPathfindRequest( sword::PathfindRequest& dst,
                                               uint32_t unit,
                                               const kernel::CoordinateConverter_ABC& converter,
                                               const kernel::Entity_ABC& entity,
                                               const std::vector< geometry::Point2f >& points,
                                               const std::string& name )
{
    dst.mutable_unit()->set_id( unit );
    dst.set_name( name );
    for( auto it = points.begin(); it != points.end(); ++it )
        converter.ConvertToGeo( *it, *dst.add_positions() );
    auto& equipments = entity.Get< kernel::Equipments_ABC >();
    equipments.Visit( [&]( const kernel::EquipmentType& type ) -> bool {
        dst.add_equipment_types()->set_id( type.GetId() );
        return true;
    });
    dst.set_ignore_dynamic_objects( true );
}