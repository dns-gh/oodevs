// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "ReportUpdater.h"
#include "Workspace_ABC.h"
#include "ReportFactory.h"
#include "WorkingSession.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"


#include <boost/lexical_cast.hpp>

using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: ReportUpdater constructor
// Created: JCR 2010-03-23
// -----------------------------------------------------------------------------
ReportUpdater::ReportUpdater( Workspace_ABC& workspace, const dispatcher::Config& config, const dispatcher::Model& model, const WorkingSession& session )
    : database_ ( workspace.GetDatabase( "flat" ) )
    , reportFactory_ ( new ReportFactory( config, model ) )
    , session_ ( session )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReportUpdater destructor
// Created: JCR 2010-03-23
// -----------------------------------------------------------------------------
ReportUpdater::~ReportUpdater()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReportUpdater::Clean
// Created: JCR 2010-03-23
// -----------------------------------------------------------------------------
void ReportUpdater::Clean()
{
    try
    {
        const std::string clause( "session_id=" + boost::lexical_cast< std::string >( session_.GetId() ) );
        database_.ClearTable( "Reports", clause );
    }
    catch ( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "QueryDatabaseUpdater is not correctly loaded : " + std::string( e.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void ReportUpdater::Update( const MsgsSimToClient::MsgReport& msg )
{
    std::auto_ptr< Table_ABC > table( database_.OpenTable( "Reports" ) );
    
    Row_ABC& row = table->CreateRow();
    row.SetField( "unit_id", FieldVariant( msg.oid() ) );
    row.SetField( "message", FieldVariant( reportFactory_->CreateMessage( msg ) ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );
    table->InsertRow( row );
}