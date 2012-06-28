// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SQL_ABC_H
#define SQL_ABC_H

#include <boost/noncopyable.hpp>
#include <stdint.h>
#include <string>

namespace boost
{
    template< typename T > class shared_ptr;
}

namespace host
{
    struct Statement_ABC;
}

namespace host
{
// =============================================================================
/** @class  Sql_ABC
    @brief  Sql_ABC interface
*/
// Created: BAX 2012-06-28
// =============================================================================
struct Sql_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Sql_ABC() {}
    virtual ~Sql_ABC() {}
    //@}

    //! @name Typedef helpers
    //@{
    typedef boost::shared_ptr< Statement_ABC > StatementPtr;
    //@}

    //! @name Methods
    //@{
    virtual StatementPtr Prepare( const std::string& sql ) = 0;
    virtual StatementPtr Execute( const std::string& sql ) = 0;
    //@}
};

// =============================================================================
/** @class  Statement_ABC
    @brief  Statement_ABC interface
*/
// Created: BAX 2012-06-28
// =============================================================================
struct Statement_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Statement_ABC() {}
    virtual ~Statement_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual bool   Bind( const void* data, size_t size ) = 0;
    virtual bool   Bind( double value ) = 0;
    virtual bool   Bind( int value ) = 0;
    virtual bool   Bind( int64_t value ) = 0;
    virtual bool   Bind( const std::string& value ) = 0;
    virtual bool   Bind() = 0;
    virtual bool   Execute() = 0;
    virtual size_t RowCount() = 0;
    virtual bool   Read( size_t col, void* data, size_t size ) = 0;
    virtual bool   Read( size_t col, double& value ) = 0;
    virtual bool   Read( size_t col, int& value ) = 0;
    virtual bool   Read( size_t col, int64_t& value ) = 0;
    virtual bool   Read( size_t col, std::string& value ) = 0;
    virtual bool   Next() = 0;
    virtual bool   Reset() = 0;
    //@}
};
}

#endif // SQL_ABC_H
