// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "SchemaWriter.h"
#include "version.h"
#include <xeumeuleu/xml.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: SchemaWriter constructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
SchemaWriter::SchemaWriter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SchemaWriter destructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
SchemaWriter::~SchemaWriter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SchemaWriter destructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void SchemaWriter::WriteExerciseSchema( xml::xostream& xos, const std::string& schemaName ) const
{
    xos << xml::prefix( "http://www.w3.org/2001/XMLSchema-instance", "xsi" )
            << xml::attribute ("model-version", tools::AppModelVersion() )
            << xml::ns( "http://www.w3.org/2001/XMLSchema-instance" )
            << xml::attribute( "noNamespaceSchemaLocation", "schemas/" + std::string( tools::AppMajorVersion() ) + "/exercise/" + schemaName + ".xsd" );
}
