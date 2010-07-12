// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "MissionParameterPolygon.h"
#include "PointList.h"
#include "SerializationTools.h"
#include <xeumeuleu/xml.h>

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: MissionParameterPolygon constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterPolygon::MissionParameterPolygon( xml::xistream& xis, const kernel::OrderParameter& type )
    : MissionParameter_ABC( type )
    , points_( 0 )
{
    xis >> xml::start( NS( "PolygonArea", "cbml" ) )
            >> xml::start( NS( "BoundingLine", "jc3iedm" ) );
    points_.reset( new PointList( xis ) );
    xis     >> xml::end
        >> xml::end;
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
void MissionParameterPolygon::Serialize( MsgMissionParameter& parameter ) const
{
    parameter.set_null_value( 0 );
    parameter.mutable_value()->mutable_polygon()->mutable_location()->set_type ( MsgLocation_Geometry_polygon );
    points_->Serialize( *parameter.mutable_value()->mutable_polygon()->mutable_location()->mutable_coordinates() );
}
