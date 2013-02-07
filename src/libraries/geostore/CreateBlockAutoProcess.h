// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __CreateBlockAutoProcess_h_
#define __CreateBlockAutoProcess_h_

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <string>

class PointProjector_ABC;
class UrbanModel;

namespace kernel
{
    class UrbanObject_ABC;
}

namespace geostore
{
    class GeometryFactory;
    class SpatialIndexer;
    class GeoTable;
    typedef boost::ptr_map< std::string, GeoTable > T_GeoTables;

// =============================================================================
/** @class  CreateBlockAutoProcess
    @brief  CreateBlockAutoProcess
*/
// Created: AME 2010-08-02
// =============================================================================
class CreateBlockAutoProcess : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             CreateBlockAutoProcess( const T_GeoTables& tables, const SpatialIndexer& index, PointProjector_ABC& projector, double roadWidth );
    virtual ~CreateBlockAutoProcess();
    //@}

    //! @name Operations
    //@{
    void Run( const geometry::Polygon2f& footprint, std::vector< geometry::Polygon2f >& blocks );
    //@}

private:
    //! @name Helpers
    //@{
    void ExtractTerrainComponents( gaiaGeomCollPtr footprint, gaiaGeomCollPtr& areas, gaiaGeomCollPtr& buffers );
    gaiaGeomCollPtr SubstractTerrainComponentsFromAreas( gaiaGeomCollPtr urbans, gaiaGeomCollPtr areas, gaiaGeomCollPtr lines );
    gaiaGeomCollPtr ClipBlocksWithCollection( gaiaGeomCollPtr blocks, gaiaGeomCollPtr collection );
    gaiaGeomCollPtr GetUrbanBlocksInAreaFromIndex( const geometry::Polygon2f& footprint );
    void FillPolygonVector( gaiaGeomCollPtr blocks, std::vector< geometry::Polygon2f >& vec );
    //@}

private:
    //! @name Member data
    //@{
    const T_GeoTables&      tables_;
    const SpatialIndexer&   index_;
    PointProjector_ABC&     projector_;
    double                  roadWidth_;

    std::auto_ptr< GeometryFactory > geometryFactory_;
    //@}
};

}

#endif // __CreateBlockAutoProcess_h_
