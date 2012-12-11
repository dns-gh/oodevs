// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "FileMigration.h"
#include "GeneralConfig.h"
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: FileMigration constructor
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
FileMigration::FileMigration( xml::xistream& xis )
    : fromVersion_( xis.attribute< std::string >( "from" ) )
    , toVersion_  ( xis.attribute< std::string >( "to" ) )
{
    xis >> xml::list( "upgrade", *this, &FileMigration::ReadXslTransform );
}

// -----------------------------------------------------------------------------
// Name: FileMigration destructor
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
FileMigration::~FileMigration()
{
    // NOTHING
}

// =============================================================================
// Tools
// =============================================================================

// -----------------------------------------------------------------------------
// Name: FileMigration::ReadXslTransform
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
void FileMigration::ReadXslTransform( xml::xistream& xis )
{
    const std::string schema = xis.attribute< std::string >( "schema" );
    const std::string xslFile = GeneralConfig::BuildResourceChildFile( xis.attribute< std::string >( "apply" ) );
    xsl::xstringtransform xslTmp( xslFile );
    if( !transformsFromSchema_.insert( std::make_pair( schema, xslFile ) ).second )
        throw MASA_EXCEPTION( boost::str( boost::format( "Invalid migration from %s to %s for schema %s: more than one transformtion for this schema" ) % fromVersion_ % toVersion_ % schema ) );
}

// =============================================================================
// Operations
// =============================================================================

// -----------------------------------------------------------------------------
// Name: FileMigration::UpgradeFile
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
std::auto_ptr< xml::xistream > FileMigration::UpgradeFile( std::auto_ptr< xml::xistream > xis, const std::string& schema ) const
{
    T_XslTransforms::const_iterator it = transformsFromSchema_.find( schema );
    if( it == transformsFromSchema_.end() )
        return xis;

    xsl::xstringtransform xst( it->second );
    xst << *xis;
    return std::auto_ptr< xml::xistream >( new xml::xistringstream( xst.str() ) );
}
