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
#include "tools/IdManager.h"
#include "KnowledgeGroupFactory.h" // LTO
#include "LimitsModel.h"
#include "WeatherModel.h"
#include "ObjectFactory.h"
#include "ObjectsModel.h"
#include "PerformanceIndicator.h"
#include "ProfilesModel.h"
#include "ProfileFactory.h"
#include "ScoresModel.h"
#include "ScoreFactory.h"
#include "SuccessFactorsModel.h"
#include "SuccessFactorFactory.h"
#include "UndergroundAttribute.h"
#include "UrbanModel.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/ResourceNetworkSelectionObserver.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/DrawerFactory.h"
#include "clients_gui/DrawerModel.h"
#include "indicators/GaugeTypes.h"
#include "tools/ExerciseConfig.h"
#include "tools/ExerciseSettings.h"
#include "tools/Loader_ABC.h"
#include "tools/SchemaWriter.h"
#include <tools/XmlCrc32Signature.h>
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
Model::Model( Controllers& controllers, const ::StaticModel& staticModel )
    : EntityResolverFacade( static_cast< Model_ABC& >( *this ) )
    , staticModel_( staticModel )
    , config_( 0 )
    , idManager_( *new tools::IdManager() )
    , controllers_( controllers )
    , teamFactory_( *new TeamFactory( controllers, *this, staticModel, idManager_ ) )
    , knowledgeGroupFactory_( *new KnowledgeGroupFactory( controllers, staticModel, idManager_ ) )
    , formationFactory_( *new FormationFactory( controllers, staticModel, idManager_, symbolsFactory_ ) )
    , agentFactory_( *new AgentFactory( controllers, *this, staticModel, idManager_, knowledgeGroupFactory_, symbolsFactory_ ) )
    , objectFactory_( *new ObjectFactory( controllers, *this, staticModel, idManager_ ) )
    , profileFactory_( *new ProfileFactory( controllers.controller_, *this, staticModel.extensions_ ) )
    , scoreFactory_( *new ScoreFactory( controllers_, staticModel.indicators_, staticModel.gaugeTypes_, *this ) )
    , successFactorFactory_( *new SuccessFactorFactory( controllers_, *this, staticModel.successFactorActionTypes_ ) )
    , drawingFactory_( *new gui::DrawerFactory( controllers, staticModel.drawings_, staticModel.coordinateConverter_ ) )
    , ghostFactory_( *new GhostFactory( controllers, *this, staticModel, idManager_, knowledgeGroupFactory_, symbolsFactory_ ) )
    , resourceObserver_( *new ResourceNetworkSelectionObserver( controllers ) )
    , loaded_( false )
    , consistencyErrorsOnLoad_( false )
    , exercise_             ( *new Exercise( controllers.controller_ ) )
    , knowledgeGroups_      ( *new KnowledgeGroupsModel( controllers, knowledgeGroupFactory_ ) ) // LTO
    , teams_                ( *new TeamsModel( controllers, teamFactory_, knowledgeGroups_ ) )
    , objects_              ( *new ObjectsModel( controllers, objectFactory_, staticModel.objectTypes_ ) )
    , agents_( *new AgentsModel( controllers, agentFactory_, staticModel ) )
    , formations_( *new FormationModel( controllers, formationFactory_, agents_, staticModel ) )
    , limits_( *new LimitsModel( controllers, staticModel.coordinateConverter_, idManager_ ) )
    , weather_( *new WeatherModel( controllers.controller_, staticModel.coordinateConverter_ ) )
    , profiles_( *new ProfilesModel( controllers, profileFactory_ ) )
    , scores_( *new ScoresModel( scoreFactory_, teams_, staticModel.objectTypes_, staticModel.objectTypes_ ) )
    , successFactors_( *new SuccessFactorsModel( successFactorFactory_ ) )
    , urban_( *new UrbanModel( controllers, staticModel, objects_, idManager_ ) )
    , drawings_( *new gui::DrawerModel( controllers, drawingFactory_, *this ) )
    , ghosts_( *new GhostModel( controllers, ghostFactory_ ) )
    , symbolsFactory_( *new SymbolFactory() )
    , performanceIndicator_( *new PerformanceIndicator( *this ) )
    , width_                ( 0.f )
    , height_               ( 0.f )
    , needsSaving_          ( false )
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
    delete &drawings_;
    delete &drawingFactory_;
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
    delete &performanceIndicator_;
}


