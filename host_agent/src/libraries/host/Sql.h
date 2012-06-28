// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SQL_H
#define SQL_H

#include "Sql_ABC.h"

#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>

struct sqlite3;
struct sqlite3_stmt;

namespace cpplog
{
    class BaseLogger;
}

namespace host
{
    typedef boost::filesystem3::path Path;
}

namespace host
{
// =============================================================================
/** @class  Sql
    @brief  Sql class
*/
// Created: BAX 2012-06-28
// =============================================================================
struct Sql : public Sql_ABC
{
    //! @name Constructors/Destructor
    //@{
             Sql( cpplog::BaseLogger& log,
                  const Path& file );
    virtual ~Sql();
    //@}

    //! @name Sql_ABC Methods
    //@{
    virtual void Prepare( const std::string& sql, const Operand& op );
    //@}

private:
    cpplog::BaseLogger& log_;
    const Path file_;
    boost::mutex access_;
    boost::shared_ptr< sqlite3 > db_;
};

// =============================================================================
/** @class  Statement
    @brief  Statement class
*/
// Created: BAX 2012-06-28
// =============================================================================
struct Statement : public Statement_ABC
{
    //! @name Constructors/Destructor
    //@{
             Statement( cpplog::BaseLogger& log, sqlite3_stmt* stmt );
    virtual ~Statement();
    //@}

    //! @name Methods
    //@{
    virtual bool   Bind( int col, double value );
    virtual bool   Bind( int col, int value );
    virtual bool   Bind( int col, int64_t value );
    virtual bool   Bind( int col, const std::string& value );
    virtual bool   Next();
    virtual bool   Read( int col, double& value );
    virtual bool   Read( int col, int& value );
    virtual bool   Read( int col, int64_t& value );
    virtual bool   Read( int col, std::string& value );
    virtual bool   Reset();
    //@}

private:
    cpplog::BaseLogger& log_;
    boost::shared_ptr< sqlite3_stmt > stmt_;
};
}

#endif // SQL_H
