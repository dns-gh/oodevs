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
#include "ADN_NBC_Data.h"
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
ADN_NBC_GUI::ADN_NBC_GUI( ADN_NBC_Data& data )
    : ADN_GUI_ABC( eNBC )
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
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Propagation
    Q3GroupBox* pPropagationGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Propagation" ) );
    builder.AddField< ADN_EditLine_Double >( pPropagationGroup, "contamination-distance", tr( "Contamination distance" ), data_.rContaminationDistance_, tr( "m" ), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pPropagationGroup, "contamination-quantity", tr( "Contamination quantity given" ), data_.rContaminationQuantityGiven_, 0, eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pPropagationGroup, "wind-speed-limit", tr( "Wind speed limit" ), data_.rWindSpeedLimitForSpreading_, tr( "km/h" ), eGreaterEqualZero );
    // NBC Suit
    Q3GroupBox* pNBCSuitGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "NBC Suit" ) );
    builder.AddField< ADN_EditLine_Double >( pNBCSuitGroup, "max-speed-modifier", tr( "Max speed modifier" ), data_.rNbcSuitMaxSpeedMultiplier_, 0, eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pNBCSuitGroup, "reloading-time-modifier", tr( "Reloading time modifier" ), data_.rNbcSuitReloadSpeedMultiplier_, 0, eGreaterEqualZero );

    // Specific parameter
    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );

    ADN_EditLine_ABC* nameField = builder.AddLocalizedField( pInfoHolder, "name", tr( "Name" ), vInfosConnectors[eName] );
    nameField->ConnectWithRefValidity( data_.GetNbcAgentVector() );

    // effect
    Q3HBox* effectGroup = new Q3HBox();
    effectGroup->setSpacing( 5 );
    // Liquid
    ADN_GroupBox* liquidGroup = builder.AddGroupBox( effectGroup, "liquid", tr( "Liquid" ), vInfosConnectors[ eLiquidGroupPresent ], 3 );
    ADN_NBC_Intox_GUI* liquid = new ADN_NBC_Intox_GUI( liquidGroup, builder.GetChildName( "liquid-intox" ) );
    vInfosConnectors[ eLiquidGroup ] = &liquid->GetConnector();
    // Gaseous
    ADN_GroupBox* gazGroup = builder.AddGroupBox( effectGroup, "gaz", tr( "Gaseous" ), vInfosConnectors[ eGazGroupPresent ], 1 );
    ADN_NBC_Intox_GUI* gaz = new ADN_NBC_Intox_GUI( gazGroup, builder.GetChildName( "gaz-intox" ) );
    vInfosConnectors[ eGazGroup ] = &gaz->GetConnector();
    // Span
    QWidget* pHolder = builder.AddFieldHolder( gazGroup );
    ADN_TimeField* pField = builder.AddField< ADN_TimeField >( pHolder, "span", tr( "Span" ), vInfosConnectors[eGazLifetime] );
    pField->SetMinimumValueInSecond( 1 );
    builder.AddField< ADN_EditLine_Double >( pHolder, "spread-angle", tr( "Spread angle" ), vInfosConnectors[eGazSpreadAngle], tr( "°" ) );
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
    QWidget* pSearchListView = builder.AddSearchListView< ADN_NBC_NbcAgentListView >( this, data_.GetNbcAgentVector(), vInfosConnectors );

    // Sub content
    QWidget* pSubContent = CreateScrollArea( builder.GetChildName( "content" ), *pSpecificContent, pSearchListView, false, false, true, 0, 0 );

    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pGeneralContent );
    pContentLayout->addWidget( pSubContent, 1 );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent );
}
