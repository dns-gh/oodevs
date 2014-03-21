// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "ModelToXml.h"
#include "core/Model.h"
#include "core/ModelVisitor_ABC.h"
#include <tools/Exception.h>
#include <tools/Path.h>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ModelToXml
// Created: ABR 2013-08-14
// -----------------------------------------------------------------------------
void xsd::ModelToXml( const core::Model& model, const tools::Path& output )
{
    xml::xofstream xos( output.ToUTF8() );
    ModelToXml( model, xos );
}

// -----------------------------------------------------------------------------
// Name: ModelToXml
// Created: ABR 2013-08-14
// -----------------------------------------------------------------------------
void xsd::ModelToXml( const core::Model& /*model*/, xml::xostream& /*xos*/ )
{
    // NOTHING
}
