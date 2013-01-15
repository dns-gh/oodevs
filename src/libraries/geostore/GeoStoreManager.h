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

#include <boost/filesystem.hpp>
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
             GeoStoreManager( const boost::filesystem::path& path, const SpatialIndexer& index );
    virtual ~GeoStoreManager();
    //@}
    const Database& GetDatabase() const;
    void CreateUrbanBlocksOnCities( const geometry::Polygon2f& footprint, double roadWidth, std::vector< geometry::Polygon2f >& urbanBlocks );
    std::vector< const kernel::UrbanObject_ABC* > IntersectedBlocks( const geometry::Polygon2f& footprint );

private:
    //! @name Helpers
    //@{
    void Initialize( const boost::filesystem::path& path );
    void InitProjector( const boost::filesystem::path& terrainFile ) ;
    void InitProjectorOld( const boost::filesystem::path& worldfile ) ;
    //@}

private:
    //! @name Member data
    //@{
    boost::filesystem::path             path_;
    const SpatialIndexer&               index_;
    std::auto_ptr< Database >           spatialDb_;
    std::auto_ptr< PointProjector_ABC > proj_;
    std::auto_ptr< PointProjector_ABC > trans_;
    //@}
};

} //! namespace geostore

#endif // __GeoStoreManager_h_
