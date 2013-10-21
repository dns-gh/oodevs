// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CRYPT_H
#define CRYPT_H

#include "Crypt_ABC.h"

namespace host
{
// =============================================================================
/** @class  Crypt
    @brief  Crypt class
*/
// Created: BAX 2012-07-09
// =============================================================================
struct Crypt : public Crypt_ABC
{
    //! @name Constructors/Destructor
    //@{
             Crypt();
    virtual ~Crypt();
    //@}

    //! @name Methods
    //@{
    virtual std::string Hash( const std::string& password ) const;
    virtual bool Validate( const std::string& password, const std::string& hash ) const;
    //@}
};
}

#endif // CRYPT_H
