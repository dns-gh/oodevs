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
#include "ADN_SearchListView.h"
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
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder;
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    // Propagation
    Q3GroupBox* pPropagationGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Propagation" ) );
    builder.AddField< ADN_EditLine_Double >( pPropagationGroup, tr( "Contamination distance" ), data_.rContaminationDistance_, tr( "m" ), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pPropagationGroup, tr( "Contamination quantity given" ), data_.rContaminationQuantityGiven_, 0, eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pPropagationGroup, tr( "Wind speed limit" ), data_.rWindSpeedLimitForSpreading_, tr( "km/h" ), eGreaterEqualZero );
    // NBC Suit
    Q3GroupBox* pNBCSuitGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "NBC Suit" ) );
    builder.AddField< ADN_EditLine_Double >( pNBCSuitGroup, tr( "Max speed modifier" ), data_.rNbcSuitMaxSpeedMultiplier_, 0, eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pNBCSuitGroup, tr( "Reloading time modifier" ), data_.rNbcSuitReloadSpeedMultiplier_, 0, eGreaterEqualZero );

    // Specific parameter
    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddField<ADN_EditLine_String>( pInfoHolder, tr( "Name" ), vInfosConnectors[eName] );
    // effect
    Q3HBox* effectGroup = new Q3HBox();
    effectGroup->setSpacing( 5 );
    // Liquid
    ADN_GroupBox* liquidGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Liquid" ), effectGroup );
    ADN_NBC_Intox_GUI* liquid = new ADN_NBC_Intox_GUI( liquidGroup );
    vInfosConnectors[eLiquidGroup] = &liquid->GetConnector();
    vInfosConnectors[eLiquidGroupPresent] = &liquidGroup->GetConnector();
    // Gasous
    ADN_GroupBox* gazGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Gaseous" ), effectGroup );
    vInfosConnectors[eGazGroupPresent] = &gazGroup->GetConnector();
    ADN_NBC_Intox_GUI* gaz = new ADN_NBC_Intox_GUI( gazGroup );
    vInfosConnectors[eGazGroup] = &gaz->GetConnector();
    QWidget* pHolder = builder.AddFieldHolder( gazGroup );
    // Span
    ADN_TimeField* pField = builder.AddField<ADN_TimeField>( pHolder, tr( "Span" ), vInfosConnectors[eGazLifetime] );
    pField->SetMinimumValueInSecond( 1 );
    builder.AddField<ADN_EditLine_Double>( pHolder, tr( "Spread angle" ), vInfosConnectors[eGazSpreadAngle], tr( "°" ) );
    builder.SetValidator( new ADN_DoubleValidator( 0.01, 360, 2, this ) );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // General layout
    QWidget* pGeneralContent = new QWidget();
    QHBoxLayout* pGeneralLayout = new QHBoxLayout( pGeneralContent );
    pGeneralLayout->setMargin( 0 );
    pGeneralLayout->setSpacing( 10 );
    pGeneralLayout->setAlignment( Qt::AlignLeft );
    pGeneralLayout->addWidget( pPropagationGroup );
    pGeneralLayout->addWidget( pNBCSuitGroup );

    // Specific layout
    QWidget* pSpecificContent = new QWidget();
    QVBoxLayout* pSpecificLayout = new QVBoxLayout( pSpecificContent );
    pSpecificLayout->setMargin( 10 );
    pSpecificLayout->setSpacing( 10 );
    pSpecificLayout->setAlignment( Qt::AlignTop );
    pSpecificLayout->addWidget( pInfoHolder );
    pSpecificLayout->addWidget( effectGroup );

    // List view
    ADN_SearchListView< ADN_NBC_NbcAgentListView >* pSearchListView = new ADN_SearchListView< ADN_NBC_NbcAgentListView >( data_.GetNbcAgentVector(), vInfosConnectors );

    // Sub content
    QWidget* pSubContent = CreateScrollArea( *pSpecificContent, pSearchListView, false, false, true, 0, 0 );

    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pGeneralContent );
    pContentLayout->addWidget( pSubContent, 1 );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent );
}
