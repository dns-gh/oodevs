// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CRYPT_ABC_H
#define CRYPT_ABC_H

#include <boost/noncopyable.hpp>
#include <string>

namespace host
{
// =============================================================================
/** @class  Crypt_ABC
    @brief  Crypt_ABC interface
*/
// Created: BAX 2012-07-09
// =============================================================================
struct Crypt_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Crypt_ABC() {}
    virtual ~Crypt_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual std::string Hash( const std::string& password ) const = 0;
    virtual bool Validate( const std::string& password, const std::string& hash ) const = 0;
    //@}
};
}

#endif // CRYPT_ABC_H
