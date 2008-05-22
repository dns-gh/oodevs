// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "MissionParameterHeading.h"
#include "Point.h"

using namespace bml;

// -----------------------------------------------------------------------------
// Name: MissionParameterHeading constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterHeading::MissionParameterHeading( xml::xistream& xis, const kernel::OrderParameter& type )
    : MissionParameter_ABC( type )
    , angle_( 0 )
{
    Point point( xis );
    // $$$$ SBO 2008-05-22: set angle from unit position
}

// -----------------------------------------------------------------------------
// Name: MissionParameterHeading destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterHeading::~MissionParameterHeading()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterHeading::Serialize
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterHeading::Serialize( ASN1T_MissionParameter& parameter ) const
{
    parameter.null_value = 0;
    parameter.value.t = T_MissionParameter_value_heading;
    parameter.value.u.heading = angle_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameterHeading::Clean
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterHeading::Clean( ASN1T_MissionParameter& ) const
{
    // NOTHING
}
