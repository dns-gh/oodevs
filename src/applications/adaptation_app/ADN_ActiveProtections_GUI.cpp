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
    // Create the top widget.
    if( pMainWidget_ != 0 )
        return;

    ADN_GuiBuilder builder;

    // Create the top widget.
    pMainWidget_ = new QWidget( 0, "Active Protection" );

    // Create the active protection listview.
    ADN_ActiveProtectionsListView* pActiveProtectionListView = new ADN_ActiveProtectionsListView( pMainWidget_ );
    pActiveProtectionListView->GetConnector().Connect( &data_.GetActiveProtectionsInfos() );
    T_ConnectorVector vConnectors( ADN_ActiveProtections_GUI::eNbrActiveProtectionsGuiElements, (ADN_Connector_ABC*)0 );

    Q3GroupBox* pGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Active Protection" ), pMainWidget_ );

    Q3GroupBox* pPropertiesGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Properties" ), pGroup );

    builder.AddField<ADN_EditLine_String>( pPropertiesGroup, tr( "Name" ), vConnectors[eActiveProtectionName] );
    ADN_EditLine_Double* pEdit = builder.AddField<ADN_EditLine_Double>( pPropertiesGroup, tr( "Coefficient" ), vConnectors[eActiveProtectionCoeffiscient], 0, eGreaterEqualZero );
    pEdit->GetValidator().setTop( 1 );
    builder.AddField<ADN_CheckBox>( pPropertiesGroup, tr( "Hard kill" ), vConnectors[eActiveProtectionHardKill] );

    // dotations
    Q3GroupBox* pDotationGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Resource" ), pGroup );

    builder.AddField< ADN_ComboBox_Vector<ADN_Equipement_Data::AmmoCategoryInfo> >( pDotationGroup, tr( "Resource" ), vConnectors[eActiveProtectionDotation] );
    builder.SetEnabled( true );
    builder.AddField<ADN_EditLine_Double>( pDotationGroup, tr( "Usage" ), vConnectors[eActiveProtectionUsage], 0, eGreaterEqualZero );

    // Weapons
    Q3GroupBox* pWeaponsGroup_ = new Q3GroupBox( 1, Qt::Horizontal, tr( "Ammunitions" ), pGroup );

    ADN_ActiveProtections_WeaponsTable* pWeapons = new ADN_ActiveProtections_WeaponsTable( tr( "Ammunitions" ).ascii(), pWeaponsGroup_ );
    vConnectors[eActiveProtectionWeapons] = &pWeapons->GetConnector();

    //Connect
    pActiveProtectionListView->SetItemConnectors(vConnectors);

     // Layout
    Q3HBoxLayout* pMainLayout = new Q3HBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pActiveProtectionListView, 1 );
    pMainLayout->addWidget( pGroup, 3 );
}
