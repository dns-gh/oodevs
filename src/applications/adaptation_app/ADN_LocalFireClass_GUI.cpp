// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_LocalFireClass_GUI.h"
#include "moc_ADN_LocalFireClass_GUI.cpp"

#include "ADN_LocalFireClass_ListView.h"
#include "ADN_LocalFireClass_Data.h"
#include "ADN_GuiBuilder.h"

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_GUI constructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_LocalFireClass_GUI::ADN_LocalFireClass_GUI( ADN_LocalFireClass_Data& data )
: ADN_GUI_ABC( "ADN_LocalFireClass_GUI" )
, data_( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_GUI destructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_LocalFireClass_GUI::~ADN_LocalFireClass_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_GUI::Build
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );

    // Local fire class list
    ADN_LocalFireClass_ListView* pLocalFireClassListView = new ADN_LocalFireClass_ListView( pMainWidget_ );
    pLocalFireClassListView->GetConnector().Connect( &data_.GetLocalFireClassesInfos() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Local fire class data
    QGroupBox* pLocalFireClassDataGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Local fire class" ), pMainWidget_ );

    ADN_GuiBuilder builder;
    QWidget* pHolder = builder.AddFieldHolder( pLocalFireClassDataGroup );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "Name" ), vInfosConnectors[ eName ] );
    builder.AddField< ADN_EditLine_Int >( pHolder, tr( "Initial heat" ), vInfosConnectors[ eInitialHeat ] );
    builder.AddField< ADN_EditLine_Int >( pHolder, tr( "Max heat" ), vInfosConnectors[ eMaxHeat ] );
    builder.AddField< ADN_EditLine_Int >( pHolder, tr( "Increase rate" ), vInfosConnectors[ eIncreaseRate ] );
    builder.AddField< ADN_EditLine_Int >( pHolder, tr( "Decrease rate" ), vInfosConnectors[ eDecreaseRate ] );

    pLocalFireClassListView->SetItemConnectors( vInfosConnectors );

    // Main vertical layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pLocalFireClassListView, 1 );
    pMainLayout->addWidget( pLocalFireClassDataGroup, 4 );
}
