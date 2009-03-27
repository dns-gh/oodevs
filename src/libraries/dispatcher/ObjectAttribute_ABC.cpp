// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ObjectAttribute_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ObjectAttribute_ABC constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ObjectAttribute_ABC::ObjectAttribute_ABC( const Model& /*model*/, const ASN1T_ObjectAttributes& asnMsg )
    : nType_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectAttribute_ABC destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ObjectAttribute_ABC::~ObjectAttribute_ABC()
{
    // NOTHING
}  

