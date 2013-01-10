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

class PointProjector_ABC;
struct sqlite3;

namespace geostore
{
    class GeoTable;
    class LogTable;

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
    explicit Database( const boost::filesystem::path& path );
    virtual ~Database();
    //@}

    //! @name Operations
    //@{
    void LoadLayers( PointProjector_ABC& projector );
    //@}

private:
    void LoadLayer( const std::string& name, const boost::filesystem::path& path, PointProjector_ABC& projector );
    void LoadLayer( const std::string& name );

public:
   //! @name Member data
    //@{
    std::map< std::string, GeoTable* > tables_;
    std::auto_ptr< LogTable > logTable_;
    //@}

private:
    //! @name Member data
    //@{
    boost::filesystem::path path_;
    sqlite3* db_;
    //@}
};

} //! namespace geostore

#endif // __Database_h_
