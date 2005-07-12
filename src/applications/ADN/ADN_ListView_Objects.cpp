//*****************************************************************************
//
// $Created: JDY 03-07-03 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_ListView_Objects.cpp $
// $Author: Nld $
// $Modtime: 29/04/05 16:39 $
// $Revision: 11 $
// $Workfile: ADN_ListView_Objects.cpp $
//
//*****************************************************************************
#include "ADN_Pch.h"
#include "ADN_ListView_Objects.h"

#include <qpopupmenu.h>

#include "ADN_Objects_Data.h"
#include "ADN_Objects_GUI.h"
#include "ADN_Connector_ListView.h"


typedef ADN_Objects_Data::ObjectInfos ObjectInfos;


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Objects constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Objects::ADN_ListView_Objects( QWidget* pParent, const char* szName, WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Objects" ) );
    setResizeMode(QListView::AllColumns);
    
    // Connector creation
    pConnector_=new ADN_Connector_ListView<ObjectInfos>(*this);
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Objects destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Objects::~ADN_ListView_Objects()
{
    delete pConnector_; 
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Objects::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListView_Objects::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ObjectInfos* pInfos = (ObjectInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Objects_GUI::eNbrGuiElements );
    
    vItemConnectors_[ADN_Objects_GUI::eName]->Connect(&pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eCanBeBypassed]->Connect(&pInfos->bCanBeBypassed_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eAvoidDistance]->Connect(&pInfos->rAvoidDistance_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eDefaultSpeed]->Connect(&pInfos->rDefaultSpeed_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eDefaultBypassSpeed]->Connect(&pInfos->rDefaultBypassSpeed_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eMaxInteractionHeight]->Connect(&pInfos->rMaxInteractionHeight_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eDefaultConsumption]->Connect(&pInfos->nDefaultConsumption_, bConnect );

    vItemConnectors_[ADN_Objects_GUI::eMaxAgentSpeedPercentage]->Connect(&pInfos->rMaxAgentSpeedPercentage_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eMaxNbrUsers]->Connect(&pInfos->nMaxNbrUsers_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eToBuild]->Connect(&pInfos->ptrToBuild_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eToReinforce]->Connect(&pInfos->ptrToReinforce_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eLocationScores]->Connect(&pInfos->vScoreLocation_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eAttritions]->Connect(&pInfos->attritions_, bConnect );

    vItemConnectors_[ADN_Objects_GUI::eSpeedImpact]->Connect(&pInfos->nSpeedImpact_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eHasToBuild]->Connect( &pInfos->bToBuild_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eHasToReinforce]->Connect( &pInfos->bToReinforce_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eHasAttritions]->Connect( &pInfos->bAttritions_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eCanBePrepared]->Connect( &pInfos->bCanBePrepared_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eCanBeValorized]->Connect( &pInfos->bCanBeValorized_, bConnect );
}
