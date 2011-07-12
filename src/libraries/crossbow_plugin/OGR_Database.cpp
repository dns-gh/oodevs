// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "OGR_Database.h"
#include "OGR_Workspace.h"
#include "OGR_FeatureClass.h"
#include <algorithm>
#include <gdal/ogrsf_frmts.h>

using namespace plugins;
using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: OGR_Database constructor
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
OGR_Database::OGR_Database( OGRDataSource* datasource )
    : datasource_ ( datasource, &OGRDataSource::DestroyDataSource )
    , name_ ( "" )
    , schema_ ( "" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OGR_Database constructor
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
OGR_Database::OGR_Database( OGRDataSource* datasource, const std::string& name, const std::string& schema )
    : datasource_ ( datasource, &OGRDataSource::DestroyDataSource )
    , name_ ( name )
    , schema_ ( schema )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OGR_Database destructor
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
OGR_Database::~OGR_Database()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OGR_Database::GetTableName
// Created: JCR 2010-02-25
// -----------------------------------------------------------------------------
std::string OGR_Database::GetTableName( const std::string& name ) const
{
    std::string result( name );
    std::transform( result.begin(), result.end(), result.begin(), ::tolower );
    if( name_ != "" && schema_ != "" )
        return name_ + "." + schema_ + "." + result;
    return result;
}

// -----------------------------------------------------------------------------
// Name: OGR_Database::OpenTable
// open feature dataset / layer
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
Table_ABC* OGR_Database::OpenTable( const std::string& name )
{
    OGRLayer* layer = datasource_->GetLayerByName( GetTableName( name ).c_str() );
    if( !layer )
        throw std::runtime_error( "Unable to open table : " + GetTableName( name ) );
    return new OGR_FeatureClass( *layer );
}

// -----------------------------------------------------------------------------
// Name: OGR_Database::OpenBufferedTable
// Created: JCR 2010-02-25
// -----------------------------------------------------------------------------
Table_ABC& OGR_Database::OpenBufferedTable( const std::string& name, bool clear /*= true*/ )
{
    T_TablePtr& table = tables_[ name ];
    if( !table )
    {
        table.reset( OpenTable( name ) );
        if( table && clear )
            table->Clear();
    }
    if( !table )
        throw std::runtime_error( "Unable to open table : " + GetTableName( name ) );
    return *table;
}

// -----------------------------------------------------------------------------
// Name: OGR_Database::ClearTable
// Created: JCR 2010-02-27
// -----------------------------------------------------------------------------
void OGR_Database::ClearTable( const std::string& name, const std::string& filter )
{
    std::auto_ptr< Table_ABC > table( OpenTable( name ) );
    table->DeleteRows( filter );
}

// -----------------------------------------------------------------------------
// Name: OGR_Database::Flush
// Created: JCR 2010-02-27
// -----------------------------------------------------------------------------
void OGR_Database::Flush()
{
    if( OGRERR_NONE != datasource_->SyncToDisk() )
        throw std::runtime_error( "Crossbow plugin : OGR_Database::SyncToDisk failed" );
}
