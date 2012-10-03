//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Categories_Size.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:45 $
// $Revision: 6 $
// $Workfile: ADN_ListView_Categories_Size.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ListView_Urban_Type.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Urban_Data.h"
#include "ADN_Urban_GUI.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"

typedef ADN_Urban_Data::UrbanInfos UrbanInfos;

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Urban_Type constructor
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
ADN_ListView_Urban_Type::ADN_ListView_Urban_Type( QWidget * parent )
    : ADN_ListView( parent, "Facade", tools::translate( "ADN_ListView_Urban_Type", "Facade" ) )
{
    pConnector_ = new ADN_Connector_ListView< UrbanInfos >( *this );
    this->SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Urban_Type destructor
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
ADN_ListView_Urban_Type::~ADN_ListView_Urban_Type()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Urban_Type::ConnectItem
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
void ADN_ListView_Urban_Type::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    UrbanInfos* pInfos = ( UrbanInfos* )pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Urban_GUI::eNbrUrbanGuiElements );

    vItemConnectors_[ADN_Urban_GUI::eUrbanName]->Connect( &pInfos->strName_, bConnect );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Urban_Type::OnContextMenu
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
void  ADN_ListView_Urban_Type::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< UrbanInfos > wizard( tools::translate( "ADN_ListView_Urban_Type", "Facades" ), ADN_Workspace::GetWorkspace().GetUrban().GetData().GetFacadesInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}