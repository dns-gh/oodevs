// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "geostore_pch.h"
#include "DatabaseException.h"

using namespace geostore;

DatabaseException::DatabaseException( int errCode, const std::string& errMsg )
    : std::runtime_error( errMsg )
    , err_              ( errCode )
{
}

int DatabaseException::errorCode() const
{
    return err_;
}
