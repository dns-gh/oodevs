// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __geostore_Table_h_
#define __geostore_Table_h_

#include "geostore_pch.h"

namespace geostore
{

class Table : private boost::noncopyable
{
public:
    typedef std::vector< std::vector< std::string > > T_ResultSet;

    Table( sqlite3* db, const std::string& name );

    const std::string& GetName() const;

    void ExecuteQuery( const std::string& query );
    void ExecuteQuery( const std::string& query, T_ResultSet& result );

protected:
    sqlite3_stmt* CreateStatement( const std::string& query ) const;
    int StepStatement( sqlite3_stmt* stmt ) const;
    sqlite3* GetDB() const;

private:
    sqlite3* db_;
    std::string name_;
};

}

#endif /* __geostore_Table_h_ */
