// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "FileMatcherFactory.h"
#include "FileMatcherFileName.h"
#include "FileMatcherRootNode.h"
#include "FileMatcherFileName.h"
#include "FileMatcherNonVersionedSchema.h"
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem/path.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: FileMatcherFactory constructor
// Created: NLD 2011-03-08
// -----------------------------------------------------------------------------
FileMatcherFactory::FileMatcherFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileMatcherFactory destructor
// Created: NLD 2011-03-08
// -----------------------------------------------------------------------------
FileMatcherFactory::~FileMatcherFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileMatcherFilFileMatcherFactoryeName::CreateFileMatcher
// Created: NLD 2011-03-08
// -----------------------------------------------------------------------------
boost::shared_ptr< FileMatcher_ABC > FileMatcherFactory::CreateFileMatcher( xml::xistream& xis ) const
{
    if( xis.has_attribute( "root-node" ) )
        return boost::shared_ptr< FileMatcher_ABC >( new FileMatcherRootNode( xis ) );
    else if( xis.has_attribute( "filename" ) )
        return boost::shared_ptr< FileMatcher_ABC >( new FileMatcherFileName( xis ) );
    else if( xis.has_attribute( "non-versioned-schema" ) )
        return boost::shared_ptr< FileMatcher_ABC >( new FileMatcherNonVersionedSchema( xis ) );
    throw std::runtime_error( "Invalid file matcher" );
}
