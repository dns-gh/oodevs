// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPolygon.h"

// -----------------------------------------------------------------------------
// Name: ParamPolygon constructor
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
ParamPolygon::ParamPolygon( const QString& name, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter )
    : ParamLocation( name, layer, converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPolygon destructor
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
ParamPolygon::~ParamPolygon()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPolygon::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamPolygon::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_polygon;
    asn.value.u.polygon = new ASN1T_Localisation();
    ParamLocation::CommitTo( *asn.value.u.polygon );
    asn.null_value = asn.value.u.polygon->vecteur_point.n ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ParamPolygon::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamPolygon::Clean( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.polygon;
}
