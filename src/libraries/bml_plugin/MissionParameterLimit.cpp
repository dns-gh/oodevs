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
#include <xeumeuleu/xml.h>

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
    xis >> xml::end();
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
void MissionParameterLimit::Serialize( ASN1T_MissionParameter& parameter ) const
{
    parameter.null_value = 0;
    parameter.value.t = T_MissionParameter_value_line;
    parameter.value.u.line = new ASN1T_Line();
    parameter.value.u.line->type = EnumLocationType::line;
    points_->Serialize( parameter.value.u.line->coordinates );
}

// -----------------------------------------------------------------------------
// Name: MissionParameterLimit::Clean
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterLimit::Clean( ASN1T_MissionParameter& parameter ) const
{
    points_->Clean( parameter.value.u.line->coordinates );
    delete parameter.value.u.line;
}
