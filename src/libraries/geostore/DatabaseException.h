// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __geostore_DatabaseException_h_
#define __geostore_DatabaseException_h_

#include "geostore_pch.h"

namespace geostore
{
    class DatabaseException : public std::runtime_error
    {
    public:
        DatabaseException( int errCode, const std::string& errMsg );

        int errorCode() const;

    private:
        int err_;
    };
}

#endif // __geostore_DatabaseException_h_
