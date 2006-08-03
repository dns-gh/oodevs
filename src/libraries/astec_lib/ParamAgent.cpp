// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ParamAgent.h"
#include "Agent_ABC.h"

// -----------------------------------------------------------------------------
// Name: ParamAgent constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgent::ParamAgent(  QWidget* pParent, ASN1T_OID& id, const std::string& label, const std::string& menu )
    : EntityParameter< Agent_ABC >( pParent, id, label, menu )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgent destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgent::~ParamAgent()
{
    // NOTHING
}
