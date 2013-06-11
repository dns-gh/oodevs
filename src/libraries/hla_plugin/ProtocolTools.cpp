// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "ProtocolTools.h"

#pragma warning( push, 0 )
#include "protocol/proto/common.pb.h"
#pragma warning( pop )

using namespace plugins::hla;

namespace
{
    geometry::Point2d doConvert( const sword::Location& location )
    {
        geometry::Point2d retval(0.,0.);
        switch( location.type() )
        {
        case sword::Location_Geometry_circle:
            break;
        case sword::Location_Geometry_point:
            retval = geometry::Point2d( location.coordinates().elem( 0 ).latitude(),
                                        location.coordinates().elem( 0 ).longitude() );
            break;
        case sword::Location_Geometry_polygon:
            {
                double x=0., y=0;
                const int sz = location.coordinates().elem_size();
                for( int i=0; i<sz; ++i)
                {
                    const sword::CoordLatLong& coord = location.coordinates().elem( i );
                    x+=coord.latitude();
                    y+=coord.longitude();
                }
                retval = geometry::Point2d( x/sz, y/sz );
            }
            break;
        }
        return retval;
    }
}

geometry::Point2d ProtocolTools::ConvertToPoint( const sword::MissionParameter& parameter )
{
    geometry::Point2d retval;
    if( !parameter.null_value() && parameter.value_size() != 0 )
    {
        const sword::MissionParameter_Value& value = parameter.value(0);
        if( value.has_location() )
            retval = doConvert( value.location() );
        if( value.has_point() )
            retval = doConvert( value.point().location() );
        if( value.has_area() )
            retval = doConvert( value.area().location() );
    }
    return retval;
}