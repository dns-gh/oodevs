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
void MissionParameterPoint::Serialize( ASN1T_MissionParameter& parameter ) const
{
    parameter.null_value = 0;
    parameter.value.t = T_MissionParameter_value_point;
    parameter.value.u.point = new ASN1T_Point();
    parameter.value.u.point->type = EnumLocationType::point;
    parameter.value.u.point->coordinates.n = 1;
    parameter.value.u.point->coordinates.elem = new ASN1T_CoordLatLong[1];
    point_->Serialize( parameter.value.u.point->coordinates.elem[0] );
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPoint::Clean
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void MissionParameterPoint::Clean( ASN1T_MissionParameter& parameter ) const
{
    delete[] parameter.value.u.point->coordinates.elem;
    delete parameter.value.u.point;
}
