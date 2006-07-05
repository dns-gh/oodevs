// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ParamAgentList.h"
#include "Agent.h"

// -----------------------------------------------------------------------------
// Name: ParamAgentList constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentList::ParamAgentList( QWidget* pParent, ASN1T_ListAgent& asn, const std::string& label, const std::string& menu )
    : EntityListParameter< Agent >( pParent, asn.n, asn.elem, label, menu )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentList::~ParamAgentList()
{
    // NOTHING
}
