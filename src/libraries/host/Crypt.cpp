// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Crypt.h"

extern "C" {
#include <bcrypt/bcrypt.h>
}

using namespace host;

// -----------------------------------------------------------------------------
// Name: Crypt::Crypt
// Created: BAX 2012-07-09
// -----------------------------------------------------------------------------
Crypt::Crypt()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Crypt::Crypt
// Created: BAX 2012-07-09
// -----------------------------------------------------------------------------
Crypt::~Crypt()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Crypt::Hash
// Created: BAX 2012-07-09
// -----------------------------------------------------------------------------
std::string Crypt::Hash( const std::string& password ) const
{
    char salt[BCRYPT_HASHSIZE], hash[BCRYPT_HASHSIZE+1];
    bcrypt_gensalt( 4, salt );
    bcrypt_hashpw( password.c_str(), salt, hash );
    hash[BCRYPT_HASHSIZE] = 0;
    return std::string( hash );
}

// -----------------------------------------------------------------------------
// Name: Crypt::Validate
// Created: BAX 2012-07-09
// -----------------------------------------------------------------------------
bool Crypt::Validate( const std::string& password, const std::string& hash ) const
{
    char output[BCRYPT_HASHSIZE+1];
    const int err = bcrypt_hashpw( password.c_str(), hash.c_str(), output );
    if( err )
        return false;
    output[BCRYPT_HASHSIZE] = 0;
    return hash == output;
}
