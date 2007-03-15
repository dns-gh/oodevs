// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPolygonList.h"

// -----------------------------------------------------------------------------
// Name: ParamPolygonList constructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPolygonList::ParamPolygonList( QObject* parent, const QString& name, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter )
    : ParamLocationList( parent, name, layer, converter )
{
    SetShapeFilter( false, false, true, false );
}

// -----------------------------------------------------------------------------
// Name: ParamPolygonList destructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPolygonList::~ParamPolygonList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPolygonList::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamPolygonList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_listPolygon;
    ParamLocationList::CommitTo( (ASN1T_ListLocalisation*&)asn.value.u.listPolygon );
    asn.null_value = asn.value.u.listPolygon->n ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ParamPolygonList::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamPolygonList::Clean( ASN1T_MissionParameter& asn ) const
{
    ParamLocationList::Clean( (ASN1T_ListLocalisation*&)asn.value.u.listPolygon );
}
