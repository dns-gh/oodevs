// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "Reports.h"
#include "MT_Tools/MT_Logger.h"
#include "clients_kernel/XmlAdapter.h"
#include "tools/Sql.h"
#include "protocol/Protocol.h"
#include "protocol/XmlWriters.h"
#include "protocol/XmlReaders.h"
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
            ", source_type  INTEGER NOT NULL"
            ", type         INTEGER NOT NULL"
            ", category     INTEGER NOT NULL"
            ", time         DATETIME NOT NULL"
            ", tick         INTEGER NOT NULL"
            ", parameters   TEXT"
            ")" ) );
        db.Commit( *tr );
    }
}

Reports::Reports( const tools::Path& filename )
    : database_( new tools::Sql( filename ) )
    , tick_    ( 0 )
{
    MakeTable( *database_ );
}

Reports::~Reports()
{
    // NOTHING
}

namespace
{
    enum E_Type
    {
        eUnit = 0,
        eAutomat,
        eCrowd,
        eFormation,
        eParty,
        ePopulation
    };
    boost::optional< std::pair< uint32_t, uint32_t > > TaskerToId( const sword::Tasker& tasker )
    {
        if( tasker.has_unit() )
            return std::make_pair( tasker.unit().id(), eUnit );
        if( tasker.has_automat() )
            return std::make_pair( tasker.automat().id(), eAutomat );
        if( tasker.has_crowd() )
            return std::make_pair( tasker.crowd().id(), eCrowd );
        if( tasker.has_formation() )
            return std::make_pair( tasker.formation().id(), eFormation );
        if( tasker.has_party() )
            return std::make_pair( tasker.party().id(), eParty );
        if( tasker.has_population() )
            return std::make_pair( tasker.population().id(), ePopulation );
        return boost::none;
    }
    sword::Tasker IdToTasker( int id, int type )
    {
        sword::Tasker tasker;
        switch( type )
        {
        case eUnit:
            tasker.mutable_unit()->set_id( id );
            break;
        case eAutomat:
            tasker.mutable_automat()->set_id( id );
            break;
        case eCrowd:
            tasker.mutable_crowd()->set_id( id );
            break;
        case eFormation:
            tasker.mutable_formation()->set_id( id );
            break;
        case eParty:
            tasker.mutable_party()->set_id( id );
            break;
        case ePopulation:
            tasker.mutable_population()->set_id( id );
            break;
        }
        return tasker;
    }
}

void Reports::AddReport( const sword::Report& report )
{
    try
    {
        const auto source = TaskerToId( report.source() );
        if( !source )
            throw MASA_EXCEPTION( "Invalid tasker" );

        auto tr = database_->Begin( true );
        auto st = database_->Prepare( *tr,
            "INSERT INTO reports ("
            "            id "
            ",           source "
            ",           source_type "
            ",           type "
            ",           category "
            ",           time "
            ",           tick "
            ",           parameters "
            ") VALUES  ( ?, ?, ?, ?, ?, ?, ?, ? ) "
            );
        st->Bind( static_cast< int64_t >( report.report().id() ) );
        st->Bind( static_cast< int >( source->first ) );
        st->Bind( static_cast< int >( source->second ) );
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
        else
            st->Bind();

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
    void MakeReport( sword::ListReportsAck& reports, tools::Statement_ABC& st, unsigned int count )
    {
        if( static_cast< unsigned int >( reports.reports_size() ) < count )
        {
            auto& report = *reports.add_reports();
            report.mutable_report()->set_id( static_cast< int32_t >( st.ReadInt64() ) );
            const auto sourceId = st.ReadInt();
            const auto sourceType = st.ReadInt();
            *report.mutable_source() = IdToTasker( sourceId, sourceType );
            report.mutable_type()->set_id( st.ReadInt() );
            report.set_category( sword::Report_EnumReportType( st.ReadInt() ) );
            report.mutable_time()->set_data( st.ReadText() );
            st.SkipNull();
            if( st.IsColumnDefined() )
                protocol::Read( kernel::XmlReaderEmptyAdapter(), *report.mutable_parameters(),
                    xml::xistringstream( st.ReadText() ) >> xml::start( "parameters" ) );
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
            ",      source_type "
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
            MakeReport( reports, *st, count );
    }
    catch( const tools::SqlException& err )
    {
        reports.set_error_code( sword::ListReportsAck::error_invalid_request );
        reports.set_error_msg( err.msg );
    }
}

void Reports::Save( const tools::Path& filename )
{
    try
    {
        database_->Save( filename );
    }
    catch( const tools::SqlException& err )
    {
        MT_LOG_ERROR_MSG( err.msg );
    }
}

void Reports::SetTick( int tick )
{
    tick_ = tick;
}
