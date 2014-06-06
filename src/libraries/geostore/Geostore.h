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

#include <tools/Path.h>
#include <geometry/Types.h>

namespace kernel
{
    class UrbanObject_ABC;
}

class PointProjector_ABC;
class UrbanModel;

namespace geostore
{
    class Database;
    class SpatialIndexer;
    class Loader;

// =============================================================================
/** @class  Geostore
    @brief  Geostore
*/
// Created: AME 2010-07-19
// =============================================================================
class Geostore : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Geostore( const tools::Path& terrain, const SpatialIndexer& index );
    virtual ~Geostore();
    //@}

    const Database& GetDatabase() const;
    void CreateUrbanBlocksOnCities( const geometry::Polygon2f& footprint, double roadWidth, std::vector< geometry::Polygon2f >& urbanBlocks );
    std::vector< const kernel::UrbanObject_ABC* > IntersectedBlocks( const geometry::Polygon2f& footprint );

private:
    //! @name Member data
    //@{
    const SpatialIndexer& index_;
    std::unique_ptr< Loader > loader_;
    std::unique_ptr< PointProjector_ABC > projector_;
    std::unique_ptr< Database > database_;
    //@}
};

}

#endif // __GeoStoreManager_h_
