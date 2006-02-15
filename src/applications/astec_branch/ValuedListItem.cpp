// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ValuedListItem.h"

// -----------------------------------------------------------------------------
// Name: ValuedListItem::rtti
// Created: AGE 2005-09-15
// -----------------------------------------------------------------------------
int ValuedListItem::rtti() const
{
    return container_.rtti();
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem destructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
ValuedListItem::~ValuedListItem()
{
    delete &container_;
}
