// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-20 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/MissionInterface_ABC.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 18:36 $
// $Revision: 11 $
// $Workfile: MissionInterface_ABC.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "MissionInterface_ABC.h"
#include "moc_MissionInterface_ABC.cpp"

#include "MissionPanel.h"
#include "ParamPoint.h"
#include "ParamAgent.h"
#include "ParamAgentList.h"
#include "ParamPath.h"
#include "ParamPathList.h"
#include "ParamGDH.h"
#include "ParamComboBox.h"
#include "ParamBool.h"
#include "ParamSlider.h"
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

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC constructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface_ABC::MissionInterface_ABC( Agent_ABC& agent, MissionPanel& parentPanel )
    : QVBox         ( &parentPanel )
    , agent_        ( agent )
    , parentPanel_  ( parentPanel )
{
    this->setMargin( 5 );
    this->setSpacing( 4 );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface_ABC::~MissionInterface_ABC()
{
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::FillRemotePopupMenu
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
void MissionInterface_ABC::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if( popupMenu.count() > 0 )
        popupMenu.insertSeparator();

    for( IT_ParamVector itP = paramVector_.begin(); itP != paramVector_.end(); ++itP )
        (*itP)->FillRemotePopupMenu( popupMenu, context );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CheckValidity
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
bool MissionInterface_ABC::CheckValidity()
{
    bool b = true;
    for( IT_ParamVector itP = paramVector_.begin(); itP != paramVector_.end(); ++itP )
        if( ! (*itP)->IsOptional() && ! (*itP)->CheckValidity() )
            b = false;  // Don't stop here, we want to turn red the labels of all invalid params.
    return b;
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
    connect( pCancel, SIGNAL( clicked() ), &parentPanel_, SLOT( hide() ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateNatureAtlas
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateNatureAtlas( ASN1T_NatureAtlas& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamAgentType( asn, strName, this, bOptional ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateGDH
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateGDH( ASN1T_GDH& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamGDH( asn, strName, this, bOptional ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateDirection
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateDirection( ASN1T_Direction& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamDirection( asn, strName, this, bOptional ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreatePoint
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreatePoint( ASN1T_Point& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamPoint( asn, strName, strName, this, bOptional ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreatePath
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreatePath( ASN1T_Itineraire& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamPath( asn, agent_, strName, strName, this, bOptional ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreatePathList
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreatePathList( ASN1T_ListItineraire& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamPathList( asn, strName, strName, 1, 999, this, true /*bOptional*/ ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateAgentList
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateAgentList( ASN1T_ListAgent& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamAgentList( asn, strName, strName, 0, 999, this, true /*bOptional*/ ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateAgent
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateAgent( ASN1T_Agent& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamAgent( asn, strName, strName, this, bOptional ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateAutomateList
// Created: APE 2004-10-25
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateAutomateList( ASN1T_ListAutomate& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamAgentList( (ASN1T_ListAgent&)asn, strName, strName, 0, 999, this, true /*bOptional*/ ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateAutomate
// Created: APE 2004-10-25
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateAutomate( ASN1T_Agent& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamAgent( asn, strName, strName, this, bOptional ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateLocation
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateLocation( ASN1T_Localisation& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamLocation( asn, strName, strName, this, bOptional ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateLocationList
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateLocationList( ASN1T_ListLocalisation& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamLocationList( asn, strName, strName, this, true /*bOptional*/ ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateLPolygonList
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreatePolygonList( ASN1T_ListPolygon& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamLocationList( asn, strName, strName, this, true /*bOptional*/ ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreatePointList
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreatePointList( ASN1T_ListPoint& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamLocationList( asn, strName, strName, this, true /*bOptional*/ ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateAgentKnowledge
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateAgentKnowledge( ASN1T_KnowledgeAgent& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamAgentKnowledge( asn, agent_, strName, strName, this, bOptional ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateAgentKnowledgeList
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateAgentKnowledgeList( ASN1T_ListKnowledgeAgent& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamAgentKnowledgeList( asn, agent_, strName, strName, 1, 999, this, true /*bOptional*/ ) );
}



// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateObjectKnowledge
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateObjectKnowledge( ASN1T_KnowledgeObject& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamObjectKnowledge( asn, agent_, strName, strName, this, bOptional ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateObjectKnowledgeList
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateObjectKnowledgeList( ASN1T_ListKnowledgeObject& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamObjectKnowledgeList( asn, agent_, strName, strName, 1, 999, this, true /*bOptional*/ ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateGenObject
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateGenObject( ASN1T_MissionGenObject& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamObstacle( asn, strName, strName, this, bOptional ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateGenObjectList
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateGenObjectList( ASN1T_ListMissionGenObject& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamObstacleList( asn, strName, strName, this, true /*bOptional*/ ) );

}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateBool
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateBool( ASN1BOOL& asn, const std::string& strName, bool bOptional, QWidget* pParent )
{
    paramVector_.push_back( new ParamBool( asn, false, strName, (pParent != 0) ? pParent : this, bOptional ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateNumeric
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateNumeric( ASN1INT& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamNumericField( asn, 0, 9999, 0, strName, this, bOptional ) );
}


// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateNumeric
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateNumeric( ASN1REAL& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamNumericField( asn, 0.0f, 9999.0f, 0.0f, strName, this, bOptional ) );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateMaintenancePriorities
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateMaintenancePriorities( ASN1T_MaintenancePriorites& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamEquipmentList( asn, strName, this, bOptional ) );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateMedicalPriorities
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateMedicalPriorities( ASN1T_SantePriorites& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamHumanWoundList( asn, strName, this, bOptional ) );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreatePopulationKnowledge
// Created: HME 2005-12-22
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreatePopulationKnowledge( ASN1T_KnowledgePopulation& asn, const std::string& strName, bool bOptional )
{
    paramVector_.push_back( new ParamPopulationKnowledge( asn, agent_, strName, strName, this, bOptional ) );
}
