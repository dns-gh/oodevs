// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_FragOrderTypes.h"
#include "moc_ADN_ListView_FragOrderTypes.cpp"

#include "ADN_Missions_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tools.h"
#include "ADN_Missions_GUI.h"
#include "ADN_Models_Data.h"
#include "ADN_FragOrder_Wizard.h"

#include <Qt3Support/q3popupmenu.h>

typedef ADN_Missions_Data::FragOrder FragOrder;

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes constructor
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
ADN_ListView_FragOrderTypes::ADN_ListView_FragOrderTypes( ADN_Missions_Data::T_FragOrder_Vector& orders, QWidget* parent /* = 0*/, const char* szName /* = 0*/ )
    : ADN_ListView( parent, szName )
    , orders_( orders )
{
    addColumn( tr( "Fragmentary order" ) );
    setResizeMode( Q3ListView::AllColumns );

    pConnector_ = new ADN_Connector_ListView< FragOrder >( *this );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes destructor
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
ADN_ListView_FragOrderTypes::~ADN_ListView_FragOrderTypes()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes::ConnectItem
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
void ADN_ListView_FragOrderTypes::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    FragOrder* pInfos = (FragOrder*)pCurData_;
    vItemConnectors_[ADN_Missions_GUI::eName]                            ->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDoctrineDescription]             ->Connect( &pInfos->doctrineDescription_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eUsageDescription]                ->Connect( &pInfos->usageDescription_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eParameters]                      ->Connect( &pInfos->parameters_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eFragOrderAvailableWithoutMission]->Connect( &pInfos->isAvailableWithoutMission_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDiaType]                         ->Connect( &pInfos->diaType_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes::OnContextMenu
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
void ADN_ListView_FragOrderTypes::OnContextMenu( const QPoint& pt )
{
    if( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin )
    {
        Q3PopupMenu popupMenu( this );
        ADN_FragOrder_Wizard wizard( orders_, this );
        FillContextMenuWithDefault( popupMenu, wizard );
        popupMenu.exec( pt );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes::OnToogled
// Created: HBD 2010-09-06
// -----------------------------------------------------------------------------
void ADN_ListView_FragOrderTypes::OnToogled( bool isChecked )
{
    if( pCurData_ == 0 || isChecked )
        return;
    FragOrder* pInfos = (FragOrder*)pCurData_;
    const std::string& name = pInfos->strName_.GetData();
    ADN_Models_Data::T_ModelInfos_Vector& units = ADN_Workspace::GetWorkspace().GetModels().GetData().GetUnitModelsInfos();
    ADN_Models_Data::T_ModelInfos_Vector& automata = ADN_Workspace::GetWorkspace().GetModels().GetData().GetAutomataModelsInfos();
    ADN_Models_Data::T_ModelInfos_Vector& pops = ADN_Workspace::GetWorkspace().GetModels().GetData().GetPopulationModelsInfos();
    for( ADN_Models_Data::IT_ModelInfos_Vector it1 = units.begin(); it1 != units.end(); ++it1 )
        (*it1)->RemoveFragOder( name );
    for( ADN_Models_Data::IT_ModelInfos_Vector it1 = automata.begin(); it1 != automata.end(); ++it1 )
        (*it1)->RemoveFragOder( name );
    for( ADN_Models_Data::IT_ModelInfos_Vector it1 = pops.begin(); it1 != pops.end(); ++it1 )
        (*it1)->RemoveFragOder( name );
}
