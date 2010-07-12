// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "MissionParameterLimit.h"
#include "PointList.h"
#include "SerializationTools.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: MissionParameterLimit constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterLimit::MissionParameterLimit( xml::xistream& xis, const kernel::OrderParameter& type )
    : MissionParameter_ABC( type )
    , points_( 0 )
{
    xis >> xml::start( NS( "Line", "cbml" ) );
    points_.reset( new PointList( xis ) );
    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MissionParameterLimit destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterLimit::~MissionParameterLimit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterLimit::Serialize
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterLimit::Serialize( MsgMissionParameter& parameter ) const
{
    parameter.set_null_value( 0 );
    parameter.mutable_value()->mutable_line()->mutable_location()->set_type( MsgLocation::line );
    points_->Serialize( *parameter.mutable_value()->mutable_line()->mutable_location()->mutable_coordinates() );
}
