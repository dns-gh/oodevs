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
#include "ADN_ActiveProtectionsListView.h"
#include "ADN_ActiveProtections_Data.h"
#include "ADN_ActiveProtections_WeaponsTable.h"
#include "ADN_Composantes_Dotations_GUI.h"
#include "ADN_ComboBox_ActiveProtection_Dotations.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_GroupBox.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_SearchListView.h"
#include "ENT/ENT_Tr.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_GUI constructor
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
ADN_ActiveProtections_GUI::ADN_ActiveProtections_GUI( ADN_ActiveProtections_Data& data )
    : ADN_GUI_ABC( "ADN_ActiveProtections_GUI" )
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
    ADN_GuiBuilder builder;
    T_ConnectorVector vConnectors( ADN_ActiveProtections_GUI::eNbrActiveProtectionsGuiElements, (ADN_Connector_ABC*)0 );

    // Properties
    Q3GroupBox* pPropertiesGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Properties" ) );
    builder.AddField<ADN_EditLine_String>( pPropertiesGroup, tr( "Name" ), vConnectors[eActiveProtectionName] );
    ADN_EditLine_Double* pEdit = builder.AddField<ADN_EditLine_Double>( pPropertiesGroup, tr( "Coefficient" ), vConnectors[eActiveProtectionCoeffiscient], 0, eGreaterEqualZero );
    pEdit->GetValidator().setTop( 1 );
    builder.AddField<ADN_CheckBox>( pPropertiesGroup, tr( "Hard kill" ), vConnectors[eActiveProtectionHardKill] );

    // dotations
    Q3GroupBox* pDotationGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Resource" ) );
    builder.AddField< ADN_ComboBox_Vector<ADN_Equipement_Data::AmmoCategoryInfo> >( pDotationGroup, tr( "Resource" ), vConnectors[eActiveProtectionDotation] );
    builder.SetEnabled( true );
    builder.AddField<ADN_EditLine_Double>( pDotationGroup, tr( "Usage" ), vConnectors[eActiveProtectionUsage], 0, eGreaterEqualZero );

    // Weapons
    Q3GroupBox* pWeaponsGroup_ = new Q3GroupBox( 1, Qt::Horizontal, tr( "Ammunitions" ) );
    ADN_ActiveProtections_WeaponsTable* pWeapons = new ADN_ActiveProtections_WeaponsTable( tr( "Ammunitions" ).ascii(), pWeaponsGroup_ );
    vConnectors[eActiveProtectionWeapons] = &pWeapons->GetConnector();

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pPropertiesGroup );
    pContentLayout->addWidget( pDotationGroup );
    pContentLayout->addWidget( pWeaponsGroup_ );
    pContentLayout->addStretch( 1 );

    // List view
    ADN_SearchListView< ADN_ActiveProtectionsListView >* pSearchListView = new ADN_SearchListView< ADN_ActiveProtectionsListView >( data_.GetActiveProtectionsInfos(), vConnectors );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent, pSearchListView );
}
