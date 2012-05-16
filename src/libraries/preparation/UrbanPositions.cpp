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
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <boost/bind.hpp>

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
// Name: UrbanPositions constructor
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( xml::xistream& xis, EUrbanLevel level, const kernel::UrbanObject_ABC& object, const kernel::CoordinateConverter_ABC& converter )
    : kernel::UrbanPositions( level, object, Convert( xis, level, converter ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions destructor
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
UrbanPositions::~UrbanPositions()
{
    // NOTHING
}
