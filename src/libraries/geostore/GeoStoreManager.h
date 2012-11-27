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

#include "geostore_pch.h"
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
             GeoStoreManager( const boost::filesystem::path& path, UrbanModel& model );
    explicit GeoStoreManager( UrbanModel& model );
    virtual ~GeoStoreManager();
    //@}

    //! @name Operations
    //@{
    void Initialize( const boost::filesystem::path& path );
    void LoadTerrainFiles();
    void CreateUrbanBlocksOnCities( const geometry::Polygon2f& footprint, kernel::UrbanObject_ABC& parent );
    bool BlockAutoProcess( const geometry::Polygon2f& footprint );
    bool IsInitialized() const;
    //@}

private:
    //! @name Helpers
    //@{
    void InitProjector( const boost::filesystem::path& terrainFile ) ;
    void InitProjectorOld( const boost::filesystem::path& worldfile ) ;
    //@}

private:
    //! @name Member data
    //@{
    boost::filesystem::path             path_;
    std::auto_ptr< Database >           spatialDb_;
    std::auto_ptr< PointProjector_ABC > proj_;
    std::auto_ptr< PointProjector_ABC > trans_;
    UrbanModel&                         urbanModel_;
    //@}
};

} //! namespace geostore

#endif // __GeoStoreManager_h_
