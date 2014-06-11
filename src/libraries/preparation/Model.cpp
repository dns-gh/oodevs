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
#include "UrbanModel.h"
#include "clients_gui/DrawerFactory.h"
#include "clients_gui/DrawerModel.h"
#include "clients_gui/ResourceNetworkSelectionObserver.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/Tools.h"
#include "indicators/GaugeTypes.h"
#include "tools/ExerciseConfig.h"
#include "tools/ExerciseSettings.h"
#include "tools/Loader_ABC.h"
#include "tools/SchemaWriter.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::Model( Controllers& controllers, const ::StaticModel& staticModel )
    : EntityResolverFacade( static_cast< Model_ABC& >( *this ) )
    , controllers_          ( controllers )
    , staticModel_          ( staticModel )
    , idManager_            ( new tools::IdManager( true ) )
    , teamFactory_          ( new TeamFactory( controllers, *this, staticModel, *idManager_ ) )
    , knowledgeGroupFactory_( new KnowledgeGroupFactory( controllers, staticModel, *idManager_ ) )
    , symbolsFactory_       ( new SymbolFactory() )
    , formationFactory_     ( new FormationFactory( controllers, staticModel, *idManager_, *symbolsFactory_ ) )
    , agentFactory_         ( new AgentFactory( controllers, *this, staticModel, *idManager_, *symbolsFactory_ ) )
    , objectFactory_        ( new ObjectFactory( controllers, *this, staticModel, *idManager_ ) )
    , profileFactory_       ( new ProfileFactory( controllers.controller_, *this ) )
    , scoreFactory_         ( new ScoreFactory( controllers, staticModel.indicators_, staticModel.gaugeTypes_, *this ) )
    , successFactorFactory_ ( new SuccessFactorFactory( controllers, *this, staticModel.successFactorActionTypes_ ) )
    , drawingFactory_       ( new gui::DrawerFactory( controllers, staticModel.drawings_, staticModel.coordinateConverter_ ) )
    , ghostFactory_         ( new GhostFactory( controllers, *this, staticModel, *idManager_, *symbolsFactory_ ) )
    , resourceObserver_     ( new gui::ResourceNetworkSelectionObserver( controllers ) )
    , loaded_               ( false )
    , consistencyErrorsOnLoad_( false )
    , oldUrbanMode_         ( false )
    , exercise_             ( new Exercise( controllers.controller_ ) )
    , knowledgeGroups_      ( new KnowledgeGroupsModel( controllers, *knowledgeGroupFactory_ ) ) // LTO
    , teams_                ( new TeamsModel( controllers, *teamFactory_, *knowledgeGroups_ ) )
    , objects_              ( new ObjectsModel( controllers, *objectFactory_, staticModel.objectTypes_ ) )
    , agents_               ( new AgentsModel( controllers, *agentFactory_, staticModel ) )
    , formations_           ( new FormationModel( controllers, *formationFactory_, *agents_, staticModel ) )
    , limits_               ( new LimitsModel( controllers, staticModel.coordinateConverter_, *idManager_ ) )
    , weather_              ( new WeatherModel( controllers.controller_, staticModel.coordinateConverter_ ) )
    , profiles_             ( new ProfilesModel( controllers, *profileFactory_ ) )
    , scores_               ( new ScoresModel( *scoreFactory_, *teams_, staticModel.objectTypes_, staticModel.objectTypes_ ) )
    , successFactors_       ( new SuccessFactorsModel( *successFactorFactory_ ) )
    , urban_                ( new UrbanModel( controllers, staticModel, *objects_, *idManager_ ) )
    , drawings_             ( new gui::DrawerModel( controllers, *drawingFactory_, *this ) )
    , ghosts_               ( new GhostModel( controllers, *ghostFactory_ ) )
    , performanceIndicator_ ( new PerformanceIndicator( *this ) )
    , width_                ( 0.f )
    , height_               ( 0.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::~Model()
{
    // crash on egypt if we rely on destruction order
    // there is an hidden dependency somewhere which we must make explicit
    agents_.reset();
}

// -----------------------------------------------------------------------------
// Name: Model::GetTeamResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< Team_ABC >& Model::GetTeamResolver() const
{
    return *teams_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetFormationResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< Formation_ABC >& Model::GetFormationResolver() const
{
    return *formations_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetAutomatResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< Automat_ABC >& Model::GetAutomatResolver() const
{
    return *agents_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetAgentResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< Agent_ABC >& Model::GetAgentResolver() const
{
    return *agents_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetGhostResolver
// Created: JSR 2014-06-04
// -----------------------------------------------------------------------------
tools::Resolver_ABC< kernel::Ghost_ABC >& Model::GetGhostResolver() const
{
    return *ghosts_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetKnowledgeGroupResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< KnowledgeGroup_ABC >& Model::GetKnowledgeGroupResolver() const
{
    return *knowledgeGroups_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetObjectResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< Object_ABC >& Model::GetObjectResolver() const
{
    return *objects_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetPopulationResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< Population_ABC >& Model::GetPopulationResolver() const
{
    return *agents_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetInhabitantResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< Inhabitant_ABC >& Model::GetInhabitantResolver() const
{
    return *agents_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetUrbanObjectResolver
// Created: JSR 2012-05-16
// -----------------------------------------------------------------------------
tools::Resolver_ABC< kernel::UrbanObject_ABC >& Model::GetUrbanObjectResolver() const
{
    return *urban_;
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
// Name: Model::OldUrbanMode
// Created: JSR 2012-09-05
// -----------------------------------------------------------------------------
bool Model::OldUrbanMode() const
{
    return oldUrbanMode_;
}

// -----------------------------------------------------------------------------
// Name: Model::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void Model::Purge()
{
    ClearLoadingErrors();
    UpdateName( "orbat" );
    profiles_->Purge();
    urban_->Purge();
    successFactors_->Purge();
    scores_->Purge();
    weather_->Purge();
    limits_->Purge();
    ghosts_->Purge();
    drawings_->Purge();
    agents_->Purge();
    formations_->Purge();
    knowledgeGroups_->Purge();
    teams_->Purge();
    objects_->Purge();
    exercise_->Purge();
    idManager_->Reset();
    SetLoaded( false );
}

namespace
{
    template< typename M >
    bool LoadOptional( const tools::Loader_ABC& fileLoader, const tools::Path& fileName, M& model, const tools::SchemaWriter_ABC& schemaWriter )
    {
        if( fileName.Exists() )
        {
            model->Load( fileLoader, fileName );
            return true;
        }
        model->Serialize( fileName, schemaWriter );
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: Model::Load
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void Model::Load( const tools::ExerciseConfig& config )
{
    width_ = config.GetTerrainWidth();
    height_ = config.GetTerrainHeight();
    config.GetLoader().LoadFile( config.GetExerciseFile(), boost::bind( &Exercise::Load, exercise_.get(), _1 ) );
    config.GetLoader().LoadFile( config.GetSettingsFile(), boost::bind( &tools::ExerciseSettings::Load, &exercise_->GetSettings(), _1 ) );
    oldUrbanMode_ = false;
    if( !config.GetLoader().LoadOptionalFile( config.GetUrbanFile(), boost::bind( &UrbanModel::LoadUrban, urban_.get(), _1 ) ) )
        if( config.GetLoader().LoadOptionalFile( config.GetTerrainUrbanFile(), boost::bind( &UrbanModel::LoadUrban, urban_.get(), _1 ) ) )
        {
            if( !config.IsTerrainSamePhysicalRef() )
                AppendLoadingError( eOthers, tools::translate( "Model", "Terrain's physical base does not match the one selected for the exercise. All urban materials, roofshapes, usages and infrastructures will be lost at next save." ).toAscii().constData() );
            oldUrbanMode_ = true;
            config.GetLoader().LoadOptionalFile( config.GetUrbanStateFile(), boost::bind( &UrbanModel::LoadUrbanState, urban_.get(), _1 ) );
        }
    urban_->CreateGeostoreManager( config );
    symbolsFactory_->Load( config );
    urban_->Load();

    const tools::Path orbatFile = config.GetOrbatFile();
    if( orbatFile.Exists() )
    {
        UpdateName( orbatFile );
        config.GetLoader().LoadFile( orbatFile, boost::bind( &TeamsModel::Load, teams_.get(), _1, boost::ref( *this ) ) );
        objects_->Finalize();
    }

    const tools::SchemaWriter schemaWriter;
    if( !LoadOptional( config.GetLoader(), config.GetWeatherFile(), weather_, schemaWriter ) )
        controllers_.controller_.Update( *weather_ );
    LoadOptional( config.GetLoader(), config.GetProfilesFile(), profiles_, schemaWriter );
    LoadOptional( config.GetLoader(), config.GetScoresFile(), scores_, schemaWriter );
    LoadOptional( config.GetLoader(), config.GetSuccessFactorsFile(), successFactors_, schemaWriter );
    LoadOptional( config.GetLoader(), config.GetDrawingsFile(), drawings_, schemaWriter );

    performanceIndicator_->Load( config, tools::GeneralConfig::BuildResourceChildFile( "PerformanceIndicator.xml" ) );
    if( orbatFile.Exists() )
        ghosts_->Finalize( staticModel_ ); // $$$$ ABR 2012-06-25: Resolve logistic link and profiles for ghost ... frozen ICD

    if( urban_->ManageIdConflicts() )
        AppendLoadingError( eOthers, tools::translate( "Model", "Urban blocks ids have been changed on loading because they were in conflict with other entities ids." ).toAscii().constData() );
    idManager_->SetKeepIds( false );

    SetLoaded( true );
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
    exercise_->Serialize( config, schemaWriter );
    exercise_->GetSettings().Serialize( config.GetSettingsFile(), schemaWriter );
    {
        tools::Xofstream xos( config.GetOrbatFile() );
        xos << xml::start( "orbat" );
        schemaWriter.WriteExerciseSchema( xos, "orbat" );
        teams_->Serialize( xos );
        xos << xml::end;
    }
    if( urban_->Count() > 0 )
        urban_->Serialize( config.GetUrbanFile(), schemaWriter );
    drawings_->Serialize( config.GetDrawingsFile(), schemaWriter );
    weather_->Serialize( config.GetWeatherFile(), schemaWriter );
    profiles_->Serialize( config.GetProfilesFile(), schemaWriter );
    scores_->Serialize( config.GetScoresFile(), schemaWriter );
    successFactors_->Serialize( config.GetSuccessFactorsFile(), schemaWriter );
    successFactors_->SerializeScript( config );
    UpdateName( config.GetOrbatFile() );
}

// -----------------------------------------------------------------------------
// Name: Model::AppendLoadingError
// Created: JSR 2012-01-05
// -----------------------------------------------------------------------------
void Model::AppendLoadingError( E_ConsistencyCheck type, const std::string& errorMsg, kernel::Entity_ABC* entity /* = 0 */ )
{
    ModelConsistencyChecker::ConsistencyError error( type );
    kernel::SafePointer< kernel::Entity_ABC >* safePtr = new kernel::SafePointer< kernel::Entity_ABC >( controllers_, entity );
    error.items_.push_back( safePtr );
    error.optional_ = errorMsg;
    loadingErrors_.push_back( error );
}

// -----------------------------------------------------------------------------
// Name: Model::SetExerciseValidity
// Created: JSR 2012-01-09
// -----------------------------------------------------------------------------
void Model::SetExerciseValidity( bool valid )
{
    exercise_->SetExerciseValidity( valid );
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
    return *idManager_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetSymbolsFactory
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
kernel::SymbolFactory& Model::GetSymbolsFactory() const
{
    return *symbolsFactory_;
}

// -----------------------------------------------------------------------------
// Name: Model::UpdateName
// Created: SBO 2006-11-21
// -----------------------------------------------------------------------------
void Model::UpdateName( const tools::Path& orbat )
{
    name_ = ( orbat.IsEmpty() ) ? "" : orbat.BaseName().ToUTF8().c_str();
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
