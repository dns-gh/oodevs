// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectFactory.h"
#include "Object_Factory.h"

// -----------------------------------------------------------------------------
// Name: ObjectFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
ObjectFactory::ObjectFactory()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ObjectFactory destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
ObjectFactory::~ObjectFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Object_ABC* ObjectFactory::Create( const ASN1T_MsgObjectCreation& asnMsg )
{
    return Object_Factory::Create( asnMsg );
}
