// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Objects.h"
#include "Object.h"

Objects::Objects()
{
    // NOTHING
}

Objects::~Objects()
{
    // NOTHING
}

void Objects::AddObject( const Object& object )
{
    Register( object.GetId(), object );
}
