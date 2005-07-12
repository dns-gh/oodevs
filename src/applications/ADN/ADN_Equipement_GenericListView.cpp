// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-29 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_GenericListView.cpp $
// $Author: Ape $
// $Modtime: 21/04/05 11:06 $
// $Revision: 10 $
// $Workfile: ADN_Equipement_GenericListView.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Equipement_GenericListView.h"

#include <qpopmenu.h>

#include "ADN_Equipement_Data.h"
#include "ADN_Equipement_GUI.h"
#include "ADN_Enums.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Equipement_Wizard.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Equipement_Data::DotationInfos DotationInfos;
typedef ADN_Equipement_Data::CategoryInfo CategoryInfo;


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GenericListView constructor
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Equipement_GenericListView::ADN_Equipement_GenericListView( E_TypeDotation nType, QWidget* pParent, const char* szName, WFlags f )
: ADN_ListView( pParent, szName, f )
, nType_      ( nType )
{
    // Add one column
    std::string strName = ENT_Tr::ConvertFromTypeDotation( nType, ENT_Tr::eToTr );
    addColumn( strName.c_str() );
    setResizeMode( QListView::AllColumns );

    // Connector creation
    pConnector_ = new ADN_Connector_ListView<CategoryInfo>( *this );

    this->SetDeletionEnabled( true );
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GenericListView destructor
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Equipement_GenericListView::~ADN_Equipement_GenericListView()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GenericListView::ConnectItem
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Equipement_GenericListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    CategoryInfo* pInfos = (CategoryInfo*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Equipement_GUI::eNbrGenericGuiElements );

    vItemConnectors_[ADN_Equipement_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eNbrInUnitVolume]->Connect( &pInfos->rNbrInUnitVolume_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eGenEMAT6Code]->Connect( &pInfos->strCodeEMAT6_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eGenEMAT8Code]->Connect( &pInfos->strCodeEMAT8_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eGenLFRILCode]->Connect( &pInfos->strCodeLFRIL_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eGenNNOCode]->Connect( &pInfos->strCodeNNO_, bConnect );
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GenericListView::OnContextMenu
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Equipement_GenericListView::OnContextMenu( const QPoint& pt )
{
    QPopupMenu popupMenu( this );
    DotationInfos& dotation = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( nType_ );
    ADN_Equipement_Wizard wizard( dotation, this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
