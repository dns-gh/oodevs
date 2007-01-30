// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Exceptions_h_
#define __Exceptions_h_

#include <stdexcept>

namespace frontend
{

class UserRequestFailed : public std::exception
{
public:
             UserRequestFailed( const QString& message ) : std::exception( message.ascii() ) {}
    virtual ~UserRequestFailed() {}
};

}

#endif // __Exceptions_h_
