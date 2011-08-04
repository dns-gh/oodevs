// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_NBC_GUI.h"
#include "moc_ADN_NBC_GUI.cpp"
#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_NBC_Datas.h"
#include "ADN_CommonGfx.h"
#include "ADN_NBC_NbcAgentListView.h"
#include "ADN_NBC_Intox_GUI.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Validator.h"
#include "ADN_TimeField.h"
#include "ADN_GroupBox.h"

// -----------------------------------------------------------------------------
// Name: ADN_NBC_GUI constructor
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_GUI::ADN_NBC_GUI( ADN_NBC_Datas& data )
    : ADN_GUI_ABC( "ADN_NBC_GUI" )
    , data_( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_GUI destructor
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_GUI::~ADN_NBC_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_GUI::Build
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
void ADN_NBC_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );

    // NBC agents list
    ADN_NBC_NbcAgentListView* pNBCAgentListView = new ADN_NBC_NbcAgentListView(pMainWidget_);
    pNBCAgentListView->GetConnector().Connect( &data_.GetNbcAgentVector() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    Q3GroupBox* pPropagationGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Propagation" ), pMainWidget_ );
    builder.AddField< ADN_EditLine_Double >( pPropagationGroup, tr( "Contamination distance" ), data_.rContaminationDistance_, tr( "m" ), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pPropagationGroup, tr( "Contamination quantity given" ), data_.rContaminationQuantityGiven_, 0, eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pPropagationGroup, tr( "Wind speed limit" ), data_.rWindSpeedLimitForSpreading_, tr( "km/h" ), eGreaterEqualZero );

    Q3GroupBox* pNBCSuitGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "NBC Suit" ), pMainWidget_ );
    builder.AddField< ADN_EditLine_Double >( pNBCSuitGroup, tr( "Max speed modifier" ), data_.rNbcSuitMaxSpeedMultiplier_, 0, eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pNBCSuitGroup, tr( "Reloading time modifier" ), data_.rNbcSuitReloadSpeedMultiplier_, 0, eGreaterEqualZero );

    // NBC agent data
    Q3GroupBox* pGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "NBC Agent" ), pMainWidget_ );

    QWidget* pHolder = builder.AddFieldHolder( pGroup );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "Name" ), vInfosConnectors[eName] );

    Q3HBox* hBox = new Q3HBox( pGroup );
    hBox->setSpacing( 5 );

    ADN_GroupBox* liquidGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Liquid" ), hBox );
    ADN_NBC_Intox_GUI* liquid = new ADN_NBC_Intox_GUI( liquidGroup );
    vInfosConnectors[eLiquidGroup] = &liquid->GetConnector();
    vInfosConnectors[eLiquidGroupPresent] = &liquidGroup->GetConnector();
    ADN_GroupBox* gazGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Gaseous" ), hBox );
    vInfosConnectors[eGazGroupPresent] = &gazGroup->GetConnector();

    ADN_NBC_Intox_GUI* gaz = new ADN_NBC_Intox_GUI( gazGroup );
    vInfosConnectors[eGazGroup] = &gaz->GetConnector();
    pHolder = builder.AddFieldHolder( gazGroup );
    builder.AddField<ADN_TimeField>( pHolder, tr( "Span" ), vInfosConnectors[eGazLifetime] );
    builder.AddField<ADN_EditLine_Double>( pHolder, tr( "Spread angle" ), vInfosConnectors[eGazSpreadAngle], tr( "°" ) );
    builder.SetValidator( new ADN_DoubleValidator( 0.01, 360, 2, this ) );

    pNBCAgentListView->SetItemConnectors( vInfosConnectors );

    // Main vertical layout
    Q3VBoxLayout* pMainLayout = new Q3VBoxLayout( pMainWidget_, 10, 10 );

    Q3HBoxLayout* pLayout = new Q3HBoxLayout();
    pLayout->addWidget( pPropagationGroup );
    pLayout->addWidget( pNBCSuitGroup );

    // Top horizontal layout
    Q3BoxLayout* pListAndDataLayout = new Q3HBoxLayout( 0, 10, 10 );
    pListAndDataLayout->addWidget( pNBCAgentListView, 1 );
    pListAndDataLayout->addWidget( pGroup, 4 );

    pMainLayout->addLayout( pLayout );
    pMainLayout->addLayout( pListAndDataLayout, 2 );
}
