// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef geostore_DatabaseException_h_
#define geostore_DatabaseException_h_

#include <tools/Exception.h>

namespace geostore
{
class DatabaseException : public tools::Exception
{
public:
    DatabaseException( const std::string& file, const std::string& function, const unsigned int line, const std::string& what )
        : tools::Exception( file, function, line, what )
    {}
};

}

#define MASA_EXCEPTION_SQLITE( message ) geostore::DatabaseException( __FILE__, __FUNCTION__, __LINE__, message )

#endif // geostore_DatabaseException_h_
