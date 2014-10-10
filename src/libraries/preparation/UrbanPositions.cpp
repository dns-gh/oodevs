// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanPositions.h"

#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <boost/bind.hpp>
#include "geometry/Polygon2.h"

namespace
{
    void ReadPoint( xml::xistream& xis, std::vector< geometry::Point2f >& positions, const kernel::CoordinateConverter_ABC& converter )
    {
        positions.push_back( converter.ConvertToXY( xis.attribute< std::string >( "location" ) ) );
    }

    std::vector< geometry::Point2f > Convert( xml::xistream& xis, EUrbanLevel level, const kernel::CoordinateConverter_ABC& converter )
    {
        std::vector< geometry::Point2f > positions;
        if( level == eUrbanLevelBlock && xis.has_child( "footprint" ) )
        {
            xis >> xml::start( "footprint" )
                    >> xml::list( "point", boost::bind( &ReadPoint, _1, boost::ref( positions ), boost::ref( converter ) ) )
                >> xml::end;
        }
        return positions;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor for city or district from list view context menu
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( gui::PropertiesDictionary& dictionary, EUrbanLevel level, const kernel::UrbanObject_ABC& object,
                                const kernel::CoordinateConverter_ABC& converter, gui::Tesselator& tesselator )
    : gui::UrbanPositions( level, object, tesselator )
    , converter_( converter )
    , level_( level )
{
    assert( level_ == eUrbanLevelCity || level_ == eUrbanLevelDistrict );
    dictionary.Register( object, tools::translate( "UrbanPositions", "Info/Area" ), area_, true );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor for urban blocs from buttons
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( const geometry::Polygon2f& location, gui::PropertiesDictionary& dictionary, EUrbanLevel level,
                                const kernel::UrbanObject_ABC& object, const kernel::CoordinateConverter_ABC& converter, gui::Tesselator& tesselator )
    : gui::UrbanPositions( level, object, location.Vertices(), tesselator )
    , converter_( converter )
    , level_( level )
{
    assert( level_ == eUrbanLevelBlock );
    dictionary.Register( object, tools::translate( "UrbanPositions", "Info/Area" ), area_, true );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor for any urban object from xis
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( xml::xistream& xis, gui::PropertiesDictionary& dictionary, EUrbanLevel level, const kernel::UrbanObject_ABC& object,
                                const kernel::CoordinateConverter_ABC& converter, gui::Tesselator& tesselator )
    : gui::UrbanPositions( level, object, Convert( xis, level, converter ), tesselator )
    , converter_( converter )
    , level_( level )
{
    dictionary.RegisterExtension( object, this, tools::translate( "UrbanPositions", "Info/Area" ), area_, true );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions destructor
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
UrbanPositions::~UrbanPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::SerializeAttributes
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void UrbanPositions::SerializeAttributes( xml::xostream& xos ) const
{
    if( level_ == eUrbanLevelBlock )
    {
        xos << xml::start( "footprint" );
        const geometry::Polygon2f::T_Vertices& locations = polygon_.Vertices();
        for( geometry::Polygon2f::CIT_Vertices it = locations.begin(); it != locations.end(); ++it )
            xos << xml::start( "point" )
                    << xml::attribute( "location", converter_.ConvertToMgrs( *it ) )
                << xml::end;
        xos << xml::end; // footprint
    }
}
