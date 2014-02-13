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