// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gearth_plugin_pch.h"
#include "LineEditor.h"
#include "xeumeuleu/xml.h"
#include <sstream>

using namespace gearth;

// -----------------------------------------------------------------------------
// Name: LineEditor::LineEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
LineEditor::LineEditor( xml::xobufferstream& xobs )
    : xobs_ ( xobs )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LineEditor::~LineEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
LineEditor::~LineEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LineEditor::Write
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
void LineEditor::Write( const ASN1T_Line& asnLine )
{
    Write( asnLine.coordinates );
}

// -----------------------------------------------------------------------------
// Name: LineEditor::CreateGeometry
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void LineEditor::Write( const ASN1T_CoordUTMList& asnCoords )
{
    std::stringstream ss;    
    
    ss.precision( 14 );
    for( unsigned i = 0; i < asnCoords.n; ++i )
    {
        geometry::Point2d position( editor_.ConvertPosition( std::string( (char*)asnCoords.elem[i].data, 15 ) ) );
        ss << position.X() << "," << position.Y() << "," << 0. << std::endl;
    }
    xobs_ << xml::start( "Point" )
            << xml::content( "coordinates", ss.str() )
          << xml::end();
}
