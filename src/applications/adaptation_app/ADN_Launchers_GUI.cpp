// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Launchers_GUI.h"
#include "moc_ADN_Launchers_GUI.cpp"
#include "ADN_GuiBuilder.h"
#include "ADN_CommonGfx.h"
#include "ADN_Launchers_Data.h"
#include "ADN_ListView_Launchers.h"
#include "ADN_Launchers_ModifPhs_GUI.h"
#include "ADN_GroupBox.h"
#include "ADN_HtmlBuilder.h"

//-----------------------------------------------------------------------------
// Name: ADN_Launchers_GUI constructor
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
ADN_Launchers_GUI::ADN_Launchers_GUI( ADN_Launchers_Data& data )
    : ADN_GUI_ABC( eLaunchers )
    , data_( data )
    , pModifPhs_( 0 )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Launchers_GUI destructor
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
ADN_Launchers_GUI::~ADN_Launchers_GUI()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Launchers_GUI::Build
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
void ADN_Launchers_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    ADN_EditLine_ABC* nameField = builder.AddLocalizedField( pInfoHolder, "name", tr( "Name" ), vConnectors[ eName ] );
    nameField->ConnectWithRefValidity( data_.GetLaunchersInfos() );

    // Indirect fire
    builder.AddField< ADN_CheckBox >( pInfoHolder, "indirect-fire", tr( "Indirect fire" ), vConnectors[ eIndirect ] );

    // Direct fire
    ADN_GroupBox* pDirectGroup = builder.AddGroupBox( 0, "direct-fire", tr( "Direct fire" ), vConnectors[ eDirect ], 1 );
    QGroupBox* pGroupModificators = new QGroupBox( tr( "Phs modifiers" ), pDirectGroup );

    QLabel* pTargetLabel = new QLabel( tr( "Target's stance" ) );
    pTargetLabel->setAlignment( Qt::AlignHCenter );

    QLabel* pShooterLabel = new QLabel( tr( "Shooter's\nstance" ) );

    // modifiers tab
    pModifPhs_ = new ADN_Launchers_ModifPhs_GUI( builder.GetChildName( "modif-ph" ), vConnectors[ ePhModifiers ] );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Modifier layout
    QGridLayout* pModificatorsLayout = new QGridLayout( pGroupModificators, 2, 2 );
    pModificatorsLayout->setMargin( 10 );
    pModificatorsLayout->setSpacing( 10 );
    pModificatorsLayout->addWidget( pTargetLabel, 0, 1 );
    pModificatorsLayout->addWidget( pShooterLabel, 1, 0 );
    pModificatorsLayout->addWidget( pModifPhs_, 1, 1 );

    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );
    pContentLayout->addWidget( pDirectGroup );
    pContentLayout->addStretch( 1 );

    // ListView
    QWidget* pSearchListView = builder.AddSearchListView< ADN_ListView_Launchers >( this, data_.GetLaunchersInfos(), vConnectors );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent, pSearchListView );
}

// -----------------------------------------------------------------------------
// Name: ADN_Launchers_GUI::ExportPHModifiers
// Created: APE 2005-04-21
// -----------------------------------------------------------------------------
void ADN_Launchers_GUI::ExportPHModifiers( ADN_HtmlBuilder& builder, ADN_Launchers_Data::LauncherInfos& infos )
{
    if( ! infos.bDirect_.GetData() )
        return;

    pListView_->SetCurrentItem( &infos );
    builder.Section( tr( "Phs modificators" ) );
    builder.Stream() << tr( "Shooter's stance / Target's stance" ).toStdString();
    builder.CreateTableFrom( *pModifPhs_ );
}
