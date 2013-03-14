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

using namespace tools;

// -----------------------------------------------------------------------------
// Name: FileMatcherFileName constructor
// Created: NLD 2011-03-08
// -----------------------------------------------------------------------------
FileMatcherFileName::FileMatcherFileName( xml::xistream& xis )
    : fileNameToMatch_( Path::FromUTF8( xis.attribute< std::string >( "filename" ) ) )
    , assignedSchema_ ( Path::FromUTF8( xis.attribute< std::string >( "assign-schema" ) ) )
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
bool FileMatcherFileName::MatchAndReturnNewSchema( const Path& inputFileName, xml::xistream& /*xis*/, Path& outputSchema ) const
{
    outputSchema.Clear();

    const std::wstring inputString = inputFileName.ToUnicode();
    const std::wstring stringToMatch = fileNameToMatch_.ToUnicode();

    if( inputString.size() >= stringToMatch.size() &&
        inputString.compare( inputString.size() - stringToMatch.size(), stringToMatch.size(), stringToMatch ) == 0 )
    {
        outputSchema = assignedSchema_;
        return true;
    }
    return false;
}
