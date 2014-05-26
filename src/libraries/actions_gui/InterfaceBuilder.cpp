// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "InterfaceBuilder.h"

#include "LimaParameter.h"
#include "LimitParameter.h"
#include "ListParameter.h"
#include "MissionInterface.h"
#include "ParamAgent.h"
#include "ParamAtlasNature.h"
#include "ParamAutomat.h"
#include "ParamBool.h"
#include "ParamCrowd.h"
#include "ParamDateTime.h"
#include "ParamDirection.h"
#include "ParamDotationType.h"
#include "ParamDotationTypeList.h"
#include "ParamEquipmentType.h"
#include "ParamEquipmentList.h"
#include "ParamHumanWoundList.h"
#include "ParamInhabitant.h"
#include "ParamLocation.h"
#include "ParamLocation_ABC.h"
#include "ParamLocationComposite.h"
#include "ParamNumericField.h"
#include "ParamObjectKnowledge.h"
#include "ParamObstacle.h"
#include "ParamObstacleType.h"
#include "ParamPath.h"
#include "ParamPoint.h"
#include "ParamPopulationKnowledge.h"
#include "ParamQuantity.h"
#include "ParamResourceNetworkNode.h"
#include "ParamResourceNetworkType.h"
#include "ParamStringField.h"
#include "ParamTime.h"
#include "ParamUrbanBlock.h"
#include "ParamFireClass.h"

#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/OrderParameterValue.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "clients_kernel/Time_ABC.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder constructor
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
InterfaceBuilder::InterfaceBuilder( kernel::Controllers& controllers,
                                    const tools::ExerciseConfig& config,
                                    gui::ParametersLayer& layer,
                                    const kernel::StaticModel& staticModel,
                                    kernel::AgentKnowledgeConverter_ABC* knowledgeConverter /* = 0*/,
                                    kernel::ObjectKnowledgeConverter_ABC* objectKnowledgeConverter /* = 0*/,
                                    const kernel::Time_ABC* simulation /* = 0*/,
                                    tools::Resolver< kernel::TacticalLine_ABC >* tacticalLineResolver /* = 0 */ )
    : controllers_             ( controllers )
    , config_                  ( config )
    , layer_                   ( layer )
    , knowledgeConverter_      ( knowledgeConverter )
    , objectKnowledgeConverter_( objectKnowledgeConverter )
    , staticModel_             ( staticModel )
    , simulation_              ( simulation )
    , tacticalLineResolver_    ( tacticalLineResolver )
    , missionInterface_        ( 0 )
{
    // Base types
    AddFunctor< actions::gui::ParamBool >               ( "boolean" );
    AddFunctor< actions::gui::ParamDirection >          ( "heading" );
    AddFunctor< actions::gui::ParamInt >                ( "integer");
    AddFunctor< actions::gui::ParamFloat >              ( "float" );
    AddFunctor< actions::gui::ParamUnsignedInt >        ( "identifier" );
    AddFunctor< actions::gui::ParamStringField >        ( "string" );
    AddFunctor< actions::gui::ParamDateTime >           ( "datetime" );
    AddFunctor< actions::gui::ParamComboBox< int > >    ( "enumeration" );
    AddFunctor< actions::gui::ParamTime >               ( "time" );
    AddFunctor< actions::gui::ParamQuantity >           ( "quantity" );
    // Locations
    AddFunctor< actions::gui::ParamCircle >             ( "circle" );
    AddFunctor< actions::gui::ParamLine >               ( "line" );
    AddFunctor< actions::gui::ParamPath >               ( "path" );
    AddFunctor< actions::gui::ParamPoint >              ( "point" );
    AddFunctor< actions::gui::ParamPolygon >            ( "polygon" );
    AddFunctor< actions::gui::ParamRectangle >          ( "rectangle" );
    AddFunctor< actions::gui::ParamLocation >           ( "location" );
    AddFunctor< actions::gui::ParamLocationComposite >  ( "locationcomposite" );
    // Entities
    AddFunctor< actions::gui::ParamAgent >              ( "agent" );
    AddFunctor< actions::gui::ParamAutomat >            ( "automat" );
    AddFunctor< actions::gui::ParamCrowd >              ( "crowd" );            // Not used yet, but ready to be !
    AddFunctor< actions::gui::ParamInhabitant >         ( "inhabitant" );       // Same as previous
    AddFunctor< actions::gui::ParamUrbanBlock >         ( "urbanknowledge" );
    AddFunctor< actions::gui::LimitParameter >          ( "limit" );
    AddFunctor< actions::gui::LimaParameter >           ( "phaseline" );
    // Knowledges
    AddFunctor< actions::gui::ParamAgent >              ( "agentknowledge" );
    AddFunctor< actions::gui::ParamObjectKnowledge >    ( "objectknowledge" );
    AddFunctor< actions::gui::ParamPopulationKnowledge >( "crowdknowledge" );
    // Misc
    AddFunctor< actions::gui::ParamAtlasNature >        ( "natureatlas" );
    AddFunctor< actions::gui::ParamEquipmentType >      ( "equipmenttype" );
    AddFunctor< actions::gui::ParamEquipmentList >      ( "maintenancepriorities" );
    AddFunctor< actions::gui::ParamHumanWoundList >     ( "medicalpriorities" );
    AddFunctor< actions::gui::ParamResourceNetworkNode >( "resourcenetworknode" );
    AddFunctor< actions::gui::ParamResourceNetworkType >( "resourcenetworktype" );
    AddFunctor< actions::gui::ParamObstacle >           ( "plannedwork" );
    AddFunctor< actions::gui::ParamDotationType >       ( "resourcetype" );
    AddFunctor< actions::gui::ParamDotationTypeList >   ( "allresourcetype" );
    AddFunctor< actions::gui::ParamFireClass >          ( "fireclass" );
    AddFunctor< actions::gui::ParamObstacleType >       ( "obstacletype" );
}

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder destructor
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
InterfaceBuilder::~InterfaceBuilder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder::AddFunctor
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
template< typename ParameterType >
void InterfaceBuilder::AddFunctor( const std::string& typeName )
{
    builderFunctors_[ typeName ] = &InterfaceBuilder::BuildElement< ParameterType >;
}

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder::BuildElement
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
template< typename Element >
actions::gui::Param_ABC& InterfaceBuilder::BuildElement( const kernel::OrderParameter& parameter ) const
{
    if( parameter.IsRepeated() )
        return *new actions::gui::ListParameter< Element >( *this, parameter );
    return *new Element( *this, parameter );
}

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder::Build
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void InterfaceBuilder::BuildAll( actions::gui::MissionInterface& missionInterface, const kernel::OrderType& order )
{
    missionInterface_ = &missionInterface;
    parentObject_ = &missionInterface;
    paramInterface_ = &missionInterface;
    tools::Iterator< const kernel::OrderParameter& > it = order.CreateIterator();
    while( it.HasMoreElements() )
        BuildOne( it.NextElement() );
}

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder::Build
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
actions::gui::Param_ABC& InterfaceBuilder::BuildOne( const kernel::OrderParameter& parameter, bool isRegistered ) const
{
    std::string paramType = parameter.GetType();
    std::transform( paramType.begin(), paramType.end(), paramType.begin(), std::tolower );
    CIT_BuilderFunctors it = builderFunctors_.find( paramType );
    if( it != builderFunctors_.end() )
    {
        T_BuilderFunctor functor = it->second;
        actions::gui::Param_ABC& param = (this->*functor)( parameter );
        if( isRegistered && missionInterface_ )
            missionInterface_->AddParameter( param.GetType().c_str(), param );
        return param;
    }
    else
        throw MASA_EXCEPTION( tools::translate( "InterfaceBuilder", "Unknown mission parameter: %1" ).arg( parameter.GetType().c_str() ).toStdString() );
}

