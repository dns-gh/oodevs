// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"

#include "FileMatcherFileName.h"
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem/path.hpp>

using namespace tools;
namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: FileMatcherFileName constructor
// Created: NLD 2011-03-08
// -----------------------------------------------------------------------------
FileMatcherFileName::FileMatcherFileName( xml::xistream& xis )
    : fileNameToMatch_( xis.attribute< std::string >( "filename" ) )
    , assignedSchema_ ( xis.attribute< std::string >( "assign-schema" ) )
{
}

// -----------------------------------------------------------------------------
// Name: FileMatcherFileName destructor
// Created: NLD 2011-03-08
// -----------------------------------------------------------------------------
FileMatcherFileName::~FileMatcherFileName()
{
}

// -----------------------------------------------------------------------------
// Name: FileMatcherFileName destructor
// Created: NLD 2011-03-08
// -----------------------------------------------------------------------------
bool FileMatcherFileName::MatchAndReturnNewSchema( const std::string& inputFileName, xml::xistream& /*xis*/, std::string& outputSchema ) const
{
    outputSchema.clear();
    const std::string genericInputFileName = bfs::path( inputFileName ).string();

    if(   genericInputFileName.size() >= fileNameToMatch_.size()
        && genericInputFileName.compare( genericInputFileName.size() - fileNameToMatch_.size(), fileNameToMatch_.size(), fileNameToMatch_ ) == 0 )
    {
        outputSchema = assignedSchema_;
        return true;
    }
    return false;
//
        //if( path.filename() != fileNameToMatch_ )
    //    return false;
}
