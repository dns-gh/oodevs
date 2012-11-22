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
#include "ADN_App.h"
#include "ADN_GuiBuilder.h"
#include "ADN_SearchListView.h"
#include "ADN_ListView_Disasters.h"
#include "ADN_Disasters_Data.h"
#include "ADN_ThresholdConcentrationsTable.h"
#include "ADN_ThresholdAttritionsTable.h"

//-----------------------------------------------------------------------------
// Name: ADN_Disasters_GUI constructor
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
ADN_Disasters_GUI::ADN_Disasters_GUI( ADN_Disasters_Data& data )
    : ADN_GUI_ABC( "ADN_Disasters_GUI" )
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
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vConnectors[ eName ] );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    Q3GroupBox* pConcentrationThresholds = new Q3VGroupBox( tr( "Concentration thresholds" ) );
    new ADN_ThresholdConcentrationsTable( strClassName_ + "ConcentrationThresholds", vConnectors[ eConcentrationThresholds ], pConcentrationThresholds );

    Q3GroupBox* pAttritionThresholds = new Q3VGroupBox( tr( "Attrition thresholds" ) );
    new ADN_ThresholdAttritionsTable( strClassName_ + "AttritionThresholds", vConnectors[ eAttritionThresholds ], pAttritionThresholds );

    // Content layout
    QWidget* pContent = new QWidget();
    QGridLayout* pContentLayout = new QGridLayout( pContent, 1, 2 );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder, 0, 0, 1, 2 );
    pContentLayout->addWidget( pConcentrationThresholds, 1, 0 );
    pContentLayout->addWidget( pAttritionThresholds, 1, 1 );
    pContentLayout->setRowStretch( 2, 1 );

    // ListView
    ADN_SearchListView< ADN_ListView_Disasters >* pSearchListView = new ADN_SearchListView< ADN_ListView_Disasters >( this, data_.GetDisastersInfos(), vConnectors );
    pListView_ = pSearchListView->GetListView();
    pListView_->setObjectName( strClassName_ + "_List" );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent, pSearchListView );
    pMainWidget_->setObjectName( strClassName_ );
}
