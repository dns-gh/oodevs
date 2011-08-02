// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-10 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Automata_ListView.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:14 $
// $Revision: 5 $
// $Workfile: ADN_Automata_ListView.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Automata_ListView.h"

#include <Qt3Support/q3popupmenu.h>

#include "ADN_Automata_Data.h"
#include "ADN_Automata_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Automata_Wizard.h"

typedef ADN_Automata_Data::AutomatonInfos AutomatonInfos;


// -----------------------------------------------------------------------------
// Name: ADN_Automata_ListView constructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Automata_ListView::ADN_Automata_ListView( QWidget* pParent, const char* szName, Qt::WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Automata" ) );
    setResizeMode( Q3ListView::AllColumns );

    // Connector creation
    pConnector_ = new ADN_Connector_ListView<AutomatonInfos>(*this);

    this->SetDeletionEnabled( true );
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_ListView destructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Automata_ListView::~ADN_Automata_ListView()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_ListView::ConnectItem
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
void ADN_Automata_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    AutomatonInfos* pInfos = (AutomatonInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Automata_GUI::eNbrGuiElements );

    vItemConnectors_[ADN_Automata_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Automata_GUI::eAgentType]->Connect( &pInfos->nAgentType_, bConnect );
    vItemConnectors_[ADN_Automata_GUI::eModel]->Connect( &pInfos->ptrModel_, bConnect );
    vItemConnectors_[ADN_Automata_GUI::eUnit]->Connect( &pInfos->ptrUnit_, bConnect );
    vItemConnectors_[ADN_Automata_GUI::eSubUnit]->Connect( &pInfos->vSubUnits_, bConnect );
    vItemConnectors_[ADN_Automata_GUI::eFeedbackTime]->Connect( &pInfos->strengthRatioFeedbackTime_, bConnect );
    vItemConnectors_[ADN_Automata_GUI::eHasFeedbackTime]->Connect( &pInfos->bStrengthRatioFeedbackTime_, bConnect );
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_ListView::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Automata_ListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Automata_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
