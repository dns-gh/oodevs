// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "MissionParameterPolygon.h"
#include "PointList.h"
#include "SerializationTools.h"
#include <xeumeuleu/xml.h>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: MissionParameterPolygon constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterPolygon::MissionParameterPolygon( xml::xistream& xis, const kernel::OrderParameter& type )
    : MissionParameter_ABC( type )
    , points_( 0 )
{
    xis >> xml::start( NS( "PolygonArea", "cbml" ) )
            >> xml::start( NS( "BoundingLine", "cbml" ) );
    points_.reset( new PointList( xis ) );
    xis     >> xml::end()
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPolygon destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterPolygon::~MissionParameterPolygon()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPolygon::Serialize
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterPolygon::Serialize( ASN1T_MissionParameter& parameter ) const
{
    parameter.null_value = 0;
    parameter.value.t = T_MissionParameter_value_polygon;
    parameter.value.u.polygon = new ASN1T_Polygon();
    parameter.value.u.polygon->type = EnumLocationType::polygon;
    points_->Serialize( parameter.value.u.polygon->coordinates );
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPolygon::Clean
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterPolygon::Clean( ASN1T_MissionParameter& parameter ) const
{
    points_->Clean( parameter.value.u.polygon->coordinates );
    delete parameter.value.u.polygon;
}
