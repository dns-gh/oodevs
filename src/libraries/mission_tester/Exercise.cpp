// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Exercise.h"
#include "tools/NullFileLoaderObserver.h"
#include "tools/ExerciseConfig.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Time_ABC.h"
#include "actions/ActionFactory.h"
#include "actions/ActionParameterFactory.h"

using namespace mission_tester;

namespace
{
    class SimulationTime : public kernel::Time_ABC
    {
        virtual QDateTime GetDateTime() const { return QDateTime(); }
        virtual QDateTime GetInitialDateTime() const { return QDateTime(); }
        virtual unsigned int GetTickDuration() const { return 0; }
    };
    class AgentKnowledgeConverter : public kernel::AgentKnowledgeConverter_ABC
    {
    public:
        virtual const kernel::AgentKnowledge_ABC* FindAgent( unsigned long /*id*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::PopulationKnowledge_ABC* FindPopulation( unsigned long /*id*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::AgentKnowledge_ABC* Find( const kernel::AgentKnowledge_ABC& /*base*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::AgentKnowledge_ABC* Find( const kernel::Agent_ABC& /*base*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::PopulationKnowledge_ABC* Find( const kernel::PopulationKnowledge_ABC& /*base*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::PopulationKnowledge_ABC* Find( const kernel::Population_ABC& /*base*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
    };
    class ObjectKnowledgeConverter : public kernel::ObjectKnowledgeConverter_ABC
    {
        virtual const kernel::ObjectKnowledge_ABC* Find( unsigned long /*id*/, const kernel::Team_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::ObjectKnowledge_ABC* Find( unsigned long /*id*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::ObjectKnowledge_ABC& /*base*/, const kernel::Team_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::Object_ABC& /*base*/, const kernel::Team_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::Object_ABC& /*base*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
    };
    class EntityResolver : public kernel::EntityResolver_ABC
    {
        virtual kernel::Agent_ABC* FindAgent( unsigned int /*id*/ ) const { return 0; }
        virtual kernel::Agent_ABC& GetAgent( unsigned int /*id*/ ) const { throw std::runtime_error( "not implemented" ); }
        virtual kernel::Automat_ABC* FindAutomat( unsigned int /*id*/ ) const { return 0; }
        virtual kernel::Automat_ABC& GetAutomat( unsigned int /*id*/ ) const { throw std::runtime_error( "not implemented" ); }
        virtual kernel::KnowledgeGroup_ABC* FindKnowledgeGroup( unsigned int /*id*/ ) const { return 0; }
        virtual kernel::KnowledgeGroup_ABC& GetKnowledgeGroup( unsigned int /*id*/ ) const { throw std::runtime_error( "not implemented" ); }
        virtual kernel::Object_ABC* FindObject( unsigned int /*id*/ ) const { return 0; }
        virtual kernel::Object_ABC& GetObject( unsigned int /*id*/ ) const { throw std::runtime_error( "not implemented" ); }
        virtual kernel::Object_ABC* FindUrbanObject( unsigned int /*id*/ ) const { return 0; }
        virtual kernel::Object_ABC& GetUrbanObject( unsigned int /*id*/ ) const { throw std::runtime_error( "not implemented" ); }
        virtual kernel::Population_ABC* FindPopulation( unsigned int /*id*/ ) const { return 0; }
        virtual kernel::Population_ABC& GetPopulation( unsigned int /*id*/ ) const { throw std::runtime_error( "not implemented" ); }
        virtual kernel::Formation_ABC* FindFormation( unsigned int /*id*/ ) const { return 0; }
        virtual kernel::Formation_ABC& GetFormation( unsigned int /*id*/ ) const { throw std::runtime_error( "not implemented" ); }
        virtual kernel::Team_ABC* FindTeam( unsigned int /*id*/ ) const { return 0; }
        virtual kernel::Team_ABC& GetTeam( unsigned int /*id*/ ) const { throw std::runtime_error( "not implemented" ); }
    };
}

// -----------------------------------------------------------------------------
// Name: Exercise constructor
// Created: PHC 2011-03-21
// -----------------------------------------------------------------------------
Exercise::Exercise( const std::string& exercise, const std::string& rootDir )
    : observer_                ( new tools::NullFileLoaderObserver() )
    , config_                  ( new tools::ExerciseConfig( *observer_ ) )
    , staticModel_             ( new kernel::StaticModel() )
    , controller_              ( new kernel::Controller )
    , entities_                ( new EntityResolver() )
    , time_                    ( new SimulationTime() )
    , agentKnowledgeConverter_ ( new AgentKnowledgeConverter() )
    , objectKnowledgeConverter_( new ObjectKnowledgeConverter() )
    , parameters_              ( new actions::ActionParameterFactory( staticModel_->coordinateConverter_, *entities_, *staticModel_, *agentKnowledgeConverter_, *objectKnowledgeConverter_, *controller_ ) )
    , factory_                 ( new actions::ActionFactory( *controller_, *parameters_, *entities_, *staticModel_, *time_ ) )
{
    char* commandLine[3];
    const std::string exerciseArgument = "--exercise=" + exercise;
    const std::string rootArgument = "--root-dir=" + rootDir;
    commandLine[0] = "app";
    commandLine[1] = const_cast< char* >( exerciseArgument.c_str() );
    commandLine[2] = const_cast< char* >( rootArgument.c_str() );
    config_->Parse( 3, commandLine );
    staticModel_->Load( *config_ );
}

// -----------------------------------------------------------------------------
// Name: Exercise destructor
// Created: PHC 2011-03-21
// -----------------------------------------------------------------------------
Exercise::~Exercise()
{
    // NOTHING
}
