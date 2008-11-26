// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gearth_plugin_pch.h"
#include "LocationEditor.h"
#include <xeumeuleu/xml.h>
#include <sstream>

using namespace plugins::gearth;

// -----------------------------------------------------------------------------
// Name: LocationEditor::LocationEditor
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
LocationEditor::LocationEditor( xml::xobufferstream& xobs )
    : xobs_ ( xobs )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationEditor::~LocationEditor
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
LocationEditor::~LocationEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationEditor::CreateGeometry
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void LocationEditor::Write( const ASN1T_Location& asn )
{
    std::stringstream ss;

    ss.precision( 14 );
    for( unsigned i = 0; i < asn.coordinates.n; ++i )
        ss << asn.coordinates.elem[i].longitude << "," << asn.coordinates.elem[i].latitude << "," << 0. << std::endl;
    if( asn.coordinates.n > 2 )
        ss << asn.coordinates.elem[0].longitude << "," << asn.coordinates.elem[0].latitude << "," << 0. << std::endl;
    xobs_ << xml::start( "Point" )
            << xml::content( "coordinates", ss.str() )
          << xml::end();
}
