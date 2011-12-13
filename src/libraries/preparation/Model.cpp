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
#include "GhostFactory.h"
#include "GhostModel.h"
#include "IdManager.h"
#include "KnowledgeGroupFactory.h" // LTO
#include "LimitsModel.h"
#include "WeatherModel.h"
#include "ObjectFactory.h"
#include "ObjectsModel.h"
#include "ProfilesModel.h"
#include "ProfileFactory.h"
#include "ScoresModel.h"
#include "ScoreFactory.h"
#include "SuccessFactorsModel.h"
#include "SuccessFactorFactory.h"
#include "UrbanModel.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ExerciseSettings.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/ResourceNetworkSelectionObserver.h"
#include "clients_gui/DrawerFactory.h"
#include "clients_gui/DrawerModel.h"
#include "indicators/GaugeTypes.h"
#include "tools/ExerciseConfig.h"
#include "tools/Loader_ABC.h"
#include "tools/SchemaWriter.h"
#include <tools/XmlCrc32Signature.h>
#include <urban/WorldParameters.h>
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace bfs = boost::filesystem;

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::Model( Controllers& controllers, const StaticModel& staticModel )
    : EntityResolverFacade( static_cast< Model_ABC& >( *this ) )
    , controllers_          ( controllers )
    , idManager_            ( *new IdManager() )
    , teamFactory_          ( *new TeamFactory( controllers, *this, staticModel, idManager_ ) )
    , knowledgeGroupFactory_( *new KnowledgeGroupFactory( controllers, staticModel, idManager_ ) )
    , formationFactory_     ( *new FormationFactory( controllers, staticModel, idManager_, symbolsFactory_ ) )
    , agentFactory_         ( *new AgentFactory( controllers, *this, staticModel, idManager_, knowledgeGroupFactory_ ) )
    , objectFactory_        ( *new ObjectFactory( controllers, *this, staticModel, idManager_ ) )
    , profileFactory_       ( *new ProfileFactory( controllers.controller_, *this ) )
    , scoreFactory_         ( *new ScoreFactory( controllers_, staticModel.indicators_, staticModel.gaugeTypes_, *this ) )
    , successFactorFactory_ ( *new SuccessFactorFactory( controllers_, *this, staticModel.successFactorActionTypes_ ) )
    , drawingFactory_       ( *new gui::DrawerFactory( controllers, staticModel.drawings_, staticModel.coordinateConverter_ ) )
    , ghostFactory_         ( *new GhostFactory( controllers, *this, staticModel, idManager_, knowledgeGroupFactory_, symbolsFactory_ ) )
    , resourceObserver_     ( *new ResourceNetworkSelectionObserver( controllers ) )
    , loaded_               ( false )
    , exercise_             ( *new Exercise( controllers.controller_ ) )
    , teams_                ( *new TeamsModel( controllers, teamFactory_ ) )
    , objects_              ( *new ObjectsModel( controllers, objectFactory_ ) )
    , knowledgeGroups_      ( *new KnowledgeGroupsModel( controllers, knowledgeGroupFactory_ ) ) // LTO
    , agents_               ( *new AgentsModel( controllers, agentFactory_, *this ) )
    , formations_           ( *new FormationModel( controllers, formationFactory_, agents_, staticModel ) )
    , limits_               ( *new LimitsModel( controllers, staticModel.coordinateConverter_, idManager_ ) )
    , weather_              ( *new WeatherModel( controllers.controller_, staticModel.coordinateConverter_ ) )
    , profiles_             ( *new ProfilesModel( controllers, profileFactory_ ) )
    , scores_               ( *new ScoresModel( scoreFactory_, teams_, staticModel.objectTypes_, staticModel.objectTypes_ ) )
    , successFactors_       ( *new SuccessFactorsModel( successFactorFactory_ ) )
    , urban_                ( *new UrbanModel( controllers, staticModel, objects_ ) )
    , drawings_             ( *new gui::DrawerModel( controllers, drawingFactory_, *this ) )
    , ghosts_               ( *new GhostModel( controllers, ghostFactory_ ) )
    , symbolsFactory_       ( *new SymbolFactory() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::~Model()
{
    // $$$$ ABR 2011-10-14: TODO: Delete everything ...
    delete &profiles_;
    delete &successFactors_;
    delete &successFactorFactory_;
    delete &scores_;
    delete &scoreFactory_;
    delete &profileFactory_;
    delete &resourceObserver_;
    delete &weather_;
    delete &limits_;
    delete &objects_;
    delete &objectFactory_;
    delete &agents_;
    delete &agentFactory_;
    delete &formations_;
    delete &formationFactory_;
    delete &symbolsFactory_;
    delete &knowledgeGroups_;
    delete &teams_;
    delete &teamFactory_;
    delete &urban_;
    delete &ghosts_;
    delete &ghostFactory_;
    delete &idManager_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetTeamResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< Team_ABC >& Model::GetTeamResolver() const
{
    return teams_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetFormationResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< Formation_ABC >& Model::GetFormationResolver() const
{
    return formations_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetAutomatResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< Automat_ABC >& Model::GetAutomatResolver() const
{
    return agents_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetAgentResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< Agent_ABC >& Model::GetAgentResolver() const
{
    return agents_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetKnowledgeGroupResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< KnowledgeGroup_ABC >& Model::GetKnowledgeGroupResolver() const
{
    return knowledgeGroups_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetObjectResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< Object_ABC >& Model::GetObjectResolver() const
{
    return objects_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetPopulationResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< Population_ABC >& Model::GetPopulationResolver() const
{
    return agents_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetInhabitantResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< Inhabitant_ABC >& Model::GetInhabitantResolver() const
{
    return agents_;
}

// -----------------------------------------------------------------------------
// Name: Model::FindUrbanObject
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Object_ABC* Model::FindUrbanObject( unsigned int id ) const
{
    return urban_.tools::Resolver< gui::TerrainObjectProxy >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: Model::GetUrbanObject
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Object_ABC& Model::GetUrbanObject( unsigned int id ) const
{
    return urban_.tools::Resolver< gui::TerrainObjectProxy >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: Model::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void Model::Purge()
{
    UpdateName( "orbat" );
    profiles_.Purge();
    urban_.Purge();
    successFactors_.Purge();
    scores_.Purge();
    weather_.Purge();
    limits_.Purge();
    ghosts_.Purge();
    agents_.Purge();
    formations_.Purge();
    knowledgeGroups_.Purge();
    teams_.Purge();
    objects_.Purge();
    exercise_.Purge();
    idManager_.Reset();
    SetLoaded( false );
}

namespace
{
    template< typename M >
    bool LoadOptional( const tools::Loader_ABC& fileLoader, const std::string& fileName, M& model, const tools::SchemaWriter_ABC& schemaWriter )
    {
        if( bfs::exists( fileName ) )
        {
            model.Load( fileLoader, fileName );
            return true;
        }
        model.Serialize( fileName, schemaWriter );
        tools::WriteXmlCrc32Signature( fileName );
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: Model::Load
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void Model::Load( const tools::ExerciseConfig& config, std::string& loadingErrors )
{
    config.GetLoader().LoadFile( config.GetExerciseFile(), boost::bind( &Exercise::Load, &exercise_, _1 ) );
    config.GetLoader().LoadFile( config.GetSettingsFile(), boost::bind( &ExerciseSettings::Load, &exercise_.GetSettings(), _1 ) );
    symbolsFactory_.Load( config );

    //$$ LOADING DE FICHIERS A UNIFIER
    const std::string directoryPath = boost::filesystem::path( config.GetTerrainFile() ).branch_path().native_file_string();
    const bfs::path urbanFile = bfs::path( directoryPath, bfs::native ) / "urban" / "urban.xml";
    if( bfs::exists( urbanFile ) )
    {
        config.GetLoader().CheckFile( urbanFile.string() );
        urban::WorldParameters world( directoryPath );
        urban_.Load( directoryPath, world, loadingErrors );
        const std::string urbanStateFile = config.GetUrbanStateFile() ;
        if( bfs::exists( bfs::path( urbanStateFile, bfs::native ) ) )
            config.GetLoader().LoadFile( urbanStateFile, boost::bind( &UrbanModel::LoadUrbanState, &urban_, _1 ) );
    }

    const std::string orbatFile = config.GetOrbatFile();
    if( bfs::exists( bfs::path( orbatFile, bfs::native ) ) )
    {
        UpdateName( orbatFile );
        config.GetLoader().LoadFile( orbatFile, boost::bind( &TeamsModel::Load, &teams_, _1, boost::ref( *this ), boost::ref( loadingErrors ) ) );
        objects_.Finalize();
    }

    const tools::SchemaWriter schemaWriter;
    if( ! LoadOptional( config.GetLoader(), config.GetWeatherFile(), weather_, schemaWriter ) )
        controllers_.controller_.Update( weather_);
    LoadOptional( config.GetLoader(), config.GetProfilesFile(), profiles_, schemaWriter );
    LoadOptional( config.GetLoader(), config.GetScoresFile(), scores_, schemaWriter );
    LoadOptional( config.GetLoader(), config.GetSuccessFactorsFile(), successFactors_, schemaWriter );
    SetLoaded( true );
}

namespace
{
    template< class T >
    void SerializeAndSign( const std::string& path, T& model, const tools::SchemaWriter_ABC& schemaWriter )
    {
        model.Serialize( path, schemaWriter );
        tools::WriteXmlCrc32Signature( path );
    }
}

// -----------------------------------------------------------------------------
// Name: Model::Save
// Created: SBO 2006-11-21
// -----------------------------------------------------------------------------
bool Model::Save( const tools::ExerciseConfig& config, ModelChecker_ABC& checker )
{
    if( !loaded_ )
        return false;
    const tools::SchemaWriter schemaWriter;

    const bool valid = teams_.CheckValidity( checker )
                    && agents_.CheckValidity( checker )
                    && scores_.CheckValidity( checker, schemaWriter )
                    && successFactors_.CheckValidity( checker, schemaWriter );
    if( valid )
    {
        exercise_.SerializeAndSign( config, schemaWriter );
        SerializeAndSign( config.GetSettingsFile(), exercise_.GetSettings(), schemaWriter );
        {
            xml::xofstream xos( config.GetOrbatFile() );
            xos << xml::start( "orbat" );
            schemaWriter.WriteExerciseSchema( xos, "orbat" );
            teams_.Serialize( xos );
            xos << xml::end;
        }
        tools::WriteXmlCrc32Signature( config.GetOrbatFile() );
        SerializeAndSign( config.GetUrbanStateFile(), urban_, schemaWriter );
        SerializeAndSign( config.GetWeatherFile(), weather_, schemaWriter );
        SerializeAndSign( config.GetProfilesFile(), profiles_, schemaWriter );
        SerializeAndSign( config.GetScoresFile(), scores_, schemaWriter );
        SerializeAndSign( config.GetSuccessFactorsFile(), successFactors_, schemaWriter );
        successFactors_.SerializeScript( config );
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
// Name: Model::GetIdManager
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
IdManager& Model::GetIdManager() const
{
    return idManager_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetSymbolsFactory
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
kernel::SymbolFactory& Model::GetSymbolsFactory() const
{
    return symbolsFactory_;
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
