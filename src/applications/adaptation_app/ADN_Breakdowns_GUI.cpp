// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Breakdowns_GUI.h"

#include "ADN_GuiBuilder.h"
#include "ADN_Breakdowns_Data.h"
#include "ADN_Breakdowns_ListView.h"
#include "ADN_Breakdowns_PartsTable.h"
#include "ADN_EditLine.h"
#include "ADN_TimeField.h"
#include "ADN_SearchListView.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_GUI constructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Breakdowns_GUI::ADN_Breakdowns_GUI( ADN_Breakdowns_Data& data )
    : ADN_GUI_ABC( "ADN_Breakdowns_GUI" )
    , data_      ( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_GUI destructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Breakdowns_GUI::~ADN_Breakdowns_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_GUI::Build
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Breakdowns_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vInfosConnectors( eNbrBreakdownGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // General
    Q3GroupBox* pGeneralGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "General parameters" ) );
    //QWidget* pHolder2 = builder.AddFieldHolder( pGeneralGroup );
    builder.AddField< ADN_TimeField >( pGeneralGroup, tr( "Average diagnostic duration" ), data_.strAverageDiagnosticTime_ );

    // Specific parameter
    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vInfosConnectors[eName] );
    builder.AddEnumField( pInfoHolder, tr( "Type" ), vInfosConnectors[eType] );
    builder.AddEnumField( pInfoHolder, tr( "Seriousness" ), vInfosConnectors[eNTI] );
    builder.AddField< ADN_TimeField >( pInfoHolder, tr( "Repair duration" ), vInfosConnectors[eRepairTime] );
    builder.AddField< ADN_TimeField >( pInfoHolder, tr( "Repair duration variance" ), vInfosConnectors[eRepairTimeVariance] );

    // Parts
    QGroupBox* pPartsGroup = new QGroupBox( tr( "Required parts" ) );
    QVBoxLayout* pPartsLayout = new QVBoxLayout( pPartsGroup );
    ADN_Breakdowns_PartsTable* pPartsTable = new ADN_Breakdowns_PartsTable( strClassName_ + "_Parts", vInfosConnectors[ eParts ] );
    pPartsTable->SetGoToOnDoubleClick( ::eEquipement );
    pPartsLayout->addWidget( pPartsTable );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Specific layout
    QWidget* pSpecificContent = new QWidget();
    QVBoxLayout* pSpecificLayout = new QVBoxLayout( pSpecificContent );
    pSpecificLayout->setMargin( 10 );
    pSpecificLayout->setSpacing( 10 );
    pSpecificLayout->setAlignment( Qt::AlignTop );
    pSpecificLayout->addWidget( pInfoHolder );
    pSpecificLayout->addWidget( pPartsGroup, 1 );

    // List view
    ADN_SearchListView< ADN_Breakdowns_ListView >* pSearchListView = new ADN_SearchListView< ADN_Breakdowns_ListView >( this, data_.vBreakdowns_, vInfosConnectors );
    pListView_ = pSearchListView->GetListView();
    pListView_->setObjectName( strClassName_ + "_List" );

    // Sub content
    QWidget* pSubContent = CreateScrollArea( *pSpecificContent, pSearchListView, false, false, true, 0, 0 );

    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pGeneralGroup );
    pContentLayout->addWidget( pSubContent, 1 );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent );
    pMainWidget_->setObjectName( strClassName_ );
}
