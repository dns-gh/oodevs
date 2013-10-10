// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Fires_ListView.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Fires_Data.h"
#include "ADN_Fires_GUI.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"
#include "ADN_WorkspaceElement.h"

// -----------------------------------------------------------------------------
// Name: ADN_Fires_ListView constructor
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
ADN_Fires_ListView::ADN_Fires_ListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_Fires_ListView", ADN_Tr::ConvertFromWorkspaceElement( eFires ).c_str() )
{
    // Connector creation
    pConnector_ = new ADN_Connector_ListView< ADN_Fires_Data::FireClassInfos >( *this );
    this->SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Fires_ListView destructor
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
ADN_Fires_ListView::~ADN_Fires_ListView()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Fires_ListView::ConnectItem
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_Fires_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    ADN_Fires_Data::FireClassInfos* pInfos = static_cast< ADN_Fires_Data::FireClassInfos* >( pCurData_ );
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Fires_GUI::eNbrGuiElements );
    vItemConnectors_[ ADN_Fires_GUI::eName ]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ ADN_Fires_GUI::eInitialHeat ]->Connect( &pInfos->initialHeat_, bConnect );
    vItemConnectors_[ ADN_Fires_GUI::eMaxHeat ]->Connect( &pInfos->maxHeat_, bConnect );
    vItemConnectors_[ ADN_Fires_GUI::eIncreaseRate ]->Connect( &pInfos->increaseRate_, bConnect );
    vItemConnectors_[ ADN_Fires_GUI::eDecreaseRate ]->Connect( &pInfos->decreaseRate_, bConnect );
    vItemConnectors_[ ADN_Fires_GUI::eNbrHurtU1 ]->Connect( &pInfos->injuryInfos_.nNbHurtHumans1_, bConnect );
    vItemConnectors_[ ADN_Fires_GUI::eNbrHurtU2 ]->Connect( &pInfos->injuryInfos_.nNbHurtHumans2_, bConnect );
    vItemConnectors_[ ADN_Fires_GUI::eNbrHurtU3 ]->Connect( &pInfos->injuryInfos_.nNbHurtHumans3_, bConnect );
    vItemConnectors_[ ADN_Fires_GUI::eNbrHurtUE ]->Connect( &pInfos->injuryInfos_.nNbHurtHumansE_, bConnect );
    vItemConnectors_[ ADN_Fires_GUI::eNbrDead ]->Connect( &pInfos->injuryInfos_.nNbDeadHumans_, bConnect );
    vItemConnectors_[ ADN_Fires_GUI::eUrbanAttrition ]->Connect( &pInfos->modifUrbanBlocks_, bConnect );
    vItemConnectors_[ ADN_Fires_GUI::eExtinguisherAgents ]->Connect( &pInfos->agents_, bConnect );
    vItemConnectors_[ ADN_Fires_GUI::eWeatherFireEffects ]->Connect( &pInfos->weatherEffects_, bConnect );
    vItemConnectors_[ ADN_Fires_GUI::eSurfaceEffects ]->Connect( &pInfos->surfaceInfos_, bConnect );
    vItemConnectors_[ ADN_Fires_GUI::eSurface ]->Connect( &pInfos->isSurface_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_Fires_ListView::OnContextMenu
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_Fires_ListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< ADN_Fires_Data::FireClassInfos > wizard( ADN_Tr::ConvertFromWorkspaceElement( eFires ).c_str(), ADN_Workspace::GetWorkspace().GetFires().GetData().GetFireClassesInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
