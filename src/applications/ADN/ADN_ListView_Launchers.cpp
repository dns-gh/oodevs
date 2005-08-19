//*****************************************************************************
//
// $Created: JDY 03-07-15 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Launchers.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:04 $
// $Revision: 7 $
// $Workfile: ADN_ListView_Launchers.cpp $
//
//*****************************************************************************
#include "ADN_pch.h"
#include "ADN_ListView_Launchers.h"
#include "moc_ADN_ListView_Launchers.cpp"

#include <qpopupmenu.h>

#include "ADN_Connector_ListView.h"
#include "ADN_Launcher_Wizard.h"
#include "ADN_Tools.h"
#include "ADN_Launchers_GUI.h"


typedef ADN_Launchers_Data::LauncherInfos LauncherInfos;


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Launchers constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Launchers::ADN_ListView_Launchers( QWidget* pParent, const char* szName, WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Launchers" ) );
    setResizeMode( QListView::AllColumns );

    // Connector creation.
    pConnector_ = new ADN_Connector_ListView<LauncherInfos>( *this );

    this->SetDeletionEnabled( true );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Launchers destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Launchers::~ADN_ListView_Launchers()
{
    delete pConnector_;
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Launchers::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListView_Launchers::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    LauncherInfos* pInfos = (LauncherInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Launchers_GUI::eNbrGuiElements );

    vItemConnectors_[ADN_Launchers_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Launchers_GUI::ePhModifiers]->Connect( &pInfos->tabModifs_, bConnect );

    // Connect those at the end so that the items in the associated group boxes
    // are correctly enabled / disabled according to those values.
    vItemConnectors_[ADN_Launchers_GUI::eDirect]->Connect( &pInfos->bDirect_, bConnect );
    vItemConnectors_[ADN_Launchers_GUI::eIndirect]->Connect( &pInfos->bIndirect_, bConnect );
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView_Launchers::OnContextMenu
// Created: APE 2005-01-28
// -----------------------------------------------------------------------------
void ADN_ListView_Launchers::OnContextMenu( const QPoint& pt )
{
    QPopupMenu popupMenu( this );
    ADN_Launcher_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
