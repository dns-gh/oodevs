// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Natures_GUI.h"
#include "moc_ADN_Natures_Gui.cpp"

#include "ADN_GuiBuilder.h"
#include "ADN_ListView_Categories_DotationNature.h"
#include "ADN_Natures_Data.h"

// -----------------------------------------------------------------------------
// Name: ADN_Natures_GUI constructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_Natures_GUI::ADN_Natures_GUI( ADN_Natures_Data& data )
    : ADN_GUI_ABC( eCategories )
    , data_( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Natures_GUI destructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_Natures_GUI::~ADN_Natures_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Natures_GUI::Build
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Natures_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    builder.PushSubName( "natures" );
    T_ConnectorVector vInfosConnectors( eNbrDotationNatureGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    Q3VGroupBox* pGroup = new Q3VGroupBox( tr( "Natures of supply" ) );
    // dotation natures list
    pListView_ = builder.AddWidget< ADN_ListView_Categories_DotationNature >( "list", pGroup );
    connect( pListView_, SIGNAL( UsersListRequested( const ADN_NavigationInfos::UsedBy& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_NavigationInfos::UsedBy& ) ) );
    static_cast< ADN_Connector_Vector_ABC* >( &pListView_->GetConnector() )->Connect( &data_.GetNaturesInfos() );

    // dotation nature info
    QWidget* pHolder = builder.AddFieldHolder( pGroup );
    pHolder->layout()->setMargin( 0 );

    // Name field
    builder.AddLocalizedField( data_.GetNaturesInfos(), pHolder, "name", tr( "Name" ), vInfosConnectors[ eDotationNatureName ] );

    builder.PopSubName(); //! natures

    // set auto connectors
    pListView_->SetItemConnectors( vInfosConnectors );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    pMainWidget_ = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout( pMainWidget_ );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );
    layout->addWidget( pGroup );
}
