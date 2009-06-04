// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MissionInterfaceBuilder.h"
#include "actions_gui/MissionInterface_ABC.h"
#include "actions_gui/ParamAgent.h"
#include "actions_gui/ParamAgentList.h"
#include "actions_gui/ParamAtlasNature.h"
#include "actions_gui/ParamAutomat.h"
#include "actions_gui/ParamAutomatList.h"
#include "actions_gui/ParamDateTime.h"
#include "actions_gui/ParamPath.h"
#include "actions_gui/ParamPathList.h"
#include "actions_gui/ParamPoint.h"
#include "actions_gui/ParamPolygon.h"
#include "actions_gui/ParamBool.h"
#include "actions_gui/ParamLocation.h"
#include "actions_gui/ParamLocationList.h"
#include "actions_gui/ParamPointList.h"
#include "actions_gui/ParamPolygonList.h"
#include "actions_gui/ParamAgentKnowledge.h"
#include "actions_gui/ParamAgentKnowledgeList.h"
#include "actions_gui/ParamPopulationKnowledge.h"
#include "actions_gui/ParamObjectKnowledge.h"
#include "actions_gui/ParamObjectKnowledgeList.h"
#include "actions_gui/ParamObstacle.h"
#include "actions_gui/ParamObstacleList.h"
#include "actions_gui/ParamDirection.h"
#include "actions_gui/ParamNumericField.h"
#include "actions_gui/ParamEquipmentList.h"
#include "actions_gui/ParamHumanWoundList.h"
#include "actions_gui/ParamDotationDType.h"
#include "actions_gui/LimitParameter.h"
#include "actions_gui/ParamLimaList.h"
#include "actions_gui/ParamMissionObjective.h"
#include "actions_gui/ParamMissionObjectiveList.h"
#include "actions_gui/ParamDotationTypeList.h"
#include "actions_gui/ParamIntelligenceList.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/OrderParameterValue.h"
#include "gaming/Simulation.h"
#include "gaming/StaticModel.h"
#include "gaming/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder constructor
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
MissionInterfaceBuilder::MissionInterfaceBuilder( Controllers& controllers, gui::ParametersLayer& layer
                                                , AgentKnowledgeConverter_ABC& knowledgeConverter, ObjectKnowledgeConverter_ABC& objectKnowledgeConverter
                                                , const StaticModel& staticModel, const Simulation& simulation )
    : controllers_             ( controllers )
    , layer_                   ( layer )
    , converter_               ( staticModel.coordinateConverter_ )
    , knowledgeConverter_      ( knowledgeConverter )
    , objectKnowledgeConverter_( objectKnowledgeConverter )
    , staticModel_             ( staticModel )
    , simulation_              ( simulation )
    , missionInterface_        ( 0 )
    , entity_                  ( 0 )
{
    builderFunctors_["agent"]               = &MissionInterfaceBuilder::BuildAgent;
    builderFunctors_["agentlist"]           = &MissionInterfaceBuilder::BuildAgentList;
    builderFunctors_["automate"]            = &MissionInterfaceBuilder::BuildAutomat;
    builderFunctors_["automatelist"]        = &MissionInterfaceBuilder::BuildAutomatList;
    builderFunctors_["genobject"]           = &MissionInterfaceBuilder::BuildGenObject;
    builderFunctors_["genobjectlist"]       = &MissionInterfaceBuilder::BuildGenObjectList;
    builderFunctors_["objective"]           = &MissionInterfaceBuilder::BuildMissionObjective;
    builderFunctors_["objectivelist"]       = &MissionInterfaceBuilder::BuildMissionObjectiveList;
    builderFunctors_["dotationtype"]        = &MissionInterfaceBuilder::BuildDotation;
    builderFunctors_["natureatlas"]         = &MissionInterfaceBuilder::BuildAtlasNature;

    builderFunctors_["bool"]                = &MissionInterfaceBuilder::BuildBoolean;
    builderFunctors_["direction"]           = &MissionInterfaceBuilder::BuildDirection;
    builderFunctors_["numeric"]             = &MissionInterfaceBuilder::BuildNumeric;
    builderFunctors_["datetime"]            = &MissionInterfaceBuilder::BuildParamDateTime;

    builderFunctors_["agentknowledge"]      = &MissionInterfaceBuilder::BuildAgentKnowledge;
    builderFunctors_["agentknowledgelist"]  = &MissionInterfaceBuilder::BuildAgentKnowledgeList;
    builderFunctors_["objectknowledge"]     = &MissionInterfaceBuilder::BuildObjectKnowledge;
    builderFunctors_["objectknowledgelist"] = &MissionInterfaceBuilder::BuildObjectKnowledgeList;
    builderFunctors_["populationknowledge"] = &MissionInterfaceBuilder::BuildPopulationKnowledge;

    builderFunctors_["path"]                = &MissionInterfaceBuilder::BuildPath;
    builderFunctors_["pathlist"]            = &MissionInterfaceBuilder::BuildPathList;
    builderFunctors_["point"]               = &MissionInterfaceBuilder::BuildPoint;
    builderFunctors_["pointlist"]           = &MissionInterfaceBuilder::BuildPointList;
    builderFunctors_["polygon"]             = &MissionInterfaceBuilder::BuildPolygon;
    builderFunctors_["polygonlist"]         = &MissionInterfaceBuilder::BuildPolygonList;
    builderFunctors_["location"]            = &MissionInterfaceBuilder::BuildLocation;
    builderFunctors_["locationlist"]        = &MissionInterfaceBuilder::BuildLocationList;

    builderFunctors_["maintenancepriorities"] = &MissionInterfaceBuilder::BuildMaintenancePriorities;
    builderFunctors_["medicalpriorities"]     = &MissionInterfaceBuilder::BuildMedicalPriorities;
    builderFunctors_["enumeration"]           = &MissionInterfaceBuilder::BuildEnumeration;

    builderFunctors_["limit"]                 = &MissionInterfaceBuilder::BuildLimit;
    builderFunctors_["phaselinelist"]         = &MissionInterfaceBuilder::BuildLimaList;

    builderFunctors_["dotationlist"]          = &MissionInterfaceBuilder::BuildDotationTypeList;
    builderFunctors_["intelligencelist"]      = &MissionInterfaceBuilder::BuildIntelligenceList;
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder destructor
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
MissionInterfaceBuilder::~MissionInterfaceBuilder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::Build
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void MissionInterfaceBuilder::Build( actions::gui::MissionInterface_ABC& missionInterface, Entity_ABC& entity, const OrderType& order )
{
    missionInterface_ = &missionInterface;
    entity_ = &entity;
    Iterator< const OrderParameter& > it = order.CreateIterator();
    while( it.HasMoreElements() )
        Build( it.NextElement() );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::Build
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void MissionInterfaceBuilder::Build( const OrderParameter& parameter )
{
    CIT_BuilderFunctors it = builderFunctors_.find( QString( parameter.GetType().c_str() ).lower() );
    if( it != builderFunctors_.end() )
    {
        T_BuilderFunctor functor = it->second;
        if( actions::gui::Param_ABC* param = (this->*functor)( parameter ) )
            missionInterface_->AddParameter( *param );
    }
    else
        throw std::runtime_error( tools::translate( "MissionInterfaceBuilder", "Unknown mission parameter: %1" ).arg( parameter.GetType().c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAgent
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildAgent( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamAgent( missionInterface_, parameter, controllers_.controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAgentList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildAgentList( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamAgentList( missionInterface_, parameter, controllers_.actions_, controllers_.controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAutomat
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildAutomat( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamAutomat( missionInterface_, parameter, controllers_.controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAutomatList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildAutomatList( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamAutomatList( missionInterface_, parameter, controllers_.actions_, controllers_.controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAtlasNature
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildAtlasNature( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamAtlasNature( missionInterface_, parameter, staticModel_.atlasNatures_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildDotation
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildDotation( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamDotationDType( parameter, staticModel_.objectTypes_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildBoolean
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildBoolean( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamBool( missionInterface_, parameter ); // $$$$ SBO 2007-03-14: default value?
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildDirection
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildDirection( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamDirection( missionInterface_, parameter );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildNumeric
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildNumeric( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamNumericField( parameter, true );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildParamDateTime
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildParamDateTime( const kernel::OrderParameter& parameter ) const
{
    return new actions::gui::ParamDateTime( missionInterface_, parameter, simulation_.GetDateTime() );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAgentKnowledge
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildAgentKnowledge( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamAgentKnowledge( missionInterface_, parameter, knowledgeConverter_, *entity_, controllers_.controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAgentKnowledgeList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildAgentKnowledgeList( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamAgentKnowledgeList( missionInterface_, parameter, controllers_.actions_, controllers_.controller_, knowledgeConverter_, *entity_ );
}
 
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildObjectKnowledge
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildObjectKnowledge( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamObjectKnowledge( missionInterface_, parameter, objectKnowledgeConverter_, *entity_, controllers_.controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildObjectKnowledgeList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildObjectKnowledgeList( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamObjectKnowledgeList( missionInterface_, parameter, controllers_.actions_, controllers_.controller_, objectKnowledgeConverter_, *entity_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPopulationKnowledge
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildPopulationKnowledge( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamPopulationKnowledge( missionInterface_, parameter, knowledgeConverter_, *entity_, controllers_.controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPath
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildPath( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamPath( missionInterface_, parameter, layer_, converter_, *entity_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPathList
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildPathList( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamPathList( missionInterface_, parameter, layer_, converter_, *entity_, controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPoint
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildPoint( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamPoint( missionInterface_, parameter, converter_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPointList
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildPointList( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamPointList( missionInterface_, parameter, layer_, converter_, controllers_.actions_ );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPolygon
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildPolygon( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamPolygon( parameter, layer_, converter_ );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPolygonList
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildPolygonList( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamPolygonList( missionInterface_, parameter, layer_, converter_, controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLocation
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildLocation( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamLocation( parameter, layer_, converter_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLocationList
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildLocationList( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamLocationList( missionInterface_, parameter, layer_, converter_, controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildGenObject
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildGenObject( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamObstacle( missionInterface_, parameter, staticModel_.objectTypes_, layer_, converter_, controllers_.controller_ );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildObstacleList
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildGenObjectList( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamObstacleList( missionInterface_, parameter, staticModel_.objectTypes_, layer_, converter_, controllers_.actions_, controllers_.controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildMissionObjective
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildMissionObjective( const kernel::OrderParameter& parameter ) const
{
    return new actions::gui::ParamMissionObjective( parameter, layer_, simulation_.GetDateTime(), converter_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildMissionObjectiveList
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildMissionObjectiveList( const kernel::OrderParameter& parameter ) const
{
    return new actions::gui::ParamMissionObjectiveList( missionInterface_, parameter, layer_, simulation_.GetDateTime(), converter_, controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildMaintenancePriorities
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildMaintenancePriorities( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamEquipmentList( missionInterface_, parameter, staticModel_.objectTypes_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildMedicalPriorities
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildMedicalPriorities( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamHumanWoundList( missionInterface_, parameter );
}

namespace
{
    struct OrderParameterValueVisitor : public kernel::OrderParameterValueVisitor_ABC
    {
        OrderParameterValueVisitor( actions::gui::ParamComboBox< ASN1INT >& param ) : param_( &param ) {}
        virtual void Visit( const OrderParameterValue& value )
        {
            param_->AddItem( value.GetName().c_str(), value.GetId() );
        }
        actions::gui::ParamComboBox< ASN1INT >* param_;
    };
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildEnumeration
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildEnumeration( const OrderParameter& parameter ) const
{
    actions::gui::ParamComboBox< ASN1INT >& param = BuildVarList< ASN1INT >( parameter );
    OrderParameterValueVisitor visitor( param );
    parameter.Accept( visitor );
    return &param;
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLimit
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildLimit( const OrderParameter& parameter ) const
{
    return new actions::gui::LimitParameter( missionInterface_, parameter, converter_, controllers_.controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLimaList
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildLimaList( const OrderParameter& parameter ) const
{
    return new actions::gui::ParamLimaList( missionInterface_, parameter, converter_, controllers_.actions_, controllers_.controller_, simulation_.GetDateTime() );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildDotationTypeList
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildDotationTypeList( const kernel::OrderParameter& parameter ) const
{
    return new actions::gui::ParamDotationTypeList( missionInterface_, parameter, staticModel_.objectTypes_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildIntelligenceList
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
actions::gui::Param_ABC* MissionInterfaceBuilder::BuildIntelligenceList( const kernel::OrderParameter& parameter ) const
{
    return new actions::gui::ParamIntelligenceList( missionInterface_, parameter, converter_, controllers_.actions_, controllers_.controller_ );
}
