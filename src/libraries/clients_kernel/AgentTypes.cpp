// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AgentTypes.h"

#include "AgentType.h"
#include "AutomatType.h"
#include "ComponentType.h"
#include "DecisionalModel.h"
#include "DotationType.h"
#include "FragOrderType.h"
#include "InhabitantType.h"
#include "KnowledgeGroupType.h"
#include "MagicActionType.h"
#include "MissionFactory.h"
#include "MissionType.h"
#include "PopulationType.h"
#include "SensorType.h"
#include "SymbolFactory.h"
#include "XmlTranslations.h"
#include "MT_Tools/MT_Logger.h"

#include "protocol/Helpers.h"
#include "protocol/Protocol.h"
#include "tools/ExerciseConfig.h"
#include "tools/Language.h"
#include "tools/PhyLoader.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace protocol;

// -----------------------------------------------------------------------------
// Name: AgentTypes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentTypes::AgentTypes()
    : agentTypesTranslations_( new kernel::XmlTranslations() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentTypes constructor
// Created: AGE 2008-03-14
// -----------------------------------------------------------------------------
AgentTypes::AgentTypes( const tools::ExerciseConfig& config )
{
    Load( config );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::Load
// Created: JSR 2010-11-25
// -----------------------------------------------------------------------------
void AgentTypes::Load( const tools::ExerciseConfig& config )
{
    Purge();
    symbolFactory_.reset( new SymbolFactory() );
    symbolFactory_->Load( config );
    const tools::PhyLoader& loader = config.GetPhyLoader();
    const tools::Path localesDirectory = config.GetPhysicalChildPath( "locales-directory" );
    agentTypesTranslations_->LoadTranslationFile( config.GetOptionalPhysicalChildFile( "units" ), localesDirectory, tools::Language::Current() );
    loader.LoadPhysicalFile( "components", boost::bind( &AgentTypes::ReadComponents, this, _1 ) );
    loader.LoadPhysicalFile( "missions", boost::bind( &AgentTypes::ReadOrderTypes, this, _1 ) );
    //loader.LoadPhysicalFile( "magic-orders", boost::bind( &AgentTypes::ReadMagicOrderTypes, this, _1 ) );
    loader.LoadPhysicalFile( "models", boost::bind( &AgentTypes::ReadModels, this, _1 ) );
    loader.LoadPhysicalFile( "sensors", boost::bind( &AgentTypes::ReadSensors, this, _1 ) );
    loader.LoadPhysicalFile( "units", boost::bind( &AgentTypes::ReadAgents, this, _1 ) );
    loader.LoadPhysicalFile( "automats", boost::bind( &AgentTypes::ReadAutomats, this, _1 ) );
    loader.LoadPhysicalFile( "populations", boost::bind( &AgentTypes::ReadPopulations, this, _1 ) );
    loader.LoadPhysicalFile( "inhabitants", boost::bind( &AgentTypes::ReadInhabitants, this, _1 ) );
    loader.LoadPhysicalFile( "knowledge-groups", boost::bind( &AgentTypes::ReadKnowledgeGroups, this, _1 ) );;
    CreateMagicActionTypes();
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::Purge
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void AgentTypes::Purge()
{
    unitModels_.DeleteAll();
    automatModels_.DeleteAll();
    populationModels_.DeleteAll();
    missions_[ eMissionType_Pawn ].DeleteAll();
    missions_[ eMissionType_Automat ].DeleteAll();
    missions_[ eMissionType_Population ].DeleteAll();
    tools::Resolver< MissionType >::Clear();
    tools::Resolver< FragOrderType >::DeleteAll();
    tools::Resolver< FragOrderType, std::string >::Clear();
    tools::Resolver< KnowledgeGroupType, std::string >::DeleteAll();
    tools::Resolver< PopulationType >::DeleteAll();
    tools::Resolver< PopulationType, std::string >::Clear();
    tools::Resolver< InhabitantType >::DeleteAll();
    tools::Resolver< InhabitantType, std::string >::Clear();
    tools::Resolver< AgentType >::DeleteAll();
    tools::Resolver< AgentType, std::string >::Clear();
    tools::Resolver< AutomatType >::DeleteAll();
    tools::Resolver< AutomatType, std::string >::Clear();
    tools::Resolver< ComponentType >::DeleteAll();
    tools::Resolver< ComponentType, std::string >::Clear();
    tools::Resolver< SensorType, std::string >::DeleteAll();
    tools::Resolver< MagicActionType, std::string >::DeleteAll();
    agentTypesTranslations_->Purge();
}

// -----------------------------------------------------------------------------
// Name: AgentTypes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentTypes::~AgentTypes()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadComponents
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadComponents( xml::xistream& xis )
{
    xis >> xml::start( "equipments" )
            >> xml::list( "equipment", *this, &AgentTypes::ReadComponent );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadComponent
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadComponent( xml::xistream& xis )
{
    ComponentType* component = new ComponentType( xis );
    tools::Resolver< ComponentType, std::string >::Register( component->GetName(), *component );
    tools::Resolver< ComponentType >::Register( component->GetId(), *component );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadOrderTypes
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentTypes::ReadOrderTypes( xml::xistream& xis )
{
    xis >> xml::start( "missions" );
    ReadMissions( xis, "units"      , eMissionType_Pawn );
    ReadMissions( xis, "automats"   , eMissionType_Automat );
    ReadMissions( xis, "populations", eMissionType_Population );
    xis     >> xml::start( "fragorders" )
                >> xml::list( "fragorder", *this, &AgentTypes::ReadFragOrderType )
            >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadMissions
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void AgentTypes::ReadMissions( xml::xistream& xis, const std::string& name, E_MissionType type )
{
    xis >> xml::start( name );
    xis     >> xml::list( "mission", *this, &AgentTypes::ReadMissionType, type )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadMissionType
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentTypes::ReadMissionType( xml::xistream& xis, E_MissionType type )
{
    assert( type == eMissionType_Pawn || type == eMissionType_Automat || type == eMissionType_Population );
    MissionType* mission = new MissionType( xis, type );
    tools::Resolver< MissionType >::Register( mission->GetId(), *mission );
    missions_[ type ].Register( mission->GetName(), *mission );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadFragOrderType
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentTypes::ReadFragOrderType( xml::xistream& xis )
{
    FragOrderType* order = new FragOrderType( xis );
    tools::Resolver< FragOrderType >::Register( order->GetId(), *order );
    tools::Resolver< FragOrderType, std::string >::Register( order->GetName(), *order );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadModels
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadModels( xml::xistream& xis )
{
    T_Resolver unitResolver       = &MissionFactory::CreateAgentMission;
    T_Resolver automatResolver    = &MissionFactory::CreateAutomatMission;
    T_Resolver populationResolver = &MissionFactory::CreatePopulationMission;

    xis >> xml::start( "models" )
            >> xml::start( "units" )
                >> xml::list( "unit", *this, &AgentTypes::ReadModel, unitResolver, unitModels_ )
            >> xml::end
            >> xml::start( "automats" )
                >> xml::list( "automat", *this, &AgentTypes::ReadModel, automatResolver, automatModels_ )
            >> xml::end
            >> xml::start( "crowd" )
                >> xml::list( "crowd", *this, &AgentTypes::ReadModel, populationResolver, populationModels_ )
            >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadModel
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadModel( xml::xistream& xis, const T_Resolver& missionResolver, tools::Resolver< DecisionalModel, std::string >& models )
{
    MissionFactory factory( missions_[ eMissionType_Pawn ], missions_[ eMissionType_Automat ], missions_[ eMissionType_Population ], *this );
    DecisionalModel* model = new DecisionalModel( xis, factory, missionResolver, *this );
    models.Register( model->GetName(), *model );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadSensors
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadSensors( xml::xistream& xis )
{
    xis >> xml::start( "sensors" )
            >> xml::start( "sensors" )
                >> xml::list( "sensor", *this, &AgentTypes::ReadSensor );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadSensor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadSensor( xml::xistream& xis )
{
    std::string name;
    xis >> xml::attribute( "name", name )
        >> xml::list( "unit-detection", *this, &AgentTypes::ReallyReadSensor, name );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReallyReadSensor
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void AgentTypes::ReallyReadSensor( xml::xistream& xis, const std::string& sensor )
{
    SensorType* type = new SensorType( sensor, xis );
    tools::Resolver< SensorType, std::string >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAgents
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAgents( xml::xistream& xis )
{
    xis >> xml::start( "units" )
            >> xml::list( "unit", *this, &AgentTypes::ReadAgentType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAgentType
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAgentType( xml::xistream& xis )
{
    try
    {
        AgentType* type = new AgentType( xis, *this, unitModels_, *symbolFactory_, *agentTypesTranslations_ );
        tools::Resolver< AgentType >::Register( type->GetId(), *type );
        tools::Resolver< AgentType, std::string >::Register( type->GetKeyName(), *type );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAutomats
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAutomats( xml::xistream& xis )
{
    xis >> xml::start( "automats" )
            >> xml::list( "automat", *this, &AgentTypes::ReadAutomatType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAutomatType
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAutomatType( xml::xistream& xis )
{
    try
    {
        AutomatType* type = new AutomatType( xis, *this, automatModels_, *symbolFactory_ );
        tools::Resolver< AutomatType >         ::Register( type->GetId(), *type );
        tools::Resolver< AutomatType, std::string >::Register( type->GetName(), *type );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadPopulations
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
void AgentTypes::ReadPopulations( xml::xistream& xis )
{
    xis >> xml::start( "populations" )
            >> xml::list( "population", *this, &AgentTypes::ReadPopulationType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadPopulationType
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
void AgentTypes::ReadPopulationType( xml::xistream& xis )
{
    PopulationType* type = new PopulationType( xis, populationModels_ );
    tools::Resolver< PopulationType >::Register( type->GetId(), *type );
    tools::Resolver< PopulationType, std::string >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadInhabitants
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
void AgentTypes::ReadInhabitants( xml::xistream& xis )
{
    xis >> xml::start( "populations" )
        >> xml::list( "population", *this, &AgentTypes::ReadInhabitantType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadInhabitantType
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
void AgentTypes::ReadInhabitantType( xml::xistream& xis )
{
    InhabitantType* type = new InhabitantType( xis, *this );
    tools::Resolver< InhabitantType >::Register( type->GetId(), *type );
    tools::Resolver< InhabitantType, std::string >::Register( type->GetName(), *type );
}
// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadKnowledgeGroups
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void AgentTypes::ReadKnowledgeGroups( xml::xistream& xis )
{
    xis >> xml::start( "knowledge-groups" )
            >> xml::list( "knowledge-group", *this, &AgentTypes::ReadKnowledgeGroupType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadKnowledgeGroupType
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void AgentTypes::ReadKnowledgeGroupType( xml::xistream& xis )
{
    KnowledgeGroupType* type = new KnowledgeGroupType( xis );
    tools::Resolver< KnowledgeGroupType, std::string >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::RegisterActionType
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
void AgentTypes::RegisterActionType( MagicActionType& actionType )
{
    tools::Resolver< MagicActionType, std::string >::Register( actionType.GetName(), actionType );
}

namespace
{
    typedef void (AgentTypes::*T_Register)( MagicActionType& );

    template< typename T >
    void RegisterMagicActions( const T_Register& operand, AgentTypes* types )
    {
        const auto& tab = typename T::data_;
        for( size_t i = 0; i < typename T::size_; ++i )
            ( types->*operand )( *new MagicActionType( tab[i].name, tab[i].type ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::CreateMagicActionTypes
// Created: JSR 2010-04-08
// -----------------------------------------------------------------------------
void AgentTypes::CreateMagicActionTypes()
{
    RegisterMagicActions< mapping::MagicUnitAction >( &AgentTypes::RegisterActionType, this );
    RegisterMagicActions< mapping::MagicKnowledgeAction >( &AgentTypes::RegisterActionType, this );
    RegisterMagicActions< mapping::MagicObjectAction >( &AgentTypes::RegisterActionType, this );
    RegisterMagicActions< mapping::MagicAction >( &AgentTypes::RegisterActionType, this );
    RegisterActionType( *new MagicActionType( "change_mode" ) );
}
