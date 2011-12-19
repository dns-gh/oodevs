// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_FireClass_ListView.h"
#include "ADN_Connector_ListView.h"
#include "ADN_FireClass_Data.h"
#include "ADN_FireClass_GUI.h"
#include "ADN_FireClass_Wizard.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_ListView constructor
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
ADN_FireClass_ListView::ADN_FireClass_ListView( QWidget* pParent /* = 0*/, const char* szName /* = 0*/, Qt::WFlags f /* = 0*/ )
    : ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tools::translate( "ADN_FireClass_ListView", "Fires" ) );
    setResizeMode( Q3ListView::AllColumns );
    // Connector creation
    pConnector_ = new ADN_Connector_ListView< ADN_FireClass_Data::FireClassInfos >( *this );
    this->SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_ListView destructor
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
ADN_FireClass_ListView::~ADN_FireClass_ListView()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_ListView::ConnectItem
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_FireClass_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    ADN_FireClass_Data::FireClassInfos* pInfos = static_cast< ADN_FireClass_Data::FireClassInfos* >( pCurData_ );
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_FireClass_GUI::eNbrGuiElements );
    vItemConnectors_[ ADN_FireClass_GUI::eName ]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ ADN_FireClass_GUI::eInitialHeat ]->Connect( &pInfos->initialHeat_, bConnect );
    vItemConnectors_[ ADN_FireClass_GUI::eMaxHeat ]->Connect( &pInfos->maxHeat_, bConnect );
    vItemConnectors_[ ADN_FireClass_GUI::eIncreaseRate ]->Connect( &pInfos->increaseRate_, bConnect );
    vItemConnectors_[ ADN_FireClass_GUI::eDecreaseRate ]->Connect( &pInfos->decreaseRate_, bConnect );
    vItemConnectors_[ ADN_FireClass_GUI::eNbrHurtU1 ]->Connect( &pInfos->injuryInfos_.nNbHurtHumans1_, bConnect );
    vItemConnectors_[ ADN_FireClass_GUI::eNbrHurtU2 ]->Connect( &pInfos->injuryInfos_.nNbHurtHumans2_, bConnect );
    vItemConnectors_[ ADN_FireClass_GUI::eNbrHurtU3 ]->Connect( &pInfos->injuryInfos_.nNbHurtHumans3_, bConnect );
    vItemConnectors_[ ADN_FireClass_GUI::eNbrHurtUE ]->Connect( &pInfos->injuryInfos_.nNbHurtHumansE_, bConnect );
    vItemConnectors_[ ADN_FireClass_GUI::eNbrDead ]->Connect( &pInfos->injuryInfos_.nNbDeadHumans_, bConnect );
    vItemConnectors_[ ADN_FireClass_GUI::eUrbanAttrition ]->Connect( &pInfos->modifUrbanBlocks_, bConnect );
    vItemConnectors_[ ADN_FireClass_GUI::eExtinguisherAgents ]->Connect( &pInfos->agents_, bConnect );
    vItemConnectors_[ ADN_FireClass_GUI::eWeatherFireEffects ]->Connect( &pInfos->weatherEffects_, bConnect );
    vItemConnectors_[ ADN_FireClass_GUI::eSurfaceEffects ]->Connect( &pInfos->surfaceInfos_, bConnect );
    vItemConnectors_[ ADN_FireClass_GUI::eSurface ]->Connect( &pInfos->isSurface_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_ListView::OnContextMenu
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_FireClass_ListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_FireClass_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
