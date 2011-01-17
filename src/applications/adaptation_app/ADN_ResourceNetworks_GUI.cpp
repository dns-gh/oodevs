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
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    QGroupBox* pResourceNetwork = new QGroupBox( 3, Qt::Vertical, tr( "Resource network" ), pMainWidget_ );
    {
        QWidget* pHolder = builder.AddFieldHolder( pResourceNetwork );
        builder.AddField<ADN_EditLine_String>( pHolder, tr( "Name" ), vInfosConnectors[eName] );
    }
    {
        QWidget* pHolder = builder.AddFieldHolder( pResourceNetwork );
        dotationsCombo_ = builder.AddField< ADN_ComboBox_Vector< ADN_Equipement_Data::ResourceInfos > >( pHolder, tr( "Dotation" ), vInfosConnectors[ eDotation ] );
        connect( dotationsCombo_, SIGNAL( activated( int ) ),this, SLOT( OnDotationChanged() ) );
        categoriesCombo_ = builder.AddField< ADN_ComboBox_Vector< ADN_Equipement_Data::CategoryInfo > >( pHolder, tr( "Category" ), vInfosConnectors[ eCategory ] );
        builder.AddField< ADN_EditLine_Int >( pHolder, tr( "Default production" ), vInfosConnectors[ eProduction ] );
        builder.SetValidator( new ADN_IntValidator( 0, INT_MAX, this ) );
        builder.AddField< ADN_ColorSelector >( pHolder, tr( "Color" ), vInfosConnectors[ eColor ] );
    }
    pResourceNetworks_->SetItemConnectors( vInfosConnectors );
    // Layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pResourceNetworks_, 1 );
    pMainLayout->addWidget( pResourceNetwork, 4 );
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_GUI::OnDotationChanged
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
void ADN_ResourceNetworks_GUI::OnDotationChanged()
{
    ADN_Equipement_Data::ResourceInfos* dotation = static_cast< ADN_Equipement_Data::ResourceInfos* >( dotationsCombo_->GetCurrentData() );
    ADN_ResourceNetworks_Data::ResourceNetworkInfos* pInfos = static_cast< ADN_ResourceNetworks_Data::ResourceNetworkInfos* >( pResourceNetworks_->GetCurrentData() );
    if( !dotation || !pInfos )
        return;
    ADN_Equipement_Data::T_CategoryInfos_Vector& categories = dotation->GetCategories();
    if( &pInfos->ptrCategory_.GetVector() != &categories )
    {
        categoriesCombo_->GetConnector().Connect( &pInfos->ptrCategory_, false );
        pInfos->ptrCategory_.SetVector( categories );
        pInfos->ptrCategory_.SetData( categories.size() == 0 ? 0 : categories.front(), false );
        categoriesCombo_->GetConnector().Connect( &pInfos->ptrCategory_, true );
    }
}
