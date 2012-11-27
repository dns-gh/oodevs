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
class TerrainFileReader;

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
    void CreateTable( const TerrainFileReader& file );
    void GetTable( const std::string& name );     
    bool HasTable( const std::string& name );
    //@}

public:
   //! @name Member data
    //@{
    typedef std::map< std::string, GeoTable* > T_Tables;
    typedef T_Tables::iterator                IT_Tables;
    typedef T_Tables::const_iterator         CIT_Tables;
    T_Tables tables_;
    std::auto_ptr< LogTable > logTable_;
    //@}

private:
    //! @name Member data
    //@{
    boost::filesystem::path path_;
    char* err_msg;
    sqlite3* db_;
    //@}
};

} //! namespace geostore

#endif // __Database_h_
