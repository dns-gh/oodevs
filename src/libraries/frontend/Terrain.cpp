// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Terrain.h"
#include "clients_kernel/Controller.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Terrain constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
Terrain::Terrain( const ASN1T_MsgTerrainCreation& message, kernel::Controller& controller )
    : controller_( controller )
    , name_( message.name )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Terrain destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
Terrain::~Terrain()
{
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: Terrain::GetName
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
QString Terrain::GetName() const
{
    return name_;
}
