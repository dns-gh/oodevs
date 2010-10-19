// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Model.h"
#include "StaticModel.h"
#include "TeamsModel.h"
#include "TeamFactory.h"
#include "KnowledgeGroupsModel.h"
#include "AgentFactory.h"
#include "AgentsModel.h"
#include "Exercise.h"
#include "FormationFactory.h"
#include "FormationModel.h"
#include "IdManager.h"
#include "KnowledgeGroupFactory.h" // LTO
#include "LimitsModel.h"
#include "WeatherModel.h"
#include "ProfilesModel.h"
#include "ProfileFactory.h"
#include "ScoresModel.h"
#include "ScoreFactory.h"
#include "SuccessFactorsModel.h"
#include "SuccessFactorFactory.h"
#include "IntelligencesModel.h"
#include "OrbatReIndexer.h"
#include "UrbanModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ResourceNetworkSelectionObserver.h"
#include "clients_gui/DrawerFactory.h"
#include "clients_gui/DrawerModel.h"
#include "indicators/GaugeTypes.h"
#include "tools/ExerciseConfig.h"
#include <urban/WorldParameters.h>
#include <xeumeuleu/xml.hpp>

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )
namespace bfs = boost::filesystem;

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::Model( Controllers& controllers, const StaticModel& staticModel )
    : controllers_( controllers )
    , idManager_( *new IdManager() )
    , teamFactory_( *new TeamFactory( controllers, *this, staticModel, idManager_ ) )
    , knowledgeGroupFactory_( *new KnowledgeGroupFactory( controllers, staticModel, idManager_ ) ) // LTO
    , formationFactory_( *new FormationFactory( controllers, staticModel, idManager_ ) )
    , agentFactory_( *new AgentFactory( controllers, *this, staticModel, idManager_, knowledgeGroupFactory_ ) )
    , profileFactory_( *new ProfileFactory( controllers.controller_, *this ) )
    , scoreFactory_( *new ScoreFactory( controllers_.controller_, staticModel.indicators_, staticModel.gaugeTypes_ ) )
    , successFactorFactory_( *new SuccessFactorFactory( controllers_, *this, staticModel.successFactorActionTypes_ ) )
    , drawingFactory_( *new gui::DrawerFactory( controllers.controller_, staticModel.drawings_ ) )
    , resourceObserver_( *new ResourceNetworkSelectionObserver( controllers ) )
    , loaded_ ( false )
    , exercise_( *new Exercise( controllers.controller_ ) )
    , teams_( *new TeamsModel( controllers, teamFactory_ ) )
    , knowledgeGroups_( *new KnowledgeGroupsModel( controllers, knowledgeGroupFactory_ ) ) // LTO
    , agents_( *new AgentsModel( controllers, agentFactory_ ) ) // needs to be there : used in FormationModel
    , formations_( *new FormationModel( controllers, formationFactory_, agents_ ) )
    , limits_( *new LimitsModel( controllers, staticModel.coordinateConverter_, idManager_ ) )
    , weather_( *new WeatherModel( controllers.controller_, staticModel.coordinateConverter_ ) )
    , profiles_( *new ProfilesModel( profileFactory_ ) )
    , scores_( *new ScoresModel( scoreFactory_ ) )
    , successFactors_( *new SuccessFactorsModel( successFactorFactory_ ) )
    , intelligences_( *new IntelligencesModel( controllers.controller_, staticModel.coordinateConverter_, idManager_, staticModel.levels_ ) )
    , urban_( *new UrbanModel( controllers, staticModel ) )
    , drawings_( *new gui::DrawerModel( controllers, drawingFactory_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::~Model()
{
    delete &intelligences_;
    delete &successFactors_;
    delete &successFactorFactory_;
    delete &scores_;
    delete &scoreFactory_;
    delete &profiles_;
    delete &profileFactory_;
    delete &resourceObserver_;
    delete &weather_;
    delete &limits_;
    delete &agents_;
    delete &agentFactory_;
    delete &formations_;
    delete &formationFactory_;
    delete &knowledgeGroups_;
    delete &teams_;
    delete &teamFactory_;
    delete &urban_;
    delete &idManager_;
}

// -----------------------------------------------------------------------------
// Name: Model::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void Model::Purge()
{
    UpdateName( "orbat" );
    urban_.Purge();
    intelligences_.Purge();
    successFactors_.Purge();
    scores_.Purge();
    profiles_.Purge();
    weather_.Purge();
    limits_.Purge();
    agents_.Purge();
    formations_.Purge();
    knowledgeGroups_.Purge();
    teams_.Purge();
    exercise_.Purge();
    idManager_.Reset();
    SetLoaded( false );
}

namespace
{
    template< typename M >
    bool LoadOptional( const std::string& file, M& model )
    {
        if( bfs::exists( file ) )
        {
            model.Load( file );
            return true;
        }
        model.Serialize( file );
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: Model::Load
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void Model::Load( const tools::ExerciseConfig& config, std::string& loadingErrors )
{
    {
        xml::xifstream xis( config.GetExerciseFile() );
        exercise_.Load( xis );
    }
    {
        const std::string orbatFile = config.GetOrbatFile() ;
        if( bfs::exists( bfs::path( orbatFile, bfs::native ) ) )
        {
            UpdateName( orbatFile );
            xml::xifstream xis( orbatFile );
            teams_.Load( xis, *this, loadingErrors );
        }
    }
    {
        std::string directoryPath = boost::filesystem::path( config.GetTerrainFile() ).branch_path().string();
        try
        {
            urban::WorldParameters world( directoryPath );
            urban_.Load( directoryPath, world );
            const std::string urbanStateFile = config.GetUrbanStateFile() ;
            if( bfs::exists( bfs::path( urbanStateFile, bfs::native ) ) )
            {
                xml::xifstream xis( urbanStateFile );
                urban_.LoadUrbanState( xis );
            }
        }
        catch( std::exception& )
        {
        }
    }
    if( ! LoadOptional( config.GetWeatherFile(), weather_ ) )
        controllers_.controller_.Update( weather_);
    LoadOptional( config.GetProfilesFile(), profiles_ );
    LoadOptional( config.GetScoresFile(), scores_ );
    LoadOptional( config.GetSuccessFactorsFile(), successFactors_ );
    SetLoaded( true );
}

// -----------------------------------------------------------------------------
// Name: Model::Import
// Created: SBO 2008-04-07
// -----------------------------------------------------------------------------
void Model::Import( const std::string& orbat, const OrbatImportFilter& filter, std::string& loadingErrors )
{
    xml::xifstream xis( orbat );
    xml::xostringstream xos;
    OrbatReIndexer reindexer( xis, xos, idManager_, filter );
    xml::xistringstream newXis( xos.str() );
    teams_.Load( newXis, *this, loadingErrors );
}

// -----------------------------------------------------------------------------
// Name: Model::Save
// Created: SBO 2006-11-21
// -----------------------------------------------------------------------------
bool Model::Save( const tools::ExerciseConfig& config, ModelChecker_ABC& checker )
{
    const bool valid = teams_.CheckValidity( checker )
                    && agents_.CheckValidity( checker )
                    && profiles_.CheckValidity( *this, checker )
                    && scores_.CheckValidity( checker )
                    && successFactors_.CheckValidity( checker );
    if( valid )
    {
        exercise_.Serialize( config.GetExerciseFile() );
        {
            xml::xofstream xos( config.GetOrbatFile(), xml::encoding( "ISO-8859-1" ) );
            xos << xml::start( "orbat" );
            teams_.Serialize( xos );
            xos << xml::end;
        }
        urban_.Serialize( config.GetUrbanStateFile() );
        weather_.Serialize( config.GetWeatherFile() );
        profiles_.Serialize( config.GetProfilesFile() );
        scores_.Serialize( config.GetScoresFile() );
        successFactors_.Serialize( config );
        UpdateName( config.GetOrbatFile() );
    }
    return valid;
}

// -----------------------------------------------------------------------------
// Name: Model::GetName
// Created: SBO 2006-11-21
// -----------------------------------------------------------------------------
QString Model::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Model::UpdateName
// Created: SBO 2006-11-21
// -----------------------------------------------------------------------------
void Model::UpdateName( const std::string& orbat )
{
    if( orbat.empty() )
        name_ = "";
    else
    {
        std::string file = bfs::path( orbat, bfs::native ).leaf();
        file = file.substr( 0, file.find_last_of( '.' ) );
        name_ = file.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: Model::IsLoaded
// Created: RPD 2010-02-05
// -----------------------------------------------------------------------------
bool Model::IsLoaded() const
{
    return loaded_;
}

// -----------------------------------------------------------------------------
// Name: Model::SetLoaded
// Created: RPD 2010-02-05
// -----------------------------------------------------------------------------
void Model::SetLoaded( bool status )
{
    loaded_ = status;
}