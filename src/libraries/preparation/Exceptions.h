// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Exceptions_h_
#define __Exceptions_h_

#include <stdexcept>

class InvalidModelException : public std::exception
{
public:
    InvalidModelException( const QString& section, const QString& message )
        : std::exception( ( section + QString( ": \n" ) + message ).ascii() ) {}
};

#endif // __Exceptions_h_
