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

#include "GeometryCollection.h"
#include <geometry/Types.h>
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
    void ExtractTerrainComponents( GeometryCollection footprint, GeometryCollection& areas, GeometryCollection& buffers );
    GeometryCollection SubstractTerrainComponentsFromAreas( GeometryCollection urbans, GeometryCollection areas, GeometryCollection lines );
    GeometryCollection ClipBlocksWithCollection( GeometryCollection blocks, GeometryCollection collection );
    GeometryCollection GetUrbanBlocksInAreaFromIndex( const geometry::Polygon2f& footprint );
    void FillPolygonVector( GeometryCollection blocks, std::vector< geometry::Polygon2f >& vec );
    //@}

private:
    //! @name Member data
    //@{
    const T_GeoTables&      tables_;
    const SpatialIndexer&   index_;
    PointProjector_ABC&     projector_;
    double                  roadWidth_;

    std::unique_ptr< GeometryFactory > geometryFactory_;
    //@}
};

}

#endif // __CreateBlockAutoProcess_h_
