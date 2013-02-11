// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef KERNEL_XML_ADAPTER_H__
#define KERNEL_XML_ADAPTER_H__

#include "CoordinateConverter_ABC.h"
#include "EntityResolver_ABC.h"
#include "protocol/XmlReaders.h"

namespace kernel
{
// =============================================================================
/** @class  XmlAdapter
    @brief  XmlAdapter
*/
// Created: BAX 2013-02-11
// =============================================================================
class XmlAdapter : public protocol::Reader_ABC
{
public:
    XmlAdapter( const kernel::CoordinateConverter_ABC& converter,
                const kernel::EntityResolver_ABC& resolver )
        : converter_( converter )
        , resolver_ ( resolver )
    {
        // NOTHING
    }

    virtual Point Convert( const std::string& value ) const
    {
        const geometry::Point2f xy = converter_.ConvertToXY( value );
        const geometry::Point2d geo = converter_.ConvertToGeo( xy );
        const Point rpy = { geo.X(), geo.Y() };
        return rpy;
    }

    virtual EntityType Resolve( uint32_t id ) const
    {
        const unsigned uid = static_cast< unsigned >( id );
        if( resolver_.FindAgent( uid ) )
            return UNIT;
        if( resolver_.FindAutomat( uid ) )
            return AUTOMAT;
        if( resolver_.FindFormation( uid ) )
            return FORMATION;
        if( resolver_.FindInhabitant( uid ) )
            return POPULATION;
        if( resolver_.FindPopulation( uid ) )
            return CROWD;
        if( resolver_.FindTeam( uid ) )
            return PARTY;
        return COUNT;
    }

private:
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::EntityResolver_ABC&      resolver_;
};
}

#endif // KERNEL_XML_ADAPTER_H__
