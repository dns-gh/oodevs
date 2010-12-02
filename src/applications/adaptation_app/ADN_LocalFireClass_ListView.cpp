// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_LocalFireClass_ListView.h"
#include "ADN_Connector_ListView.h"
#include "ADN_LocalFireClass_Data.h"
#include "ADN_LocalFireClass_GUI.h"
#include "ADN_LocalFireClass_Wizard.h"

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_ListView constructor
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
ADN_LocalFireClass_ListView::ADN_LocalFireClass_ListView( QWidget* pParent /*= 0*/, const char* szName /*= 0*/, WFlags f /*= 0*/ )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Local fire classes" ) );
    setResizeMode( QListView::AllColumns );
    // Connector creation
    pConnector_ = new ADN_Connector_ListView< ADN_LocalFireClass_Data::LocalFireClassInfos >(*this);
    this->SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_ListView destructor
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
ADN_LocalFireClass_ListView::~ADN_LocalFireClass_ListView()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_ListView::ConnectItem
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    ADN_LocalFireClass_Data::LocalFireClassInfos* pInfos = static_cast< ADN_LocalFireClass_Data::LocalFireClassInfos* >( pCurData_ );
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_LocalFireClass_GUI::eNbrGuiElements );
    vItemConnectors_[ ADN_LocalFireClass_GUI::eName ]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ ADN_LocalFireClass_GUI::eInitialHeat ]->Connect( &pInfos->initialHeat_, bConnect );
    vItemConnectors_[ ADN_LocalFireClass_GUI::eMaxHeat ]->Connect( &pInfos->maxHeat_, bConnect );
    vItemConnectors_[ ADN_LocalFireClass_GUI::eIncreaseRate ]->Connect( &pInfos->increaseRate_, bConnect );
    vItemConnectors_[ ADN_LocalFireClass_GUI::eDecreaseRate ]->Connect( &pInfos->decreaseRate_, bConnect );
    vItemConnectors_[ ADN_LocalFireClass_GUI::eNbrHurtU1 ]->Connect( &pInfos->injuryInfos_.nNbHurtHumans1_, bConnect );
    vItemConnectors_[ ADN_LocalFireClass_GUI::eNbrHurtU2 ]->Connect( &pInfos->injuryInfos_.nNbHurtHumans2_, bConnect );
    vItemConnectors_[ ADN_LocalFireClass_GUI::eNbrHurtU3 ]->Connect( &pInfos->injuryInfos_.nNbHurtHumans3_, bConnect );
    vItemConnectors_[ ADN_LocalFireClass_GUI::eNbrHurtUE ]->Connect( &pInfos->injuryInfos_.nNbHurtHumansE_, bConnect );
    vItemConnectors_[ ADN_LocalFireClass_GUI::eNbrDead ]->Connect( &pInfos->injuryInfos_.nNbDeadHumans_, bConnect );
    vItemConnectors_[ ADN_LocalFireClass_GUI::eUrbanAttrition ]->Connect( &pInfos->modifUrbanBlocks_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_ListView::OnContextMenu
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_ListView::OnContextMenu( const QPoint& pt )
{
    QPopupMenu popupMenu( this );
    ADN_LocalFireClass_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
