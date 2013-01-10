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

class PointProjector_ABC;
class UrbanModel;

namespace kernel
{
    class UrbanObject_ABC;
}

namespace geostore
{
    class Database;
    class GeometryFactory;
    class SpatialIndexer;

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
             CreateBlockAutoProcess( const Database& database, const SpatialIndexer& index, PointProjector_ABC& projector, double roadWidth );
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
    gaiaGeomCollPtr GetUrbanBlocksInAreaFromIndex( gaiaGeomCollPtr blocks, const geometry::Polygon2f& footprint );
    void FillPolygonVector( gaiaGeomCollPtr blocks, std::vector< geometry::Polygon2f >& vec );
    //@}

private:
    //! @name Member data
    //@{
    const Database&         database_;
    const SpatialIndexer&   index_;
    PointProjector_ABC&     projector_;
    double                  roadWidth_;

    std::auto_ptr< GeometryFactory > geometryFactory_;
    //@}
};

}

#endif // __CreateBlockAutoProcess_h_
