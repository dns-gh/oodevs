// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "SchemaVersionExtractor.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: SchemaVersionExtractor csontructor
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
SchemaVersionExtractor::SchemaVersionExtractor()
    : regex_( "[^0-9.]*([0-9][0-9.]*).*")
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SchemaVersionExtractor destructor
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
SchemaVersionExtractor::~SchemaVersionExtractor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SchemaVersionExtractor::ExtractVersion
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
std::string SchemaVersionExtractor::ExtractVersion( const std::string& schema ) const
{
    boost::match_results< std::string::const_iterator > what;
    if( boost::regex_match( schema, what, regex_, boost::match_default | boost::match_partial ) && what[1].matched )
        return std::string( what[1].first, what[1].second );
    else
        return std::string();
}
