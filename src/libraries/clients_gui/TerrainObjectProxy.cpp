// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TerrainObjectProxy.h"

using namespace gui;
// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
TerrainObjectProxy::TerrainObjectProxy( urban::TerrainObject_ABC& object )
: object_( &object )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy destructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
TerrainObjectProxy::~TerrainObjectProxy()
{
}


// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy copy
// Created: MGD 2009-11-2
// -----------------------------------------------------------------------------
TerrainObjectProxy::TerrainObjectProxy( const TerrainObjectProxy& object )
    : object_( object.object_ )
{
   
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy assignement
// Created: MGD 2009-11-2
// -----------------------------------------------------------------------------
TerrainObjectProxy& TerrainObjectProxy::operator=( const TerrainObjectProxy& object )
{
    object_ = object.object_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy operator==
// Created: MGD 2009-11-2
// -----------------------------------------------------------------------------
bool TerrainObjectProxy::operator==( const TerrainObjectProxy& object ) const
{
    return object_ == object.object_;
}