// =============================================================================
// Accessors
// =============================================================================

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder::GetParentObject
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
QObject* InterfaceBuilder::GetParentObject() const
{
    return parentObject_;
}

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder::GetParamInterface
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
ParamInterface_ABC& InterfaceBuilder::GetParamInterface() const
{
    assert( paramInterface_ != 0 );
    return *paramInterface_;
}

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder::GetParameterLayer
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
::gui::ParametersLayer& InterfaceBuilder::GetParameterLayer() const
{
    return layer_;
}

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder::GetControllers
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
kernel::Controllers& InterfaceBuilder::GetControllers() const
{
    return controllers_;
}

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder::GetAgentKnowledgeConverter
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
kernel::AgentKnowledgeConverter_ABC* InterfaceBuilder::GetAgentKnowledgeConverter() const
{
    return knowledgeConverter_;
}

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder::GetObjectKnowledgeConverter
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
kernel::ObjectKnowledgeConverter_ABC* InterfaceBuilder::GetObjectKnowledgeConverter() const
{
    return objectKnowledgeConverter_;
}

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder::GetCurrentDateTime
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
const QDateTime InterfaceBuilder::GetCurrentDateTime() const
{
    assert( simulation_ != 0 );
    return simulation_->GetDateTime();
}

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder::GetStaticModel
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
const kernel::StaticModel& InterfaceBuilder::GetStaticModel() const
{
    return staticModel_;
}

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder::SetParentObject
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
void InterfaceBuilder::SetParentObject( QObject* parent )
{
    assert( parent != 0 );
    parentObject_ = parent;
}

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder::SetParamInterface
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
void InterfaceBuilder::SetParamInterface( ParamInterface_ABC& paramInterface )
{
    paramInterface_ = &paramInterface;
}

// -----------------------------------------------------------------------------
// Name: tools::Resolver< kernel::TacticalLine_ABC >* InterfaceBuilder::GetTacticalLineResolver
// Created: ABR 2013-06-17
// -----------------------------------------------------------------------------
tools::Resolver< kernel::TacticalLine_ABC >* InterfaceBuilder::GetTacticalLineResolver() const
{
    return tacticalLineResolver_;
}

// -----------------------------------------------------------------------------
// Name: InterfaceBuilder::GetConfig
// Created: ABR 2014-03-04
// -----------------------------------------------------------------------------
const tools::ExerciseConfig& InterfaceBuilder::GetConfig() const
{
    return config_;
}
