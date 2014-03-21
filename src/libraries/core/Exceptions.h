// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef CORE_EXCEPTIONS_H
#define CORE_EXCEPTIONS_H

#include <stdexcept>

namespace core
{
    struct RuntimeError : std::runtime_error
    {
        explicit RuntimeError( const std::string& what )
            : std::runtime_error( what )
        {}
    };
}

#endif // CORE_EXCEPTIONS_H
