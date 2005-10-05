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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MissionInterface_ABC.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 18:36 $
// $Revision: 11 $
// $Workfile: MOS_MissionInterface_ABC.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_MissionInterface_ABC.h"
#include "moc_MOS_MissionInterface_ABC.cpp"

#include "MOS_MissionPanel.h"
#include "MOS_ParamPoint.h"
#include "MOS_ParamAgent.h"
#include "MOS_ParamAgentList.h"
#include "MOS_ParamPath.h"
#include "MOS_ParamPathList.h"
#include "MOS_ParamGDH.h"
#include "MOS_ParamComboBox.h"
#include "MOS_ParamBool.h"
#include "MOS_ParamSlider.h"
#include "MOS_ParamLocation.h"
#include "MOS_ParamLocationList.h"
#include "MOS_ParamAgentType.h"
#include "MOS_ParamAgentKnowledge.h"
#include "MOS_ParamAgentKnowledgeList.h"
#include "MOS_ParamObjectKnowledge.h"
#include "MOS_ParamObjectKnowledgeList.h"
#include "MOS_ParamObstacle.h"
#include "MOS_ParamObstacleList.h"
#include "MOS_ParamDirection.h"
#include "MOS_ParamNumericField.h"
#include "MOS_ParamEquipmentList.h"
#include "MOS_ParamHumanWoundList.h"

// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC constructor
/** @param  mission 
    @param  pParent 
*/
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MOS_MissionInterface_ABC::MOS_MissionInterface_ABC( MOS_Agent_ABC& agent, MOS_MissionPanel& parentPanel )
    : QVBox         ( &parentPanel )
    , agent_        ( agent )
    , parentPanel_  ( parentPanel )
{
    this->setMargin( 5 );
    this->setSpacing( 4 );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MOS_MissionInterface_ABC::~MOS_MissionInterface_ABC()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::FillRemotePopupMenu
/** @param  popupMenu 
    @param  context 
*/
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if( popupMenu.count() > 0 )
        popupMenu.insertSeparator();

    for( IT_ParamVector itP = paramVector_.begin(); itP != paramVector_.end(); ++itP )
        (*itP)->FillRemotePopupMenu( popupMenu, context );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CheckValidity
/** @return 
*/
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
bool MOS_MissionInterface_ABC::CheckValidity()
{
    bool b = true;
    for( IT_ParamVector itP = paramVector_.begin(); itP != paramVector_.end(); ++itP )
        if( ! (*itP)->CheckValidity() )
            b = false;  // Don't stop here, we want to turn red the labels of all invalid params.
    return b;
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateOkCancelButtons
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateOkCancelButtons()
{
    QHBox* pBox = new QHBox( this );
    QPushButton* pOk = new QPushButton( tr( "Valider" ), pBox );
    QPushButton* pCancel = new QPushButton( tr( "Annuler" ), pBox );
    pOk->setDefault( true );

    connect( pOk, SIGNAL( clicked() ), this, SLOT( OnOk() ) );
    connect( pCancel, SIGNAL( clicked() ), &parentPanel_, SLOT( hide() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateNatureAtlas
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateNatureAtlas( ASN1T_NatureAtlas& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamAgentType( asn, strName, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateGDH
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateGDH( ASN1T_GDH& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamGDH( asn, strName, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateDirection
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateDirection( ASN1T_Direction& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamDirection( asn, strName, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreatePoint
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreatePoint( ASN1T_Point& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamPoint( asn, strName, strName, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreatePath
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreatePath( ASN1T_Itineraire& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamPath( asn, agent_, strName, strName, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreatePathList
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreatePathList( ASN1T_ListItineraire& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamPathList( asn, strName, strName, 1, 999, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateAgentList
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateAgentList( ASN1T_ListAgent& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamAgentList( asn, strName, strName, 0, 999, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateAgent
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateAgent( ASN1T_Agent& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamAgent( asn, strName, strName, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateAutomateList
/** @param  asn 
    @param  strName 
    */
// Created: APE 2004-10-25
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateAutomateList( ASN1T_ListAutomate& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamAgentList( (ASN1T_ListAgent&)asn, strName, strName, 0, 999, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateAutomate
/** @param  asn 
    @param  strName 
    */
// Created: APE 2004-10-25
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateAutomate( ASN1T_Agent& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamAgent( asn, strName, strName, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateLocation
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateLocation( ASN1T_Localisation& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamLocation( asn, strName, strName, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateLocationList
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateLocationList( ASN1T_ListLocalisation& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamLocationList( asn, strName, strName, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateLPolygonList
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreatePolygonList( ASN1T_ListPolygon& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamLocationList( asn, strName, strName, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreatePointList
/** @param  asn 
    @param  strName 
    */
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreatePointList( ASN1T_ListPoint& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamLocationList( asn, strName, strName, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateAgentKnowledge
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateAgentKnowledge( ASN1T_KnowledgeAgent& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamAgentKnowledge( asn, agent_, strName, strName, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateAgentKnowledgeList
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateAgentKnowledgeList( ASN1T_ListKnowledgeAgent& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamAgentKnowledgeList( asn, agent_, strName, strName, 1, 999, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateObjectKnowledge
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateObjectKnowledge( ASN1T_KnowledgeObject& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamObjectKnowledge( asn, agent_, strName, strName, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateObjectKnowledgeList
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateObjectKnowledgeList( ASN1T_ListKnowledgeObject& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamObjectKnowledgeList( asn, agent_, strName, strName, 1, 999, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateGenObject
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateGenObject( ASN1T_MissionGenObject& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamObstacle( asn, strName, strName, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateGenObjectList
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateGenObjectList( ASN1T_ListMissionGenObject& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamObstacleList( asn, strName, strName, this ) );

}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateBool
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateBool( ASN1BOOL& asn, const std::string& strName, QWidget* pParent )
{
    paramVector_.push_back( new MOS_ParamBool( asn, false, strName, (pParent != 0) ? pParent : this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateNumeric
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateNumeric( ASN1INT& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamNumericField( asn, 0, 9999, 0, strName, this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateNumeric
/** @param  asn 
    @param  strName 
*/
// Created: APE 2004-04-30
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateNumeric( ASN1REAL& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamNumericField( asn, 0.0f, 9999.0f, 0.0f, strName, this ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateMaintenancePriorities
/** @param  asn 
    @param  strName 
*/
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateMaintenancePriorities( ASN1T_MaintenancePriorites& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamEquipmentList( asn, strName, this ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionInterface_ABC::CreateMedicalPriorities
/** @param  asn 
    @param  strName 
*/
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
void MOS_MissionInterface_ABC::CreateMedicalPriorities( ASN1T_SantePriorites& asn, const std::string& strName )
{
    paramVector_.push_back( new MOS_ParamHumanWoundList( asn, strName, this ) );
}
