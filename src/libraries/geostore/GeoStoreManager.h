// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __GeoStoreManager_h_
#define __GeoStoreManager_h_

#include <geometry/Types.h>

namespace kernel
{
    class UrbanObject_ABC;
}

class PointProjector_ABC;
class UrbanModel;

namespace tools
{
    class Path;
}

namespace geostore
{
    class Database;
    class SpatialIndexer;

// =============================================================================
/** @class  GeoStoreManager
    @brief  GeoStoreManager
*/
// Created: AME 2010-07-19
// =============================================================================
class GeoStoreManager : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             GeoStoreManager( const tools::Path& path, const SpatialIndexer& index );
    virtual ~GeoStoreManager();
    //@}

    const Database& GetDatabase() const;
    void CreateUrbanBlocksOnCities( const geometry::Polygon2f& footprint, double roadWidth, std::vector< geometry::Polygon2f >& urbanBlocks );
    std::vector< const kernel::UrbanObject_ABC* > IntersectedBlocks( const geometry::Polygon2f& footprint );

private:
    //! @name Member data
    //@{
    const SpatialIndexer&               index_;
    std::auto_ptr< PointProjector_ABC > projector_;
    std::auto_ptr< Database >           database_;
    //@}
};

}

#endif // __GeoStoreManager_h_
