// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ResourceNetworks_GUI.h"
#include "moc_ADN_ResourceNetworks_GUI.cpp"
#include "ADN_ColorSelector.h"
#include "ADN_GuiBuilder.h"
#include "ADN_ListView_ResourceNetworks.h"
#include "ADN_SearchListView.h"
#include "ADN_ResourceNetworks_Data.h"

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_GUI constructor
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
ADN_ResourceNetworks_GUI::ADN_ResourceNetworks_GUI( ADN_ResourceNetworks_Data& data )
    : ADN_GUI_ABC( "ADN_ResourceNetworks_GUI" )
    , data_( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_GUI destructor
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
ADN_ResourceNetworks_GUI::~ADN_ResourceNetworks_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_GUI::Build
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
void ADN_ResourceNetworks_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder;
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vInfosConnectors[ eName ] );
    builder.AddField< ADN_ComboBox_Vector< ADN_Equipement_Data::CategoryInfo > >( pInfoHolder, tr( "Category" ), vInfosConnectors[ eCategory ] );
    builder.AddField< ADN_EditLine_Int >( pInfoHolder, tr( "Default production" ), vInfosConnectors[ eProduction ] );
    builder.SetValidator( new ADN_IntValidator( 0, INT_MAX, this ) );
    builder.AddField< ADN_ColorSelector >( pInfoHolder, tr( "Color" ), vInfosConnectors[ eColor ] );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );

    // Create the list view.
    ADN_SearchListView< ADN_ListView_ResourceNetworks >* pSearchListView = new ADN_SearchListView< ADN_ListView_ResourceNetworks >( data_.GetResourceNetworksInfos(), vInfosConnectors );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent, pSearchListView );
}
