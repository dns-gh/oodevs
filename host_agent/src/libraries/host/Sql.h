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
             Sql( const Path& file );
    virtual ~Sql();
    //@}

    //! @name Sql_ABC Methods
    //@{
    virtual T_Transaction Begin  ( bool write );
    virtual T_Statement   Prepare( const Transaction& tr, const std::string& sql );
    virtual void          Commit ( Transaction& tr );
    //@}

private:
    const Path file_;
    boost::mutex access_;
    boost::shared_ptr< sqlite3 > db_;
};

// =============================================================================
/** @class  Transaction
    @brief  Transaction class
*/
// Created: BAX 2012-06-28
// =============================================================================
struct Transaction
{
    //! @name Constructors/Destructor
    //@{
             Transaction( boost::mutex& access, Sql& db, bool write );
    virtual ~Transaction();
    //@}

    void Commit();

private:
    boost::lock_guard< boost::mutex > lock_;
    Sql& db_;
    bool write_;
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
             Statement( sqlite3_stmt* stmt );
    virtual ~Statement();
    //@}

    //! @name Statement_ABC Methods
    //@{
    virtual void   Bind( double value );
    virtual void   Bind( int value );
    virtual void   Bind( int64_t value );
    virtual void   Bind( const std::string& value );
    virtual bool   Next();
    virtual void   Read( double& value );
    virtual void   Read( int& value );
    virtual void   Read( int64_t& value );
    virtual void   Read( std::string& value );
    virtual void   Reset();
    //@}

private:
    boost::shared_ptr< sqlite3_stmt > stmt_;
    int bind_;
    int read_;
};
}

#endif // SQL_H
