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
#include "3a/AarFacade.h"
#include "3a/Task.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>
#include <directia/Brain.h>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

using namespace plugins::score;

// -----------------------------------------------------------------------------
// Name: ScoresModel constructor
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
ScoresModel::ScoresModel( dispatcher::ClientPublisher_ABC& clients )
    : clients_( clients )
    , builder_( new IndicatorBuilder() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoresModel destructor
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
ScoresModel::~ScoresModel()
{
    for( T_Scores::const_iterator it = scores_.begin(); it != scores_.end(); ++it )
        delete it->second;
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
                >> xml::list( "score", *this, &ScoresModel::ReadIndicators )
            >> xml::end();
    }
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::ReadIndicators
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
void ScoresModel::ReadIndicators( xml::xistream& xis )
{
    scores_[ xml::attribute< std::string >( xis, "name" ) ] = new Score();
    xis >> xml::start( "indicators" )
            >> xml::list( "indicator", *this, &ScoresModel::ReadIndicator )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::ReadIndicator
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScoresModel::ReadIndicator( xml::xistream& xis )
{
    try
    {
        AarFacade facade( clients_, tasks_.size() );
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
void ScoresModel::Update( const ASN1T_MsgIndicator& message )
{
    T_Scores::const_iterator it = scores_.find( message.name );
    if( it != scores_.end() )
        it->second->Update( message );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Update
// Created: SBO 2009-08-21
// -----------------------------------------------------------------------------
void ScoresModel::Update( const ASN1T_MsgsSimToClient& message )
{
    BOOST_FOREACH( const std::vector< boost::shared_ptr< Task > >::value_type& task, tasks_ )
        task->Receive( message );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::RequestPlot
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
void ScoresModel::RequestPlot( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgPlotRequest& request )
{
    const std::string name = boost::erase_head_copy< std::string >( request.request, 12 );
    T_Scores::const_iterator it = scores_.find( name );
    if( it != scores_.end() )
        it->second->Send( publisher, request.identifier );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::RegisterIn
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
void ScoresModel::RegisterIn( directia::Brain& brain )
{
    brain.RegisterObject( "indicators", this );
    brain.RegisterFunction( "CreateVariable", &ScoresModel::CreateVariable );
    brain.RegisterFunction( "Compute", &ScoresModel::ComputeIndicator );
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
