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

#include <boost/noncopyable.hpp>
#include <vector>
#include <string>

struct sqlite3;
struct sqlite3_stmt;

namespace geostore
{

class Table : private boost::noncopyable
{
public:
    typedef std::vector< std::vector< std::string > > T_ResultSet;

protected:
    Table( sqlite3* db, const std::string& name );

    const std::string& GetName() const;

    T_ResultSet ExecuteQuery( const std::string& query );

    sqlite3_stmt* CreateStatement( const std::string& query ) const;
    int StepStatement( sqlite3_stmt* stmt ) const;

private:
    sqlite3* db_;
    std::string name_;
};

}

#endif /* __geostore_Table_h_ */
