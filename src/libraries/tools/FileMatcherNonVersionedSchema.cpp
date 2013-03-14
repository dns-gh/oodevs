// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"

#include "FileMatcherNonVersionedSchema.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: FileMatcherNonVersionedSchema constructor
// Created: NLD 2011-03-08
// -----------------------------------------------------------------------------
FileMatcherNonVersionedSchema::FileMatcherNonVersionedSchema( xml::xistream& xis )
    : nonVersionedSchemaToMatch_( Path::FromUTF8( xis.attribute< std::string >( "non-versioned-schema" ) ) )
    , assignedSchema_( Path::FromUTF8( xis.attribute< std::string >( "assign-schema" ) ) )
{
}

// -----------------------------------------------------------------------------
// Name: FileMatcherNonVersionedSchema destructor
// Created: NLD 2011-03-08
// -----------------------------------------------------------------------------
FileMatcherNonVersionedSchema::~FileMatcherNonVersionedSchema()
{
}

namespace
{
    void ExtractSchemaName( xml::xistream& xis, Path& schema )
    {
        schema = xis.attribute< tools::Path >( "xsi:noNamespaceSchemaLocation", "" );
    }
}

// -----------------------------------------------------------------------------
// Name: FileMatcherNonVersionedSchema destructor
// Created: NLD 2011-03-08
// -----------------------------------------------------------------------------
bool FileMatcherNonVersionedSchema::MatchAndReturnNewSchema( const Path& /*inputFileName*/, xml::xistream& xis, Path& outputSchema ) const
{
    outputSchema.Clear();

    Path nonVersionedSchema;
    xis >> xml::list( boost::bind( &ExtractSchemaName, _3, boost::ref( nonVersionedSchema ) ) );

    if( nonVersionedSchemaToMatch_ != nonVersionedSchema )
        return false;
    outputSchema = assignedSchema_;
    return true;
}
