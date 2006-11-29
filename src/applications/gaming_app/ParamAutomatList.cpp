// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAutomatList.h"
#include "clients_kernel/Automat_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAutomatList constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomatList::ParamAutomatList( QWidget* pParent, ASN1T_ListAgent& asn, const QString& label, const QString& menu )
    : EntityListParameter< Automat_ABC >( pParent, asn.n, asn.elem, label, menu )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ParamAutomatList destructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomatList::~ParamAutomatList()
{
    // NOTHING
}
