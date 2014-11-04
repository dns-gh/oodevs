// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "Reports.h"
#include "dispatcher/Model_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "clients_kernel/XmlAdapter.h"
#include "tools/SessionConfig.h"
#include "tools/Sql.h"
#include "protocol/Protocol.h"
#include "protocol/XmlWriters.h"
#include "protocol/XmlReaders.h"
#include "protocol/MessageParameters.h"
#include <boost/optional.hpp>
#include <xeumeuleu/xml.hpp>

using namespace plugins::reports;

namespace
{
    size_t Execute( tools::Statement_ABC& st )
    {
        size_t count = 0;
        while( st.Next() )
            count++;
        return count;
    }
    void MakeTable( tools::Sql_ABC& db )
    {
        tools::Sql_ABC::T_Transaction tr = db.Begin();
        Execute( *db.Prepare( *tr,
            "CREATE TABLE IF NOT EXISTS reports ("
            "  id           INTEGER PRIMARY KEY"
            ", source       INTEGER NOT NULL"
            ", type         INTEGER NOT NULL"
            ", category     INTEGER NOT NULL"
            ", time         DATETIME NOT NULL"
            ", tick         INTEGER NOT NULL"
            ", parameters   TEXT"
            ")" ) );
        db.Commit( *tr );
    }

    std::unique_ptr< tools::Sql_ABC > InitializeDatabase( const tools::SessionConfig& config, const std::string& filename )
    {
        const auto sessionPath = config.BuildSessionChildFile( filename.c_str() );
        sessionPath.Remove();
        if( config.HasCheckpoint() )
        {
            const auto checkpointPath = config.GetCheckpointDirectory() / filename.c_str();
            checkpointPath.Copy( sessionPath, tools::Path::OverwriteIfExists );
        }
        return std::unique_ptr< tools::Sql_ABC >( new tools::Sql( sessionPath ) );
    }
}

Reports::Reports( const tools::SessionConfig& config, const dispatcher::Model_ABC& model )
    : database_( InitializeDatabase( config, "reports.db" ) )
    , config_  ( config )
    , model_   ( model )
    , tick_    ( 0 )
{
    MakeTable( *database_ );
}

Reports::~Reports()
{
    // NOTHING
}

void Reports::AddReport( const sword::Report& report )
{
    try
    {
        const auto sourceId = protocol::TryGetTasker( report.source() );
        if( !sourceId )
            throw MASA_EXCEPTION( "Invalid tasker" );

        auto tr = database_->Begin( true );
        auto st = database_->Prepare( *tr,
            "INSERT INTO reports ("
            "            id "
            ",           source "
            ",           type "
            ",           category "
            ",           time "
            ",           tick "
            ",           parameters "
            ") VALUES  ( ?, ?, ?, ?, ?, ?, ? ) "
            );
        st->Bind( static_cast< int64_t >( report.report().id() ) );
        st->Bind( static_cast< int >( *sourceId ) );
        st->Bind( static_cast< int >( report.type().id() ) );
        st->Bind( report.category() );
        st->Bind( report.time().data() );
        st->Bind( tick_ );
        if( report.has_parameters() )
        {
            xml::xostringstream xos;
            xos << xml::start( "parameters" );
            protocol::Write( xos, kernel::XmlWriterEmptyAdapter(), report.parameters() );
            xos << xml::end;
            st->Bind( xos.str() );
        }
        Execute( *st );
        database_->Commit( *tr );
    }
    catch( const tools::SqlException& err )
    {
        MT_LOG_ERROR_MSG( err.msg );
    }
}

namespace
{
    void MakeReport( sword::ListReportsAck& reports, tools::Statement_ABC& st,
                     const dispatcher::Model_ABC& model, unsigned int count )
    {
        if( static_cast< unsigned int >( reports.reports_size() ) < count )
        {
            auto& report = *reports.add_reports();
            report.mutable_report()->set_id( static_cast< int32_t >( st.ReadInt64() ) );
            model.SetToTasker( *report.mutable_source(), st.ReadInt() );
            report.mutable_type()->set_id( st.ReadInt() );
            report.set_category( sword::Report_EnumReportType( st.ReadInt() ) );
            report.mutable_time()->set_data( st.ReadText() );
            if( st.IsColumnDefined() )
            {
                protocol::Read( kernel::XmlReaderEmptyAdapter(), *report.mutable_parameters(),
                    xml::xistringstream( st.ReadText() ) >> xml::start( "parameters" ) );
            }
        }
        else
            reports.set_next_report( static_cast< int32_t >( st.ReadInt64() ) );
    }
}

void Reports::ListReports( sword::ListReportsAck& reports, unsigned int count, unsigned int from )
{
    try
    {
        std::string sql =
            "SELECT id "
            ",      source "
            ",      type "
            ",      category "
            ",      time "
            ",      tick "
            ",      parameters "
            "FROM   reports ";
        if( from != 0 )
            sql += "WHERE id >= ? ";
        sql += "ORDER BY id ASC "
               "LIMIT  ? ";

        tools::Sql_ABC::T_Transaction tr = database_->Begin( false );
        tools::Sql_ABC::T_Statement st = database_->Prepare( *tr, sql );
        if( from != 0 )
            st->Bind( static_cast< int64_t >( from ) );
        st->Bind( static_cast< int >( count + 1 ) );

        while( st->Next() )
            MakeReport( reports, *st, model_, count );
    }
    catch( const tools::SqlException& err )
    {
        MT_LOG_ERROR_MSG( err.msg );
    }
}

void Reports::Save( const std::string& path )
{
    try
    {
        database_->Save( config_.GetCheckpointDirectory( tools::Path::FromUTF8( path ) / "reports.db" ) );
    }
    catch( const tools::SqlException& err )
    {
        MT_LOG_ERROR_MSG( err.msg );
    }
}

void Reports::Update( int tick )
{
    tick_ = tick;
}
