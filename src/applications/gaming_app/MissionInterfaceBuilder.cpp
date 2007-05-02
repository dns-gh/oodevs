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
#include "MissionInterface_ABC.h"
#include "ParamAgent.h"
#include "ParamAgentList.h"
#include "ParamAtlasNature.h"
#include "ParamAutomat.h"
#include "ParamAutomatList.h"
#include "ParamPath.h"
#include "ParamPathList.h"
#include "ParamPoint.h"
#include "ParamPolygon.h"
#include "ParamGDH.h"
#include "ParamBool.h"
#include "ParamLocation.h"
#include "ParamLocationList.h"
#include "ParamPointList.h"
#include "ParamPolygonList.h"
#include "ParamAgentKnowledge.h"
#include "ParamAgentKnowledgeList.h"
#include "ParamPopulationKnowledge.h"
#include "ParamObjectKnowledge.h"
#include "ParamObjectKnowledgeList.h"
#include "ParamObstacle.h"
#include "ParamObstacleList.h"
#include "ParamDirection.h"
#include "ParamNumericField.h"
#include "ParamEquipmentList.h"
#include "ParamHumanWoundList.h"
#include "ParamDotationDType.h"
#include "ParamLimits.h"
#include "ParamLimaList.h"
#include "gaming/Tools.h"
#include "gaming/AgentKnowledge_ABC.h"
#include "gaming/ObjectKnowledge_ABC.h"
#include "gaming/PopulationKnowledge_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/OrderParameterValue.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder constructor
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
MissionInterfaceBuilder::MissionInterfaceBuilder( ActionController& controller, gui::ParametersLayer& layer
                                                , const CoordinateConverter_ABC& converter, AgentKnowledgeConverter_ABC& knowledgeConverter
                                                , ObjectKnowledgeConverter_ABC& objectKnowledgeConverter, const ObjectTypes& objectTypes )
    : controller_              ( controller )
    , layer_                   ( layer )
    , converter_               ( converter )
    , knowledgeConverter_      ( knowledgeConverter )
    , objectKnowledgeConverter_( objectKnowledgeConverter )
    , objectTypes_             ( objectTypes )
    , missionInterface_        ( 0 )
    , entity_                  ( 0 )
{
    builderFunctors_["agent"]               = &MissionInterfaceBuilder::BuildAgent;
    builderFunctors_["agentlist"]           = &MissionInterfaceBuilder::BuildAgentList;
    builderFunctors_["automate"]            = &MissionInterfaceBuilder::BuildAutomat;
    builderFunctors_["automatelist"]        = &MissionInterfaceBuilder::BuildAutomatList;
    builderFunctors_["genobject"]           = &MissionInterfaceBuilder::BuildGenObject;
    builderFunctors_["genobjectlist"]       = &MissionInterfaceBuilder::BuildGenObjectList;
    builderFunctors_["dotationtype"]        = &MissionInterfaceBuilder::BuildDotation;
    builderFunctors_["natureatlas"]         = &MissionInterfaceBuilder::BuildAtlasNature;

    builderFunctors_["bool"]                = &MissionInterfaceBuilder::BuildBoolean;
    builderFunctors_["direction"]           = &MissionInterfaceBuilder::BuildDirection;
    builderFunctors_["gdh"]                 = &MissionInterfaceBuilder::BuildGDH;
    builderFunctors_["numeric"]             = &MissionInterfaceBuilder::BuildNumeric;

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

    builderFunctors_["limits"]                = &MissionInterfaceBuilder::BuildLimits;
    builderFunctors_["limalist"]              = &MissionInterfaceBuilder::BuildLimaList;
    builderFunctors_["dangerousdirection"]    = &MissionInterfaceBuilder::BuildDangerousDirection;
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
void MissionInterfaceBuilder::Build( MissionInterface_ABC& missionInterface, Entity_ABC& entity, const OrderType& order )
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
    CIT_BuilderFunctors it = builderFunctors_.find( parameter.GetType().lower() );
    if( it != builderFunctors_.end() )
    {
        T_BuilderFunctor functor = it->second;
        if( Param_ABC* param = (this->*functor)( parameter ) )
            if( parameter.IsContext() )
                missionInterface_->AddOrderContext( *param );
            else
                missionInterface_->AddParameter( *param );
    }
    else
        throw std::runtime_error( tools::translate( "MissionInterfaceBuilder", "Unknown mission parameter: %1" ).arg( parameter.GetType() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAgent
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAgent( const OrderParameter& parameter ) const
{
    return new ParamAgent( missionInterface_, parameter );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAgentList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAgentList( const OrderParameter& parameter ) const
{
    return new ParamAgentList( missionInterface_, parameter );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAutomat
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAutomat( const OrderParameter& parameter ) const
{
    return new ParamAutomat( missionInterface_, parameter );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAutomatList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAutomatList( const OrderParameter& parameter ) const
{
    return new ParamAutomatList( missionInterface_, parameter );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAtlasNature
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAtlasNature( const OrderParameter& parameter ) const
{
    return new ParamAtlasNature( missionInterface_, parameter );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildDotation
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildDotation( const OrderParameter& parameter ) const
{
    return new ParamDotationDType( parameter, objectTypes_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildBoolean
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildBoolean( const OrderParameter& parameter ) const
{
    return new ParamBool( missionInterface_, parameter ); // $$$$ SBO 2007-03-14: default value?
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildDirection
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildDirection( const OrderParameter& parameter ) const
{
    return new ParamDirection( missionInterface_, parameter );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildGDH
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildGDH( const OrderParameter& parameter ) const
{
    return new ParamGDH( missionInterface_, parameter );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildNumeric
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildNumeric( const OrderParameter& parameter ) const
{
    return new ParamNumericField( parameter, true );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAgentKnowledge
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAgentKnowledge( const OrderParameter& parameter ) const
{
    return new ParamAgentKnowledge( missionInterface_, parameter, knowledgeConverter_, *entity_  );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAgentKnowledgeList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAgentKnowledgeList( const OrderParameter& parameter ) const
{
    return new ParamAgentKnowledgeList( missionInterface_, parameter, knowledgeConverter_, *entity_ );
}
 
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildObjectKnowledge
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildObjectKnowledge( const OrderParameter& parameter ) const
{
    return new ParamObjectKnowledge( missionInterface_, parameter, objectKnowledgeConverter_, *entity_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildObjectKnowledgeList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildObjectKnowledgeList( const OrderParameter& parameter ) const
{
    return new ParamObjectKnowledgeList( missionInterface_, parameter, objectKnowledgeConverter_, *entity_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPopulationKnowledge
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPopulationKnowledge( const OrderParameter& parameter ) const
{
    return new ParamPopulationKnowledge( missionInterface_, parameter, knowledgeConverter_, *entity_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPath
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPath( const OrderParameter& parameter ) const
{
    return new ParamPath( missionInterface_, parameter, layer_, converter_, *entity_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPathList
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPathList( const OrderParameter& parameter ) const
{
    return new ParamPathList( missionInterface_, parameter, layer_, converter_, *entity_, controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPoint
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPoint( const OrderParameter& parameter ) const
{
    return new ParamPoint( missionInterface_, parameter, converter_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPointList
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPointList( const OrderParameter& parameter ) const
{
    return new ParamPointList( missionInterface_, parameter, layer_, converter_, controller_ );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPolygon
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPolygon( const OrderParameter& parameter ) const
{
    return new ParamPolygon( parameter, layer_, converter_ );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPolygonList
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPolygonList( const OrderParameter& parameter ) const
{
    return new ParamPolygonList( missionInterface_, parameter, layer_, converter_, controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLocation
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildLocation( const OrderParameter& parameter ) const
{
    return new ParamLocation( parameter, layer_, converter_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLocationList
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildLocationList( const OrderParameter& parameter ) const
{
    return new ParamLocationList( missionInterface_, parameter, layer_, converter_, controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildGenObject
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildGenObject( const OrderParameter& parameter ) const
{
    return new ParamObstacle( missionInterface_, parameter, objectTypes_, layer_, converter_ );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildObstacleList
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildGenObjectList( const OrderParameter& parameter ) const
{
    return new ParamObstacleList( missionInterface_, parameter, objectTypes_, layer_, converter_, controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildMaintenancePriorities
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildMaintenancePriorities( const OrderParameter& parameter ) const
{
    return new ParamEquipmentList( missionInterface_, parameter, objectTypes_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildMedicalPriorities
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildMedicalPriorities( const OrderParameter& parameter ) const
{
    return new ParamHumanWoundList( missionInterface_, parameter );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildEnumeration
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildEnumeration( const OrderParameter& parameter ) const
{
    ParamComboBox< ASN1INT >& param = BuildVarList< ASN1INT >( parameter );
    Iterator< const OrderParameterValue& > it = parameter.CreateIterator();
    while( it.HasMoreElements() )
    {
        const OrderParameterValue& value = it.NextElement();
        param.AddItem( value.GetName(), value.GetId() );
    }
    return &param;
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLimit
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildLimits( const OrderParameter& parameter ) const
{
    return new ParamLimits( missionInterface_, parameter, converter_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLimaList
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildLimaList( const OrderParameter& parameter ) const
{
    return new ParamLimaList( missionInterface_, parameter, converter_, controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildDangerousDirection
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildDangerousDirection( const OrderParameter& parameter ) const
{
    return new ParamDirection( missionInterface_, parameter );
}
