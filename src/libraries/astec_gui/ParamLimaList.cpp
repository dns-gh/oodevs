// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ParamLimaList.h"
#include "astec_gaming/Lima.h"

// -----------------------------------------------------------------------------
// Name: ParamLimaList constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamLimaList::ParamLimaList( QWidget* pParent, ASN1T_ListOID& asn, const std::string& label, const std::string& menu )
    : EntityListParameter< Lima >( pParent, asn.n, asn.elem, label, menu )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamLimaList::~ParamLimaList()
{
    // NOTHING
}
