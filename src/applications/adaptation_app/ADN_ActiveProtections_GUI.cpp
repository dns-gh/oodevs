// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ActiveProtections_GUI.h"
#include "moc_ADN_ActiveProtections_GUI.cpp"
#include "ADN_ActiveProtectionsListView.h"
#include "ADN_ActiveProtections_Data.h"
#include "ADN_ActiveProtections_WeaponsTable.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_GoToButton.h"
#include "ADN_GroupBox.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ENT/ENT_Tr.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_GUI constructor
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
ADN_ActiveProtections_GUI::ADN_ActiveProtections_GUI( ADN_ActiveProtections_Data& data )
    : ADN_GUI_ABC( eActiveProtections )
    , data_( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_GUI::Build
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vConnectors( ADN_ActiveProtections_GUI::eNbrActiveProtectionsGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Properties
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddLocalizedField( data_.GetActiveProtectionsInfos(), pInfoHolder, "name", tr( "Name" ), vConnectors[eActiveProtectionName] );
    ADN_EditLine_Double* pEdit = builder.AddField< ADN_EditLine_Double >( pInfoHolder, "coefficient", tr( "Coefficient" ), vConnectors[eActiveProtectionCoeffiscient], 0, eGreaterEqualZero );
    pEdit->GetValidator().setTop( 1 );
    builder.AddField< ADN_CheckBox >( pInfoHolder, "hard-kill", tr( "Hard kill" ), vConnectors[eActiveProtectionHardKill] );

    // dotations
    Q3GroupBox* pDotationGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Supplies" ) );
    QWidget* pHolder = new QWidget( pDotationGroup );
    new Q3GridLayout( pHolder, 0, 3, 5, 5 );
    ADN_GoToButton* goToButton = new ADN_GoToButton( ::eSupplies );
    goToButton->SetLinkedCombo( builder.AddField< ADN_ComboBox_Vector >( pHolder, "resource", tr( "Supplies" ), vConnectors[eActiveProtectionDotation], 0, eNone, goToButton ) );
    builder.SetEnabled( true );
    builder.AddField< ADN_EditLine_Double >( pHolder, "usage", tr( "Usage" ), vConnectors[eActiveProtectionUsage], 0, eGreaterEqualZero );

    // Weapons
    Q3GroupBox* pWeaponsGroup_ = new Q3GroupBox( 1, Qt::Horizontal, tr( "Ammunitions" ) );
    ADN_ActiveProtections_WeaponsTable* pWeapons = new ADN_ActiveProtections_WeaponsTable( tr( "Ammunitions" ), builder.GetChildName( "ammunitions-table" ), vConnectors[ eActiveProtectionWeapons ], pWeaponsGroup_ );
    pWeapons->SetGoToOnDoubleClick( ::eSupplies );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );
    pContentLayout->addWidget( pDotationGroup );
    pContentLayout->addWidget( pWeaponsGroup_ );

    // List view
    QWidget* pSearchListView = builder.AddSearchListView< ADN_ActiveProtectionsListView >( this, data_.GetActiveProtectionsInfos(), vConnectors );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent, pSearchListView );
}
