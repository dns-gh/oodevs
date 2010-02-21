// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "MissionParameterPoint.h"
#include "Point.h"
#include "SerializationTools.h"
#include <xeumeuleu/xml.h>

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: MissionParameterPoint constructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
MissionParameterPoint::MissionParameterPoint( xml::xistream& xis, const kernel::OrderParameter& type )
    : MissionParameter_ABC( type )
    , point_( new Point( xis, NS( "Point", "jc3iedm" ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPoint destructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
MissionParameterPoint::~MissionParameterPoint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPoint::Serialize
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void MissionParameterPoint::Serialize( MsgMissionParameter& parameter ) const
{
    parameter.set_null_value( 0 );

    parameter.mutable_value()->mutable_point()->mutable_location()->set_type( MsgLocation_Geometry_point );
    parameter.mutable_value()->mutable_point()->mutable_location()->mutable_coordinates()->add_elem();
    point_->Serialize( *parameter.mutable_value()->mutable_point()->mutable_location()->mutable_coordinates()->mutable_elem(0) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPoint::Clean
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void MissionParameterPoint::Clean( MsgMissionParameter& parameter ) const
{
    delete[] parameter.mutable_value()->mutable_point()->mutable_location()->mutable_coordinates()->mutable_elem();
    delete parameter.mutable_value()->mutable_point()->mutable_location();
}
