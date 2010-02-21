// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "MissionParameterPath.h"
#include "PointList.h"
#include "SerializationTools.h"
#include <xeumeuleu/xml.h>

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: MissionParameterPath constructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
MissionParameterPath::MissionParameterPath( xml::xistream& xis, const kernel::OrderParameter& type )
    : MissionParameter_ABC( type )
    , points_( 0 )
{
    xis >> xml::start( NS( "Line", "cbml" ) );
    points_.reset( new PointList( xis ) );
    xis >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPath destructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
MissionParameterPath::~MissionParameterPath()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPath::Serialize
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void MissionParameterPath::Serialize( MsgMissionParameter& parameter ) const
{
    parameter.set_null_value( 0 );

//    parameter.mutable_value()->mutable_path() = new MsgPath();
    parameter.mutable_value()->mutable_path()->mutable_location()->set_type( MsgLocation_Geometry::MsgLocation_Geometry_line );
    points_->Serialize( *parameter.mutable_value()->mutable_path()->mutable_location()->mutable_coordinates() );
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPath::Clean
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void MissionParameterPath::Clean( MsgMissionParameter& parameter ) const
{
    points_->Clean( *parameter.mutable_value()->mutable_path()->mutable_location()->mutable_coordinates());
    delete parameter.mutable_value()->mutable_path()->mutable_location();
}
