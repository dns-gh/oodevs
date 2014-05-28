// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef KERNEL_XML_ADAPTER_H__
#define KERNEL_XML_ADAPTER_H__

#include "CoordinateConverter_ABC.h"
#include "EntityResolver_ABC.h"
#include "protocol/XmlReaders.h"
#include "protocol/XmlWriters.h"
#include <tools/Exception.h>

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

// =============================================================================
/** @class  XmlWriterAdapter
    @brief  XmlAdapter
*/
// =============================================================================
class XmlWriterAdapter : public protocol::Writer_ABC
{
public:
    XmlWriterAdapter( const kernel::CoordinateConverter_ABC& converter )
        : converter_( converter )
    {
        // NOTHING
    }

    virtual std::string Convert( double x, double y ) const
    {
        const geometry::Point2d geo( x, y );
        const geometry::Point2f xy = converter_.ConvertFromGeo( geo );
        return converter_.ConvertToMgrs( xy );
    }

private:
    const kernel::CoordinateConverter_ABC& converter_;
};

struct XmlWriterEmptyAdapter : public protocol::Writer_ABC
{
    virtual std::string Convert( double /*x*/, double /*y*/ ) const
    {
        throw MASA_EXCEPTION_NOT_IMPLEMENTED;
    }
};
}

#endif // KERNEL_XML_ADAPTER_H__
