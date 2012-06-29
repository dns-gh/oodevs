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
    struct Transaction;
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
    typedef boost::shared_ptr< Transaction >   T_Transaction;
    typedef boost::shared_ptr< Statement_ABC > T_Statement;
    //@}

    //! @name Methods
    //@{
    virtual T_Transaction Begin  ( bool write = true ) = 0;
    virtual T_Statement   Prepare( const Transaction& tr, const std::string& sql ) = 0;
    virtual void          Commit ( Transaction& tr ) = 0;
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
    virtual void        Bind( bool value ) = 0;
    virtual void        Bind( int value ) = 0;
    virtual void        Bind( int64_t value ) = 0;
    virtual void        Bind( double value ) = 0;
    virtual void        Bind( const std::string& value ) = 0;
    virtual bool        Next() = 0;
    virtual bool        ReadBool() = 0;
    virtual int         ReadInt() = 0;
    virtual int64_t     ReadInt64() = 0;
    virtual double      ReadDouble() = 0;
    virtual std::string ReadText() = 0;
    virtual void        Reset() = 0;
    //@}
};
}

#endif // SQL_ABC_H
