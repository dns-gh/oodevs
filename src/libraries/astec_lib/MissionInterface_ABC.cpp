// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "MissionInterface_ABC.h"
#include "moc_MissionInterface_ABC.cpp"

#include "ParamPoint.h"
#include "ParamAgent.h"
#include "ParamAgentList.h"
#include "ParamPath.h"
#include "ParamPathList.h"
#include "ParamGDH.h"
#include "ParamBool.h"
#include "ParamLocation.h"
#include "ParamLocationList.h"
#include "ParamAgentType.h"
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
#include "Agent.h"
#include "Object.h"
#include "ObjectKnowledge.h"
#include "AgentKnowledge.h"
#include "PopulationKnowledge.h"
#include "ParamLimits.h"
#include "ParamLimaList.h"
#include "Lima.h"
#include "Positions.h"
#include "ObjectTypes.h"
#include "OptionalParamFunctor_ABC.h"

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC constructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface_ABC::MissionInterface_ABC( QWidget* parent, Entity_ABC& agent, ActionController& controller, ParametersLayer& layer, const CoordinateConverter_ABC& converter, AgentKnowledgeConverter_ABC& knowledgeConverter, const ObjectTypes& objectTypes )
    : QVBox      ( parent )
    , controller_( controller )
    , agent_     ( agent )
    , layer_     ( layer )
    , converter_ ( converter )
    , knowledgeConverter_( knowledgeConverter )
    , objectTypes_( objectTypes )
{
    setMargin( 5 );
    setSpacing( 4 );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface_ABC::~MissionInterface_ABC()
{
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        (*it)->RemoveFromController();
    for( CIT_OptionalFunctors it = optionalFunctors_.begin(); it != optionalFunctors_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CheckValidity
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
bool MissionInterface_ABC::CheckValidity()
{
    bool b = true;
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        b = (*it)->CheckValidity() && b;
    return b;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::IsEmpty
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
bool MissionInterface_ABC::IsEmpty() const
{
    return parameters_.empty();
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateOkCancelButtons
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateOkCancelButtons()
{
    QHBox* pBox = new QHBox( this );
    QPushButton* pOk = new QPushButton( tr( "Valider" ), pBox );
    QPushButton* pCancel = new QPushButton( tr( "Annuler" ), pBox );
    pOk->setDefault( true );

    connect( pOk, SIGNAL( clicked() ), this, SLOT( OnOk() ) );
    connect( pCancel, SIGNAL( clicked() ), parent(), SLOT( hide() ) );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::AddParameter
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void MissionInterface_ABC::AddParameter( Param_ABC& parameter, OptionalParamFunctor_ABC* pOptional )
{
    parameters_.push_back( &parameter );
    parameter.SetOptional( pOptional );
    parameter.RegisterIn( controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::Commit
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionInterface_ABC::Commit()
{
    for( CIT_Parameters it = parameters_.begin() ; it != parameters_.end() ; ++it )
        (*it)->Commit();
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionInterface_ABC::Draw( const GlTools_ABC& tools, const geometry::Rectangle2f& extent ) const
{
    for( CIT_Parameters it = parameters_.begin() ; it != parameters_.end() ; ++it )
        (*it)->Draw( agent_.Get< Positions >().GetPosition(), extent, tools );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateNatureAtlas
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateNatureAtlas( ASN1T_NatureAtlas& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamAgentType( this, asn, strName ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateGDH
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateGDH( ASN1T_GDH& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamGDH( this, asn, strName ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateDirection
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateDirection( ASN1T_Direction& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamDirection( this, asn, strName ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreatePoint
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreatePoint( ASN1T_Point& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamPoint( this, asn, strName, strName, converter_ ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreatePath
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreatePath( ASN1T_Itineraire& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamPath( this, asn, strName, strName, layer_, converter_, agent_ ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreatePathList
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreatePathList( ASN1T_ListItineraire& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamPathList( this, asn, strName, layer_, converter_, agent_, controller_ ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateAgentList
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateAgentList( ASN1T_ListAgent& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamAgentList( this, asn, strName, strName ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateAgent
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateAgent( ASN1T_Agent& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamAgent( this, asn, strName, strName ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateAutomateList
// Created: APE 2004-10-25
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateAutomateList( ASN1T_ListAutomate& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamAgentList( this, (ASN1T_ListAgent&)asn, strName, strName ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateAutomate
// Created: APE 2004-10-25
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateAutomate( ASN1T_Agent& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamAgent( this, asn, strName, strName ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateLocation
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateLocation( ASN1T_Localisation& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamLocation( this, asn, strName, layer_, converter_ ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateLocationList
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateLocationList( ASN1T_ListLocalisation& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    // $$$$ AGE 2006-04-03: pOptional ?
    AddParameter( *new ParamLocationList( this, asn, strName, strName, layer_, converter_ ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateLPolygonList
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreatePolygonList( ASN1T_ListPolygon& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamLocationList( this, asn, strName, strName, layer_, converter_ ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreatePointList
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreatePointList( ASN1T_ListPoint& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamLocationList( this, asn, strName, strName, layer_, converter_ ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateAgentKnowledge
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateAgentKnowledge( ASN1T_KnowledgeAgent& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamAgentKnowledge( this, asn, strName, strName, knowledgeConverter_, agent_ ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateAgentKnowledgeList
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateAgentKnowledgeList( ASN1T_ListKnowledgeAgent& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamAgentKnowledgeList( this, asn, strName, strName, knowledgeConverter_, agent_ ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateObjectKnowledge
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateObjectKnowledge( ASN1T_KnowledgeObject& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamObjectKnowledge( this, asn, strName, strName ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateObjectKnowledgeList
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateObjectKnowledgeList( ASN1T_ListKnowledgeObject& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamObjectKnowledgeList( this, asn, strName, strName ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateGenObject
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateGenObject( ASN1T_MissionGenObject& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamObstacle( this, asn, strName, objectTypes_, layer_, converter_ ), pOptional ) ;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateGenObjectList
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateGenObjectList( ASN1T_ListMissionGenObject& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamObstacleList( this, asn, strName, objectTypes_, layer_, converter_, controller_ ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateBool
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateBool( ASN1BOOL& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional, QWidget* pParent )
{
//    (pParent != 0) ? pParent : this
    AddParameter( *new ParamBool( this, asn, strName ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateNumeric
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateNumeric( ASN1INT& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamNumericField( this, asn, strName ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateNumeric
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateNumeric( ASN1REAL& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamNumericField( this, asn, strName ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateMaintenancePriorities
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateMaintenancePriorities( ASN1T_MaintenancePriorites& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamEquipmentList( this, asn, strName, objectTypes_ ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateMedicalPriorities
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateMedicalPriorities( ASN1T_SantePriorites& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamHumanWoundList( this, asn, strName ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreatePopulationKnowledge
// Created: HME 2005-12-22
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreatePopulationKnowledge( ASN1T_KnowledgePopulation& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamPopulationKnowledge( this, asn, strName, strName, knowledgeConverter_, agent_ ), pOptional );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateLimaList
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateLimaList( ASN1T_ListOID& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamLimaList( this, asn, "Limas", strName ), pOptional );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateLimits
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateLimits( ASN1T_OID& id1, ASN1T_OID& id2 , const std::string& name1, const std::string& name2, OptionalParamFunctor_ABC* pOptional )
{
    AddParameter( *new ParamLimits( this, id1, id2, "Limite 1", "Limite 2", name1, name2 ), pOptional );
}
