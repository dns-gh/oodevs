// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPointList.h"

// -----------------------------------------------------------------------------
// Name: ParamPointList constructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPointList::ParamPointList( QObject* parent, const QString& name, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter )
    : ParamLocationList( parent, name, layer, converter )
{
    SetShapeFilter( true, false, false, false );
}

// -----------------------------------------------------------------------------
// Name: ParamPointList destructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPointList::~ParamPointList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPointList::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamPointList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_listPoint;
    ParamLocationList::CommitTo( (ASN1T_ListLocalisation*&)asn.value.u.listPoint );
    asn.null_value = asn.value.u.listPoint->n ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ParamPointList::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamPointList::Clean( ASN1T_MissionParameter& asn ) const
{
    ParamLocationList::Clean( (ASN1T_ListLocalisation*&)asn.value.u.listPoint );
}
