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
#include "Path.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: SchemaVersionExtractor csontructor
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
SchemaVersionExtractor::SchemaVersionExtractor()
    : regex_( L"[^0-9.]*([0-9][0-9.]*).*")
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
Path SchemaVersionExtractor::ExtractVersion( const Path& schema ) const
{
    const std::wstring schemaString = schema.ToUnicode();
    boost::match_results< std::wstring::const_iterator > what;
    if( boost::regex_match( schemaString, what, regex_, boost::match_default | boost::match_partial ) && what[1].matched )
        return Path::FromUnicode( std::wstring( what[1].first, what[1].second ) );
    else
        return Path();
}
