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

Objects::Objects( kernel::Controllers& controllers )
    : tools::TrackingResolver< const Object, kernel::Object_ABC >( controllers )
{
    // NOTHING
}

Objects::~Objects()
{
    DeleteAll();
}

void Objects::AddObject( const Object& object )
{
    Register( object.GetId(), object );
}
