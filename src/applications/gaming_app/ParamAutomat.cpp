// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAutomat.h"
#include "clients_kernel/Automat_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAutomat constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomat::ParamAutomat(  QWidget* pParent, ASN1T_OID& id, const QString& label, const QString& menu )
    : EntityParameter< Automat_ABC >( pParent, id, label, menu )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAutomat destructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomat::~ParamAutomat()
{
    // NOTHING
}
