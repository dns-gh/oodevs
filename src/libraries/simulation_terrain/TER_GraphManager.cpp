// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_GraphManager.h"

// -----------------------------------------------------------------------------
// Name: TER_GraphManager constructor
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
TER_GraphManager::TER_GraphManager( const tools::Path& strGraphArchive, const tools::Path& strNodeArchive, const tools::Path& strLinkArchive, float precision )
    : staticData_( strGraphArchive, strNodeArchive, strLinkArchive, precision )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_GraphManager destructor
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
TER_GraphManager::~TER_GraphManager()
{
    // NOTHING
}
