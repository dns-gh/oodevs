// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "score_plugin_pch.h"
#include "ScoresModel.h"
#include "IndicatorBuilder.h"
#include "Score.h"
#include "ScoreAnnouncer.h"
#include "3a/AarFacade.h"
#include "3a/Task.h"
#include "3a/StaticModel.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/protocol.h"
#include "tools/SessionConfig.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>
#include <directia/brain/Brain.h>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

using namespace plugins::score;

namespace
{
    QDateTime MakeDate( const std::string& str )
    {
        // $$$$ AGE 2007-10-12: ...
        QString extended( str.c_str() );
        extended.insert( 13, ':' ); extended.insert( 11, ':' );
        extended.insert(  6, '-' ); extended.insert(  4, '-' );
        return QDateTime::fromString( extended, Qt::ISODate );
    }
    const std::string separator = ";";
}

// -----------------------------------------------------------------------------
// Name: ScoresModel constructor
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
ScoresModel::ScoresModel( const tools::SessionConfig& config, dispatcher::ClientPublisher_ABC& clients )
    : clients_            ( clients )
    , builder_            ( new IndicatorBuilder( config ) )
    , model_              ( new aar::StaticModel( config ) )
    , announcer_          ( new ScoreAnnouncer() )
    , dateTimeInitialized_( false )
    , tickDuration_       ( 0 )
    , sessionDir_         ( config.GetSessionDir() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoresModel destructor
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
ScoresModel::~ScoresModel()
{
    Export();
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Load
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
void ScoresModel::Load( const std::string& file )
{
    if( bfs::exists( bfs::path( file, bfs::native ) ) )
    {
        xml::xifstream xis( file );
        xis >> xml::start( "scores" )
                >> xml::list( "score", *this, &ScoresModel::ReadIndicators );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::ReadIndicators
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
void ScoresModel::ReadIndicators( xml::xistream& xis )
{
    T_Score score( new Score( xis ) );
    scores_[ xis.attribute< std::string >( "name" ) ] = score;
    xis >> xml::start( "indicators" )
            >> xml::list( "indicator", *this, &ScoresModel::ReadIndicator )
        >> xml::end;
    score->Accept( *announcer_ );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::ReadIndicator
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScoresModel::ReadIndicator( xml::xistream& xis )
{
    try
    {
        AarFacade facade( clients_, static_cast< int >( tasks_.size() ), *model_ );
        tasks_.push_back( facade.CreateTask( xis ) );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error loading score indicator: " << e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Update
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
void ScoresModel::Update( const sword::Indicator& message )
{
    T_Scores::const_iterator it = scores_.find( message.name() );
    if( it != scores_.end() )
        it->second->Update( message );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Update
// Created: SBO 2009-08-21
// -----------------------------------------------------------------------------
void ScoresModel::Update( const sword::SimToClient& message )
{
    BOOST_FOREACH( const T_Task& task, tasks_ )
        task->Receive( message );
    if( !dateTimeInitialized_ && message.message().has_control_information() )
    {
        initialDateTime_ = MakeDate( message.message().control_information().initial_date_time().data() );
        tickDuration_ = message.message().control_information().tick_duration();
        dateTimeInitialized_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::RequestPlot
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
void ScoresModel::RequestPlot( dispatcher::ClientPublisher_ABC& publisher, const sword::PlotRequest& request )
{
    if( boost::starts_with( request.request(), "indicator://" ) )
    {
        const std::string name = boost::erase_head_copy< std::string >( request.request(), 12 );
        T_Scores::const_iterator it = scores_.find( name );
        if( it != scores_.end() )
            it->second->Send( publisher, request.identifier() );
    }
}

namespace directia
{
    void UsedByDIA( ScoresModel* ) {}
    void ReleasedByDIA( ScoresModel* ) {}
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::RegisterIn
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
void ScoresModel::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "indicators" ] = this;
    brain.Register( "CreateVariable", &ScoresModel::CreateVariable );
    brain.Register( "Compute", &ScoresModel::ComputeIndicator );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::CreateVariable
// Created: SBO 2009-08-21
// -----------------------------------------------------------------------------
boost::shared_ptr< Variable > ScoresModel::CreateVariable( const std::string& name, const std::string& type, const std::string& value )
{
    return boost::shared_ptr< Variable >( new Variable( name, type, value ) );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::ComputeIndicator
// Created: SBO 2009-08-21
// -----------------------------------------------------------------------------
void ScoresModel::ComputeIndicator( const std::string& name, const std::string& formula, const std::vector< boost::shared_ptr< Variable > >& variables )
{
    try
    {
        builder_->Start();
        typedef std::vector< boost::shared_ptr< Variable > > T_Variables;
        BOOST_FOREACH( const T_Variables::value_type var, variables )
            builder_->AddVariable( var->name_, var->type_, var->value_ );
        xml::xobufferstream xos;
        builder_->Build( name, formula, xos );
        xos >> xml::start( "indicator" );
        ReadIndicator( xos );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error in indicator '" << name << "' definition: " << e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Export
// Created: ABR 2011-02-08
// -----------------------------------------------------------------------------
void ScoresModel::Export() const
{
    if( dateTimeInitialized_ )
        try
        {
            bfs::path bfspath( sessionDir_ + "/scores.csv" );
            bfs::ofstream file( bfspath );
            const std::size_t size = AddHeader( file );
            for( std::size_t index = 0; index < size; ++index )
                AddLine( file, index );
            file.close();
        }
        catch( std::exception& e )
        {
            throw std::runtime_error( __FUNCTION__ ": Can not save scores.csv file : Error message" + std::string( e.what() ) );
        }
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::SimplifiedExport
// Created: FPO 2011-03-24
// -----------------------------------------------------------------------------
void ScoresModel::SimplifiedExport( const std::string& path ) const
{
    if( dateTimeInitialized_ )
        try
        {
            bfs::path bfspath( path + "/scores.csv" );
            bfs::ofstream file( bfspath );
            const std::size_t size = AddHeader( file );
            if( size )
            {
                AddLine( file, 0 );
                AddLine( file, size - 1 );
            }
            file.close();
        }
        catch( std::exception& e )
        {
            throw std::runtime_error( __FUNCTION__ ": Can not save scores.csv file : Error message" + std::string( e.what() ) );
        }
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::AddHeader
// Created: FPO 2011-03-24
// -----------------------------------------------------------------------------
std::size_t ScoresModel::AddHeader( std::ostream& file ) const
{
    std::size_t size = 0;
    file << "Tick" << separator << "Time";
    BOOST_FOREACH( const T_Scores::value_type& score, scores_ )
    {
        file << separator << score.first;
        if ( ! size )
            size = score.second->Size();
        else if ( size != score.second->Size() )
            throw std::runtime_error( __FUNCTION__ ": not the same number of score." );
    }
    file << std::endl;
    return size;
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::AddLine
// Created: FPO 2011-03-24
// -----------------------------------------------------------------------------
void ScoresModel::AddLine( std::ostream& file, std::size_t index ) const
{
    file << index << separator << initialDateTime_.addSecs( static_cast< int >( index * tickDuration_ ) ).toString( Qt::ISODate ).ascii();
    BOOST_FOREACH( const T_Scores::value_type& score, scores_ )
        file << separator << score.second->GetValue( index );
    file << std::endl;
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::SendInformation
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void ScoresModel::SendInformation( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile )
{
    announcer_->Announce( client, profile );
}
