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
    if( pMainWidget_ != 0 )
        return;

    ADN_GuiBuilder builder;

    // Create the top widget.
    pMainWidget_ = new QWidget( 0, "Resource Networks" );

    // Create the listview.
    pResourceNetworks_ = new ADN_ListView_ResourceNetworks( pMainWidget_ );
    pResourceNetworks_->GetConnector().Connect( &data_.GetResourceNetworksInfos() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    Q3GroupBox* pResourceNetwork = new Q3GroupBox( 3, Qt::Vertical, tr( "Resource network" ), pMainWidget_ );
    {
        QWidget* pHolder = builder.AddFieldHolder( pResourceNetwork );
        builder.AddField< ADN_EditLine_String >( pHolder, tr( "Name" ), vInfosConnectors[ eName ] );
    }
    {
        QWidget* pHolder = builder.AddFieldHolder( pResourceNetwork );
        builder.AddField< ADN_ComboBox_Vector< ADN_Equipement_Data::CategoryInfo > >( pHolder, tr( "Category" ), vInfosConnectors[ eCategory ] );
        builder.AddField< ADN_EditLine_Int >( pHolder, tr( "Default production" ), vInfosConnectors[ eProduction ] );
        builder.SetValidator( new ADN_IntValidator( 0, INT_MAX, this ) );
        builder.AddField< ADN_ColorSelector >( pHolder, tr( "Color" ), vInfosConnectors[ eColor ] );
    }
    pResourceNetworks_->SetItemConnectors( vInfosConnectors );
    // Layout
    Q3HBoxLayout* pMainLayout = new Q3HBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pResourceNetworks_, 1 );
    pMainLayout->addWidget( pResourceNetwork, 4 );
}
