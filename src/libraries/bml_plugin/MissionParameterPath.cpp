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
void MissionParameterPath::Serialize( ASN1T_MissionParameter& parameter ) const
{
    parameter.null_value = 0;
    parameter.value.t = T_MissionParameter_value_path;
    parameter.value.u.path = new ASN1T_Path();
    parameter.value.u.path->type = EnumLocationType::line;
    points_->Serialize( parameter.value.u.path->coordinates );
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPath::Clean
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void MissionParameterPath::Clean( ASN1T_MissionParameter& parameter ) const
{
    points_->Clean( parameter.value.u.path->coordinates );
    delete parameter.value.u.path;
}