// -----------------------------------------------------------------------------
// Name: Model::NeedsSaving
// Created: JSR 2013-07-03
// -----------------------------------------------------------------------------
bool Model::NeedsSaving() const
{
    return needsSaving_ || !GetLoadingErrors().empty() || ghosts_.NeedSaving() || HasConsistencyErrorsOnLoad() || exercise_.NeedsSaving();
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
// Name: Model::GetUrbanObjectResolver
// Created: JSR 2012-05-16
// -----------------------------------------------------------------------------
tools::Resolver_ABC< kernel::UrbanObject_ABC >& Model::GetUrbanObjectResolver() const
{
    return urban_;
}

// -----------------------------------------------------------------------------
// Name: Model::ClearLoadingErrors
// Created: JSR 2012-01-05
// -----------------------------------------------------------------------------
void Model::ClearLoadingErrors()
{
    loadingErrors_.clear();
}

// -----------------------------------------------------------------------------
// Name: Model::ClearExternalErrors
// Created: MCO 2013-04-30
// -----------------------------------------------------------------------------
void Model::ClearExternalErrors()
{
    externalErrors_.clear();
}

// -----------------------------------------------------------------------------
// Name: Model::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void Model::Purge()
{
    ClearLoadingErrors();
    ClearExternalErrors();
    UpdateName( "orbat" );
    profiles_.Purge();
    urban_.Purge();
    successFactors_.Purge();
    scores_.Purge();
    weather_.Purge();
    drawings_.Purge();
    limits_.Purge();
    ghosts_.Purge();
    agents_.Purge();
    formations_.Purge();
    knowledgeGroups_.Purge();
    teams_.Purge();
    objects_.Purge();
    exercise_.Purge();
    idManager_.Reset();
    UndergroundAttribute::Purge();
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

namespace
{
    class DebugModel
    {
    public:
        void Serialize( const std::string& file, const tools::SchemaWriter_ABC& /* schemaWriter */ ) const
        {
            xml::xofstream xos( file );
            xos << xml::start( "debug" )
                    << xml::start( "dispatcher" )
                        << xml::attribute( "logfiles", 500 )
                        << xml::attribute( "logsize", 100000 )
                    << xml::end
                    << xml::start( "shield" )
                        << xml::attribute( "logfiles", 10 )
                        << xml::attribute( "logsize", 1000000 )
                    << xml::end
                    << xml::start( "dispatcherlog" )
                        << xml::attribute( "logfiles", 150 )
                        << xml::attribute( "logsize", 10000000 )
                    << xml::end
                    << xml::start( "sim" )
                        << xml::attribute( "logfiles", 150 )
                        << xml::attribute( "logsize", 10000000 )
                    << xml::end
                    << xml::start( "messages" )
                        << xml::attribute( "logfiles", 150 )
                        << xml::attribute( "logsize", 10000000 )
                    << xml::end
                << xml::end;
        }
        void Load( const tools::Loader_ABC&, const std::string& ) {}
    };
}

// -----------------------------------------------------------------------------
// Name: Model::Load
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void Model::Load( const tools::ExerciseConfig& config )
{
    needsSaving_ = false;
    config_ = &config;
    if( !config.IsTerrainSamePhysicalRef() )
        AppendLoadingError( eOthers, tools::translate( "Model", "Terrain's physical base does not match the one selected for the exercise. All urban materials, roofshapes, usages and infrastructures will be lost at next save." ).toAscii().constData(), 0 );

    width_ = config.GetTerrainWidth();
    height_ = config.GetTerrainHeight();
    config.GetLoader().LoadFile( config.GetExerciseFile(), boost::bind( &Exercise::Load, &exercise_, _1 ) );
    config.GetLoader().LoadFile( config.GetSettingsFile(), boost::bind( &tools::ExerciseSettings::Load, &exercise_.GetSettings(), _1 ) );
    config.GetLoader().LoadOptionalFile( config.GetUrbanFile(), boost::bind( &UrbanModel::LoadUrban, &urban_, _1 ) );
    config.GetLoader().LoadOptionalFile( config.GetUrbanStateFile(), boost::bind( &UrbanModel::LoadUrbanState, &urban_, _1 ) );
    symbolsFactory_.Load( config );
    urban_.Load();

    const std::string orbatFile = config.GetOrbatFile();
    if( bfs::exists( bfs::path( orbatFile, bfs::native ) ) )
    {
        UpdateName( orbatFile );
        config.GetLoader().LoadFile( orbatFile, boost::bind( &TeamsModel::Load, &teams_, _1, boost::ref( *this ) ) );
        objects_.Finalize();
    }

    const tools::SchemaWriter schemaWriter;
    if( !LoadOptional( config.GetLoader(), config.GetWeatherFile(), weather_, schemaWriter ) )
        controllers_.controller_.Update( weather_);
    LoadOptional( config.GetLoader(), config.GetProfilesFile(), profiles_, schemaWriter );
    LoadOptional( config.GetLoader(), config.GetScoresFile(), scores_, schemaWriter );
    LoadOptional( config.GetLoader(), config.GetSuccessFactorsFile(), successFactors_, schemaWriter );
    DebugModel debugModel;
    LoadOptional( config.GetLoader(), config.BuildExerciseChildFile( "debug.xml" ), debugModel, schemaWriter );

    performanceIndicator_.Load( config, tools::GeneralConfig::BuildResourceChildFile( "PerformanceIndicator.xml" ) );
    if( bfs::exists( bfs::path( orbatFile, bfs::native ) ) )
        ghosts_.Finalize( staticModel_ ); // $$$$ ABR 2012-06-25: Resolve logistic link and profiles for ghost ... frozen ICD

    UpdateCrowdKnowledgeGroups();

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
// Name: Model::SaveExercise
// Created: SBO 2006-11-21
// -----------------------------------------------------------------------------
void Model::SaveExercise( const tools::ExerciseConfig& config )
{
    if( !loaded_ )
        return;
    const tools::SchemaWriter schemaWriter;

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
    SerializeAndSign( config.GetDrawingsFile(), drawings_, schemaWriter );
    UpdateName( config.GetOrbatFile() );
}

// -----------------------------------------------------------------------------
// Name: Model::SaveTerrain
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void Model::SaveTerrain( const tools::ExerciseConfig& config, bool saveUrban /* = true */ )
{
    if( !loaded_ )
        return;
    const tools::SchemaWriter schemaWriter;
    if( saveUrban )
        SerializeAndSign( config.GetUrbanFile(), urban_, schemaWriter );
    config.SerializeAndSignTerrainFiles( schemaWriter );
}

// -----------------------------------------------------------------------------
// Name: Model::AppendLoadingError
// Created: JSR 2012-01-05
// -----------------------------------------------------------------------------
void Model::AppendLoadingError( E_ConsistencyCheck type, const std::string& message, kernel::Entity_ABC* entity /* = 0 */ )
{
    ModelConsistencyChecker::ConsistencyError error( type );
    error.items_.push_back( kernel::SafePointer< kernel::Entity_ABC >( controllers_, entity ) );
    error.optional_ = message;
    loadingErrors_.push_back( error );
}

// -----------------------------------------------------------------------------
// Name: Model::AppendExternalError
// Created: MCO 2013-04-30
// -----------------------------------------------------------------------------
void Model::AppendExternalError( E_ConsistencyCheck type, const std::string& message, bool isError, kernel::Entity_ABC* entity /*= 0*/ )
{
    ModelConsistencyChecker::ConsistencyError error( type );
    error.items_.push_back( kernel::SafePointer< kernel::Entity_ABC >( controllers_, entity ) );
    error.optional_ = message;
    error.isError_ = isError;
    externalErrors_.push_back( error );
}

// -----------------------------------------------------------------------------
// Name: Model::GetLoadingErrors
// Created: JSR 2012-01-05
// -----------------------------------------------------------------------------
const ModelConsistencyChecker::T_ConsistencyErrors& Model::GetLoadingErrors() const
{
    return loadingErrors_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetExternalErrors
// Created: MCO 2013-04-30
// -----------------------------------------------------------------------------
const ModelConsistencyChecker::T_ConsistencyErrors& Model::GetExternalErrors() const
{
    return externalErrors_;
}

// -----------------------------------------------------------------------------
// Name: Model::HasConsistencyErrorsOnLoad
// Created: LDC 2012-09-13
// -----------------------------------------------------------------------------
bool Model::HasConsistencyErrorsOnLoad() const
{
    return consistencyErrorsOnLoad_;
}

// -----------------------------------------------------------------------------
// Name: Model::SetConsistencyErrorsOnLoad
// Created: LDC 2012-09-13
// -----------------------------------------------------------------------------
void Model::SetConsistencyErrorsOnLoad()
{
    consistencyErrorsOnLoad_ = true;
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
tools::IdManager& Model::GetIdManager() const
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

// -----------------------------------------------------------------------------
// Name: Model::UpdateCrowdKnowledgeGroups
// Created: JSR 2013-07-03
// -----------------------------------------------------------------------------
void Model::UpdateCrowdKnowledgeGroups()
{
    auto it = teams_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Team_ABC& team = it.NextElement();
        auto itKg = knowledgeGroups_.CreateIterator();
        bool found = false;
        while( itKg.HasMoreElements() )
        {
            const kernel::KnowledgeGroup_ABC& kg = itKg.NextElement();
            const kernel::CommunicationHierarchies* hierarchies = kg.Retrieve< kernel::CommunicationHierarchies >();
            if( hierarchies && kg.IsCrowd() && &hierarchies->GetTop() == &team )
            {
                found = true;
                break;
            }
        }
        if( !found )
        {
            needsSaving_ = true;
            knowledgeGroups_.Create( const_cast< kernel::Team_ABC& >( team ), true );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Model::GetActionPlanning
// Created: LDC 2012-09-21
// -----------------------------------------------------------------------------
std::string Model::GetActionPlanning() const
{
    return config_ ? config_->BuildExerciseChildFile( exercise_.GetActionPlanning() ) : exercise_.GetActionPlanning(); 
}

// -----------------------------------------------------------------------------
// Name: Model::ReadPosition
// Created: ABR 2012-12-04
// -----------------------------------------------------------------------------
geometry::Point2f Model::ReadPosition( xml::xistream& xis, kernel::Entity_ABC* entity )
{
    const std::string position = xis.attribute< std::string >( "position" );
    geometry::Point2f result = staticModel_.coordinateConverter_.ConvertToXY( position );
    return ClipPosition( result, entity );
}

// -----------------------------------------------------------------------------
// Name: Model::ClipPosition
// Created: ABR 2012-12-05
// -----------------------------------------------------------------------------
geometry::Point2f Model::ClipPosition( const geometry::Point2f& position, kernel::Entity_ABC* entity )
{
    if( position.X() < 0.f || position.X() > width_ ||
        position.Y() < 0.f || position.Y() > height_ )
    {
        AppendLoadingError( eUnitOutsideMap, ( entity ) ? entity->GetName().toStdString() : "", entity );
        return geometry::Point2f( 0.f, 0.f );
    }
    return position;
}
