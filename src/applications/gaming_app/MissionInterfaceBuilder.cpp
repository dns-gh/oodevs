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
#include "ParamGDH.h"
#include "ParamBool.h"
#include "ParamLocation.h"
#include "ParamLocationList.h"
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
#include "OptionalParamFunctor_ABC.h"
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

namespace
{
    class OptionalParamFunctor : public OptionalParamFunctor_ABC
    {
    public:
        OptionalParamFunctor( ASN1T_MissionParameter& asnParameter ) : asnParameter_( &asnParameter ) { asnParameter_->null_value = 0; }
        virtual void SetOptionalPresent()
        {
            asnParameter_->null_value = 0;
        }
    private:
        ASN1T_MissionParameter* asnParameter_;
    };

    class DummyOptionalFunctor : public OptionalParamFunctor_ABC
    {
    public:
        DummyOptionalFunctor() {}
        virtual void SetOptionalPresent()
        {
            // NOTHING
        }
    };

    OptionalParamFunctor_ABC* BuildOptional( ASN1T_MissionParameter& asnParameter, bool optional )
    {
        if( !optional )
            return 0;
        return new OptionalParamFunctor( asnParameter );
    }

    OptionalParamFunctor_ABC* BuildOptional( bool optional )
    {
        return !optional ? 0 : new DummyOptionalFunctor();
    }
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::AddParameter
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
void MissionInterfaceBuilder::AddParameter( ASN1T_MissionParameter& asnParameter, const QString& type, const QString& name, bool optional, const T_Values& values )
{
    Param_ABC* parameter = 0;
    CIT_BuilderFunctors it = builderFunctors_.find( type.lower() );
    if( it != builderFunctors_.end() )
    {
        if( values.empty() )
        {
            T_BuilderFunctor functor = it->second;
            parameter = (this->*functor)( asnParameter, name );
        }
        else
            parameter = BuildEnumeration( asnParameter, name, values );
    }
    else
        throw std::runtime_error( tools::translate( "MissionInterfaceBuilder", "Unknown mission parameter: %1" ).arg( type ).ascii() );
    if( parameter )
        missionInterface_->AddParameter( *parameter, BuildOptional( asnParameter, optional ) );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::AddParameter
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void MissionInterfaceBuilder::AddParameter( ASN1T_OrderContext& context, bool optional )
{
    Param_ABC* parameter = BuildLimits( context.limite_gauche, context.limite_droite
                                      , tools::translate( "MissionInterfaceBuilder", "Set limit 1" )
                                      , tools::translate( "MissionInterfaceBuilder", "Set limit 2" ) );
    missionInterface_->AddParameter( *parameter, BuildOptional( optional ) );
    parameter = BuildLimaList( context.limas, tools::translate( "MissionInterfaceBuilder", "Add lima" ) );
    missionInterface_->AddParameter( *parameter, BuildOptional( true ) );
    parameter = BuildDirection( context.direction_dangereuse, tools::translate( "MissionInterfaceBuilder", "Dangerous direction" ) );
    missionInterface_->AddParameter( *parameter, BuildOptional( false ) );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::AddParameter
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void MissionInterfaceBuilder::AddParameter( ASN1T_EnumAutomateOrderFormation& formation, bool optional )
{
    ParamRadioBtnGroup<ASN1T_EnumAutomateOrderFormation>& param = BuildRadioButtonGroup( formation, tools::translate( "MissionInterfaceBuilder", "Formation" ) );
    param.AddButton( tools::translate( "MissionInterfaceBuilder", "One echelon" ) , EnumAutomateOrderFormation::un_echelon );
    param.AddButton( tools::translate( "MissionInterfaceBuilder", "Two echelons" ), EnumAutomateOrderFormation::deux_echelons, true );
    missionInterface_->AddParameter( param, BuildOptional( optional ) );
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
Param_ABC* MissionInterfaceBuilder::BuildAgent( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_agent;
    return new ParamAgent( missionInterface_, asnParameter.value.u.agent, name, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAgentList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAgentList( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_listAgent;
    return new ParamAgentList( missionInterface_, asnParameter.value.u.listAgent, name, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAutomat
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAutomat( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_automate;
    return new ParamAutomat( missionInterface_, asnParameter.value.u.automate, name, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAutomatList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAutomatList( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_listAutomate;
    return new ParamAutomatList( missionInterface_, asnParameter.value.u.listAutomate, name, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAtlasNature
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAtlasNature( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_natureAtlas;
    return new ParamAgentType( asnParameter.value.u.natureAtlas, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildDotation
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildDotation( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_typeDotation;
    return new ParamDotationDType( asnParameter.value.u.typeDotation, name, objectTypes_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildBoolean
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildBoolean( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_aBool;
    return new ParamBool( asnParameter.value.u.aBool, name, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildDirection
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildDirection( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_direction;
    return new ParamDirection( asnParameter.value.u.direction, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildGDH
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildGDH( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_gDH;
    return new ParamGDH( missionInterface_, asnParameter.value.u.gDH, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildNumeric
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildNumeric( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_aReal;
    return new ParamNumericField( missionInterface_, asnParameter.value.u.aReal, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAgentKnowledge
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAgentKnowledge( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_knowledgeAgent;
    return new ParamAgentKnowledge( missionInterface_, asnParameter.value.u.knowledgeAgent, name, name, knowledgeConverter_, *entity_  );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildAgentKnowledgeList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildAgentKnowledgeList( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_listKnowledgeAgent;
    return new ParamAgentKnowledgeList( missionInterface_, asnParameter.value.u.listKnowledgeAgent, name, name, knowledgeConverter_, *entity_ );
}
 
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildObjectKnowledge
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildObjectKnowledge( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_knowledgeObject;
    return new ParamObjectKnowledge( missionInterface_, asnParameter.value.u.knowledgeObject, name, name, objectKnowledgeConverter_, *entity_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildObjectKnowledgeList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildObjectKnowledgeList( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_listKnowledgeObject;
    return new ParamObjectKnowledgeList( missionInterface_, asnParameter.value.u.listKnowledgeObject, name, name, objectKnowledgeConverter_, *entity_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPopulationKnowledge
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPopulationKnowledge( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_knowledgePopulation;
    return new ParamPopulationKnowledge( missionInterface_, asnParameter.value.u.knowledgePopulation, name, name, knowledgeConverter_, *entity_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPath
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPath( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_itineraire;
    return new ParamPath( missionInterface_, asnParameter.value.u.itineraire, name, name, layer_, converter_, *entity_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPathList
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPathList( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_listItineraire;
    return new ParamPathList( missionInterface_, asnParameter.value.u.listItineraire, name, layer_, converter_, *entity_, controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPoint
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPoint( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_point;
    return new ParamPoint( missionInterface_, asnParameter.value.u.point, name, name, converter_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPointList
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPointList( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_listPoint;
    return new ParamLocationList( missionInterface_, asnParameter.value.u.listPoint, name, name, layer_, converter_ );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPolygon
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPolygon( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_polygon;
    return new ParamLocation( asnParameter.value.u.polygon, name, layer_, converter_ );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildPolygonList
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildPolygonList( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_listPolygon;
    return new ParamLocationList( missionInterface_, asnParameter.value.u.listPolygon, name, name, layer_, converter_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLocation
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildLocation( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_localisation;
    return new ParamLocation( asnParameter.value.u.localisation, name, layer_, converter_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLocationList
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildLocationList( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_listLocalisation;
    return new ParamLocationList( missionInterface_, asnParameter.value.u.listLocalisation, name, name, layer_, converter_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildGenObject
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildGenObject( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_missionGenObject;
    return new ParamObstacle( missionInterface_, asnParameter.value.u.missionGenObject, name, objectTypes_, layer_, converter_ );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildObstacleList
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildGenObjectList( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_listMissionGenObject;
    return new ParamObstacleList( missionInterface_, asnParameter.value.u.listMissionGenObject, name, objectTypes_, layer_, converter_, controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildMaintenancePriorities
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildMaintenancePriorities( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_maintenancePriorites;
    return new ParamEquipmentList( missionInterface_, asnParameter.value.u.maintenancePriorites, name, objectTypes_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildMedicalPriorities
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildMedicalPriorities( ASN1T_MissionParameter& asnParameter, const QString& name ) const
{
    asnParameter.value.t = T_MissionParameter_value_santePriorites;
    return new ParamHumanWoundList( missionInterface_, asnParameter.value.u.santePriorites, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLimit
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildLimits( ASN1T_Line& limit1, ASN1T_Line& limit2, const QString& name1, const QString& name2 ) const
{
    return new ParamLimits( missionInterface_, limit1, limit2, name1, name2, name1, name2 );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildLimaList
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildLimaList( ASN1T_LimasOrder& asn, const QString& name ) const
{
    return new ParamLimaList( missionInterface_, asn, name, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildDirection
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildDirection( ASN1T_Direction&  asn, const QString& name ) const
{
    return new ParamDirection( asn, name );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceBuilder::BuildEnumeration
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
Param_ABC* MissionInterfaceBuilder::BuildEnumeration( ASN1T_MissionParameter& asnParameter, const QString& name, const T_Values& values )
{
    asnParameter.value.t = T_MissionParameter_value_enumeration;
    ParamComboBox< ASN1INT >& param = BuildVarList( asnParameter.value.u.enumeration, name );
    for( T_Values::const_iterator it = values.begin(); it != values.end(); ++it )
        param.AddItem( it->second.c_str(), it->first );
    return &param;
}
