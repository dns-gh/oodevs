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
#include "VersionHelper.h"
#include "XmlStreamOperators.h"
#include <tools/Path.h>
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
// Name: SchemaWriter::WriteSchema
// Created: ABR 2012-04-18
// -----------------------------------------------------------------------------
void SchemaWriter::WriteSchema( xml::xostream& xos, const Path& category, const Path& schemaName ) const
{
    xos << xml::prefix( "http://www.w3.org/2001/XMLSchema-instance", "xsi" )
        << xml::attribute( "model-version", tools::AppModelVersion() )
        << xml::ns( "http://www.w3.org/2001/XMLSchema-instance" )
        << xml::attribute( "noNamespaceSchemaLocation", Path( "schemas" ) / tools::AppMajorVersion() / category / schemaName + ".xsd" );
}

// -----------------------------------------------------------------------------
// Name: SchemaWriter::WritePhysicalSchema
// Created: ABR 2012-04-18
// -----------------------------------------------------------------------------
void SchemaWriter::WritePhysicalSchema( xml::xostream& xos, const Path& schemaName ) const
{
    WriteSchema( xos, "physical", schemaName );
}

// -----------------------------------------------------------------------------
// Name: SchemaWriter destructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
void SchemaWriter::WriteExerciseSchema( xml::xostream& xos, const Path& schemaName ) const
{
    WriteSchema( xos, "exercise", schemaName );
}
