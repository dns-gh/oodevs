// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "geostore_pch.h"
#include "ProjectionTable.h"
#include "DatabaseException.h"

using namespace geostore;

// -----------------------------------------------------------------------------
// Name: ProjectionTable constructor
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
ProjectionTable::ProjectionTable( sqlite3* db )
    : Table( db, "spatial_ref_sys" )
{
    ExecuteQuery( "SELECT InitSpatialMetadata()" );
    ExecuteQuery(
        "REPLACE INTO"
        "   spatial_ref_sys ( srid, auth_name, auth_srid, ref_sys_name, proj4text ) "
        "VALUES"
        "   ( 4326, 'epgs', 4326, 'WGS 84', '+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs' )"
        );
}

// -----------------------------------------------------------------------------
// Name: ProjectionTable destructor
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
ProjectionTable::~ProjectionTable()
{
    // NOTHING
}
