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

using namespace geostore;

// -----------------------------------------------------------------------------
// Name: ProjectionTable constructor
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
ProjectionTable::ProjectionTable( sqlite3* db )
{
    char* err_msg;
    std::string sqlRequest = "SELECT InitSpatialMetadata()";
    sqlite3_exec( db, sqlRequest.c_str(), NULL, NULL, &err_msg );
    sqlRequest = "INSERT INTO spatial_ref_sys (srid, auth_name, auth_srid, ref_sys_name, proj4text) VALUES (4326, 'epgs', 4326, 'WGS 84', '+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs ');";
    sqlite3_exec( db, sqlRequest.c_str(), NULL, NULL, &err_msg );
    sqlite3_free( err_msg );

}
// -----------------------------------------------------------------------------
// Name: ProjectionTable destructor
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
ProjectionTable::~ProjectionTable()
{
    // NOTHING
}
