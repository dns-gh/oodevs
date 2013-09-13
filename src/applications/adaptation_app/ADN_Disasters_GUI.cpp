// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Disasters_GUI.h"
#include "moc_ADN_Disasters_GUI.cpp"
#include "ADN_GuiBuilder.h"
#include "ADN_ListView_Disasters.h"
#include "ADN_Disasters_Data.h"
#include "ADN_ThresholdConcentrationsTable.h"
#include "ADN_ThresholdAttritionsTable.h"
#include "ADN_NbcSuitTable.h"

//-----------------------------------------------------------------------------
// Name: ADN_Disasters_GUI constructor
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
ADN_Disasters_GUI::ADN_Disasters_GUI( ADN_Disasters_Data& data )
    : ADN_GUI_ABC( eDisasters )
    , data_( data )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Disasters_GUI destructor
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
ADN_Disasters_GUI::~ADN_Disasters_GUI()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Disasters_GUI::Build
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
void ADN_Disasters_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    ADN_EditLine_ABC* nameField = builder.AddField< ADN_EditLine_String >( pInfoHolder, "name", tr( "Name" ), vConnectors[ eName ] );
    nameField->ConnectWithRefValidity( data_.GetDisastersInfos() );
    builder.AddField< ADN_EditLine_Double >( pInfoHolder, "toxicity-exponent", tr( "Toxicity exponent" ), vConnectors[ eToxicityExponent ] );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    Q3GroupBox* pNbcSuit = new Q3VGroupBox( tr( "Protections" ) );
    new ADN_NbcSuitTable( builder.GetChildName( "nbc-suite-table" ), vConnectors[ eNbcSuitRatio ], pNbcSuit );

    Q3GroupBox* pConcentrationThresholds = new Q3VGroupBox( tr( "Concentration thresholds" ) );
    new ADN_ThresholdConcentrationsTable( builder.GetChildName( "concentration-thresholds" ), vConnectors[ eConcentrationThresholds ], pConcentrationThresholds );

    Q3GroupBox* pAttritionThresholds = new Q3VGroupBox( tr( "Attrition thresholds" ) );
    new ADN_ThresholdAttritionsTable( builder.GetChildName( "attrition-thresholds" ), vConnectors[ eAttritionThresholds ], pAttritionThresholds );

    // Content layout
    QWidget* pContent = new QWidget();
    QGridLayout* pContentLayout = new QGridLayout( pContent, 1, 2 );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder, 0, 0, 1, 2 );
    pContentLayout->addWidget( pNbcSuit, 1, 0 );
    pContentLayout->addWidget( pConcentrationThresholds, 1, 1 );
    pContentLayout->addWidget( pAttritionThresholds, 2, 0, 1, 2 );
    pContentLayout->setRowStretch( 3, 1 );

    // ListView
    QWidget* pSearchListView = builder.AddSearchListView< ADN_ListView_Disasters >( this, data_.GetDisastersInfos(), vConnectors );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent, pSearchListView );
}
