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

#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/filesystem/path.hpp>
#include <string>

class PointProjector_ABC;
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
     Database( const boost::filesystem::path& dbFile,
         const boost::filesystem::path& layersDir, PointProjector_ABC& projector );
    ~Database();
    //@}

    const T_GeoTables& GetTables() const;

private:
    //! @name Helpers
    //@{
    void LoadLayers( PointProjector_ABC& projector,
            const boost::filesystem::path& layersDir );
    GeoTable* LoadLayer( PointProjector_ABC& projector, const boost::filesystem::path& file, const std::string& layer );
    //@}

private:
    //! @name Member data
    //@{
    boost::ptr_map< std::string, GeoTable > tables_;
    boost::shared_ptr< sqlite3 > db_;
    boost::scoped_ptr< LogTable > log_;
    //@}
};

}

#endif // __Database_h_
