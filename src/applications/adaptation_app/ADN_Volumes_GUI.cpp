// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Volumes_GUI.h"
#include "moc_ADN_Volumes_GUI.cpp"

#include "ADN_GuiBuilder.h"
#include "ADN_ListView_Categories_Size.h"
#include "ADN_Volumes_Data.h"

// -----------------------------------------------------------------------------
// Name: ADN_Volumes_GUI constructor
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
ADN_Volumes_GUI::ADN_Volumes_GUI( ADN_Volumes_Data& data )
    : ADN_GUI_ABC( eCategories )
    , data_( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Volumes_GUI destructor
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
ADN_Volumes_GUI::~ADN_Volumes_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Volumes_GUI::Build
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
void ADN_Volumes_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    builder.PushSubName( "volumes" );
    T_ConnectorVector vInfosConnectors( eNbrSizeGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    Q3VGroupBox* pGroup = new Q3VGroupBox( tr( "Sizes" ) );
    // sizes list view
    pListView_ = builder.AddWidget< ADN_ListView_Categories_Size >( "list", pGroup );
    connect( pListView_, SIGNAL( UsersListRequested( const ADN_NavigationInfos::UsedBy& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_NavigationInfos::UsedBy& ) ) );
    static_cast< ADN_Connector_Vector_ABC* >( &pListView_->GetConnector() )->Connect( &data_.GetVolumesInfos() );

    // size infos
    QWidget* pHolder = builder.AddFieldHolder( pGroup );
    pHolder->layout()->setMargin( 0 );
    ADN_EditLine_ABC* nameField = builder.AddField< ADN_EditLine_String >( pHolder, "name", tr( "Name" ), vInfosConnectors[ eSizeName ], 0, eVarName );
    nameField->ConnectWithRefValidity( data_.GetVolumesInfos() );

    builder.PopSubName(); //! volumes

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
