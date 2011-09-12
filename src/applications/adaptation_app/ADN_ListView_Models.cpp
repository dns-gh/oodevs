//*****************************************************************************
//
// $Created: JDY 03-07-25 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Models.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:04 $
// $Revision: 7 $
// $Workfile: ADN_ListView_Models.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ListView_Models.h"
#include "ADN_App.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Workspace.h"
#include "ADN_Model_Wizard.h"
#include "ADN_Models_GUI.h"
#include "ADN_Units_Data.h"
#include "ADN_Automata_Data.h"
#include "ADN_Population_Data.h"

typedef ADN_Models_Data::ModelInfos ModelInfos;

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Models constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Models::ADN_ListView_Models( ModelInfos::E_ModelEntityType eEntityType, QWidget* pParent, const char* szName, Qt::WFlags f )
: ADN_ListView( pParent, szName, f )
, eEntityType_( eEntityType )
{
    // Add one column.
    addColumn( tools::translate( "ADN_ListView_Models", "Models" ) );
    setResizeMode( Q3ListView::AllColumns );

    // Connector creation.
    pConnector_ = new ADN_Connector_ListView<ModelInfos>( *this );

    this->SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Models destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Models::~ADN_ListView_Models()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Models::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListView_Models::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ModelInfos* pInfos = (ModelInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Models_GUI::eNbrGuiElements );

    vItemConnectors_[ADN_Models_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Models_GUI::eDiaType]->Connect( &pInfos->strDiaType_, bConnect );
    vItemConnectors_[ADN_Models_GUI::eFile]->Connect( &pInfos->strFile_, bConnect );
    vItemConnectors_[ADN_Models_GUI::eMissions]->Connect( &pInfos->vMissions_, bConnect );
    vItemConnectors_[ADN_Models_GUI::eFragOrders]->Connect( &pInfos->vFragOrders_, bConnect );
    vItemConnectors_[ADN_Models_GUI::eMasalife]->Connect( &pInfos->isMasalife_, bConnect );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Models::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_ListView_Models::OnContextMenu( const QPoint& pt )
{
    if( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin )
    {
        Q3PopupMenu popupMenu( this );
        ADN_Model_Wizard wizard( eEntityType_, this );
        FillContextMenuWithDefault( popupMenu, wizard );
        popupMenu.exec( pt );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Models::GetToolTipFor
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
std::string ADN_ListView_Models::GetToolTipFor( Q3ListViewItem& item )
{
    void* pData = static_cast<ADN_ListViewItem&>( item ).GetData();
    ModelInfos* pCastData = (ModelInfos*)pData;
    std::string strToolTip = tools::translate( "ADN_ListView_Models", "<b>Used by:</b><br>" ).ascii();
    switch( eEntityType_ )
    {
    case ModelInfos::ePawn:
        strToolTip += ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsThatUse( *pCastData );
        break;
    case ModelInfos::eAutomat:
        strToolTip += ADN_Workspace::GetWorkspace().GetAutomata().GetData().GetAutomataThatUse( *pCastData );
        break;
    case ModelInfos::ePopulation:
        strToolTip += ADN_Workspace::GetWorkspace().GetPopulation().GetData().GetPopulationsThatUse( *pCastData );
        break;
    default:
        break;
    }
    return strToolTip;
}
