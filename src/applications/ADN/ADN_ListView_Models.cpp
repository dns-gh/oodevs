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
#include "ADN_Pch.h"
#include "ADN_ListView_Models.h"

#include "ADN_App.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Workspace.h"
#include "ADN_Model_Wizard.h"
#include "ADN_Models_GUI.h"

#include <qheader.h>
#include <qpopmenu.h>
#include <qmessagebox.h>

typedef ADN_Models_Data::ModelInfos ModelInfos;


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Models constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Models::ADN_ListView_Models( bool bForAutomata, QWidget* pParent, const char* szName, WFlags f )
: ADN_ListView  ( pParent, szName, f )
, bForAutomata_ ( bForAutomata )
{
    // Add one column.
    addColumn( tr( "Models" ) );
    setResizeMode( QListView::AllColumns );

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
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Models::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_ListView_Models::OnContextMenu( const QPoint& pt )
{
    QPopupMenu popupMenu( this );
    ADN_Model_Wizard wizard( bForAutomata_, this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
