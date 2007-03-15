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
#include "ParamAgentType.h"
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

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder constructor
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
MissionInterfaceBuilder::MissionInterfaceBuilder( kernel::ActionController& controller, gui::ParametersLayer& layer
                                                , const kernel::CoordinateConverter_ABC& converter, AgentKnowledgeConverter_ABC& knowledgeConverter
                                                , ObjectKnowledgeConverter_ABC& objectKnowledgeConverter, const kernel::ObjectTypes& objectTypes )
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
    builderFunctors_["enumeration"]           = 0;
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
// Name: MissionInterfaceBuilder::Begin
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void MissionInterfaceBuilder::Begin( MissionInterface_ABC& missionInterface, kernel::Entity_ABC& entity )
{
    missionInterface_ = &missionInterface;
    entity_ = &entity;
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::AddParameter
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
void MissionInterfaceBuilder::AddParameter( const QString& type, const QString& name, bool optional, const T_Values& values )
{
    Param_ABC* parameter = 0;
    CIT_BuilderFunctors it = builderFunctors_.find( type.lower() );
    if( it != builderFunctors_.end() )
    {
        if( values.empty() )
        {
            T_BuilderFunctor functor = it->second;
            parameter = (this->*functor)( name );
        }
        else
            parameter = BuildEnumeration( name, values );
    }
    else
        throw std::runtime_error( tools::translate( "MissionInterfaceBuilder", "Unknown mission parameter: %1" ).arg( type ).ascii() );
    if( parameter )
        missionInterface_->AddParameter( *parameter, optional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::AddOrderContext
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void MissionInterfaceBuilder::AddOrderContext( bool optional )
{
    Param_ABC* parameter = BuildLimits( tools::translate( "MissionInterfaceBuilder", "Set limit 1" )
                                      , tools::translate( "MissionInterfaceBuilder", "Set limit 2" ) );
    missionInterface_->AddOrderContext( *parameter, optional );
    parameter = BuildLimaList( tools::translate( "MissionInterfaceBuilder", "Add lima" ) );
    missionInterface_->AddOrderContext( *parameter, true );
    parameter = BuildDirection( tools::translate( "MissionInterfaceBuilder", "Dangerous direction" ) );
    missionInterface_->AddOrderContext( *parameter, false );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::End
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void MissionInterfaceBuilder::End()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAgent
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAgent( const QString& name ) const
{
    return new ParamAgent( missionInterface_, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAgentList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAgentList( const QString& name ) const
{
    return new ParamAgentList( missionInterface_, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAutomat
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAutomat( const QString& name ) const
{
    return new ParamAutomat( missionInterface_, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAutomatList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAutomatList( const QString& name ) const
{
    return new ParamAutomatList( missionInterface_, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAtlasNature
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAtlasNature( const QString& name ) const
{
    return new ParamAgentType( name ); // $$$$ SBO 2007-03-14: rename AtlasNature or something
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildDotation
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildDotation( const QString& name ) const
{
    return new ParamDotationDType( name, objectTypes_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildBoolean
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildBoolean( const QString& name ) const
{
    return new ParamBool( name ); // $$$$ SBO 2007-03-14: default value?
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildDirection
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildDirection( const QString& name ) const
{
    return new ParamDirection( name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildGDH
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildGDH( const QString& name ) const
{
    return new ParamGDH( missionInterface_, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildNumeric
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildNumeric( const QString& name ) const
{
    return new ParamNumericField( name, true );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAgentKnowledge
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAgentKnowledge( const QString& name ) const
{
    return new ParamAgentKnowledge( missionInterface_, name, knowledgeConverter_, *entity_  );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAgentKnowledgeList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAgentKnowledgeList( const QString& name ) const
{
    return new ParamAgentKnowledgeList( missionInterface_, name, knowledgeConverter_, *entity_ );
}
 
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildObjectKnowledge
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildObjectKnowledge( const QString& name ) const
{
    return new ParamObjectKnowledge( missionInterface_, name, objectKnowledgeConverter_, *entity_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildObjectKnowledgeList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildObjectKnowledgeList( const QString& name ) const
{
    return new ParamObjectKnowledgeList( missionInterface_, name, objectKnowledgeConverter_, *entity_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPopulationKnowledge
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPopulationKnowledge( const QString& name ) const
{
    return new ParamPopulationKnowledge( missionInterface_, name, knowledgeConverter_, *entity_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPath
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPath( const QString& name ) const
{
    return new ParamPath( missionInterface_, name, layer_, converter_, *entity_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPathList
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPathList( const QString& name ) const
{
    return new ParamPathList( missionInterface_, name, layer_, converter_, *entity_, controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPoint
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPoint( const QString& name ) const
{
    return new ParamPoint( missionInterface_, name, converter_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPointList
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPointList( const QString& name ) const
{
    return new ParamPointList( missionInterface_, name, layer_, converter_ );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPolygon
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPolygon( const QString& name ) const
{
    return new ParamPolygon( name, layer_, converter_ );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPolygonList
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPolygonList( const QString& name ) const
{
    return new ParamPolygonList( missionInterface_, name, layer_, converter_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLocation
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildLocation( const QString& name ) const
{
    return new ParamLocation( name, layer_, converter_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLocationList
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildLocationList( const QString& name ) const
{
    return new ParamLocationList( missionInterface_, name, layer_, converter_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildGenObject
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildGenObject( const QString& name ) const
{
    return new ParamObstacle( missionInterface_, name, objectTypes_, layer_, converter_ );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildObstacleList
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildGenObjectList( const QString& name ) const
{
    return new ParamObstacleList( missionInterface_, name, objectTypes_, layer_, converter_, controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildMaintenancePriorities
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildMaintenancePriorities( const QString& name ) const
{
    return new ParamEquipmentList( missionInterface_, name, objectTypes_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildMedicalPriorities
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildMedicalPriorities( const QString& name ) const
{
    return new ParamHumanWoundList( missionInterface_, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLimit
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildLimits( const QString& name1, const QString& name2 ) const
{
    return new ParamLimits( missionInterface_, name1, name2 );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLimaList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildLimaList( const QString& name ) const
{
    return new ParamLimaList( missionInterface_, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildEnumeration
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildEnumeration( const QString& name, const T_Values& values )
{
    ParamComboBox< ASN1INT >& param = BuildVarList< ASN1INT >( name );
    for( T_Values::const_iterator it = values.begin(); it != values.end(); ++it )
        param.AddItem( it->second.c_str(), it->first );
    return &param;
}
