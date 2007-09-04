// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gearth_plugin_pch.h"
#include "GeometryEditor.h"
#include "geocoord/Lib.h"
#include "geometry/Types.h"

using namespace gearth;

// -----------------------------------------------------------------------------
// Name: GeometryEditor::GeometryEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
GeometryEditor::GeometryEditor()    
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GeometryEditor::~GeometryEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
GeometryEditor::~GeometryEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GeometryEditor::ConvertPosition
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
geometry::Point2d GeometryEditor::ConvertPosition( const std::string& stMGRS ) const
{
    static const double convert_rad_deg = 180. / 3.14159265358979;
    
    geocoord::MGRS      mgrs( stMGRS );
    // geocoord::UTM       utm( geocoord::eNorth, manager_.GetZone(), value.Y() + offset.Y(), value.X() + offset.X() );
    geocoord::Geodetic  geo( mgrs );

    return geometry::Point2d( geo.GetLongitude() * convert_rad_deg, geo.GetLatitude() * convert_rad_deg );   
}