// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Database_h_
#define __Database_h_

#include "GeometryType.h"
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/filesystem/path.hpp>
#include <string>
#include <vector>

class PointProjector_ABC;
class TerrainObject;
struct sqlite3;

namespace geostore
{
    class GeoTable;
    class LogTable;

    typedef boost::ptr_map< std::string, GeoTable > T_GeoTables;

// =============================================================================
/** @class  Database
    @brief  Database
*/
// Created: AME 2010-07-19
// =============================================================================
class Database : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
     Database( const boost::filesystem::path& dbFile, const boost::filesystem::path& layersDir, PointProjector_ABC& projector );
    ~Database();
    //@}

    void AddLayer( std::string layer, GeometryType geomType, const std::vector< TerrainObject* >& features );

    const T_GeoTables& GetTables() const;

private:
    //! @name Helpers
    //@{
    void LoadLayers( PointProjector_ABC& projector, const boost::filesystem::path& layersDir );
    void LoadLayer( std::string layer, PointProjector_ABC& projector, const boost::filesystem::path& file );
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< sqlite3 > db_;
    boost::scoped_ptr< LogTable > log_;
    boost::ptr_map< std::string, GeoTable > tables_;
    //@}
};

}

#endif // __Database_h_
