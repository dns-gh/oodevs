// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gearth_plugin_pch.h"
#include "PositionEditor.h"
#include "xeumeuleu/xml.h"
#include <sstream>

using namespace gearth;

// -----------------------------------------------------------------------------
// Name: PositionEditor::PositionEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
PositionEditor::PositionEditor( xml::xobufferstream& xobs )
    : xobs_ ( xobs )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PositionEditor::~PositionEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
PositionEditor::~PositionEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PositionEditor::Write
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
void PositionEditor::Write( const ASN1T_CoordUTM& asnUTM )
{
    std::stringstream ss;        
    geometry::Point2d position( editor_.ConvertPosition( std::string( (char*)asnUTM.data, 15 ) ) );

    ss.precision( 14 );
    ss << position.X() << "," << position.Y() << "," << 0.;    
    xobs_ << xml::start( "Point" )
            << xml::content( "coordinates", ss.str() )
          << xml::end();
}