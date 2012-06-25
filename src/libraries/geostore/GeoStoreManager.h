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

namespace geostore
{
    class Database;
    class SpatialRequestStatus;

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
             GeoStoreManager( const std::string& path, UrbanModel& model );
    explicit GeoStoreManager( UrbanModel& model );
    virtual ~GeoStoreManager();
    //@}

    //! @name Operations
    //@{
    void LoadTerrainFiles();
    SpatialRequestStatus* CreateUrbanBlocksOnCities( const geometry::Polygon2f& footprint, kernel::UrbanObject_ABC& parent );
    bool BlockAutoProcess( const geometry::Polygon2f& footprint );
    void Initialize( const std::string& path );
    bool IsInitialized() const;
    //@}

private:
    //! @name Helpers
    //@{
    void InitProjector( const std::string& terrainFile ) ;
    void InitProjectorOld( const std::string& worldfile ) ;
    //@}

private:
    //! @name Member data
    //@{
    std::string         path_;
    Database*           spatialDb_;
    PointProjector_ABC* trans_;
    UrbanModel&         urbanModel_;
    //@}
};

} //! namespace geostore

#endif // __GeoStoreManager_h_
