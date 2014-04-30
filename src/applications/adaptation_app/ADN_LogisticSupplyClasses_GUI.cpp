// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_LogisticSupplyClasses_GUI.h"
#include "moc_ADN_LogisticSupplyClasses_GUI.cpp"
#include "ADN_GuiBuilder.h"
#include "ADN_ListView_Categories_LogisticSupplyClass.h"
#include "ADN_LogisticSupplyClasses_Data.h"

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClasses_GUI constructor
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
ADN_LogisticSupplyClasses_GUI::ADN_LogisticSupplyClasses_GUI( ADN_LogisticSupplyClasses_Data& data )
    : ADN_GUI_ABC( eCategories )
    , data_( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClasses_GUI destructor
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
ADN_LogisticSupplyClasses_GUI::~ADN_LogisticSupplyClasses_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClasses_GUI::Build
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
void ADN_LogisticSupplyClasses_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    builder.PushSubName( "logistic-categories" );
    T_ConnectorVector vInfosConnectors( eNbrLogisticSupplyClassGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    Q3GroupBox* pGroup = new Q3VGroupBox( tr( "Classes of supply" ) );
    // Logistic resource categories list
    pListView_ = builder.AddWidget< ADN_ListView_Categories_LogisticSupplyClass >( "list", pGroup );
    connect( pListView_, SIGNAL( UsersListRequested( const ADN_NavigationInfos::UsedBy& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_NavigationInfos::UsedBy& ) ) );
    static_cast< ADN_Connector_Vector_ABC* >( &pListView_->GetConnector() )->Connect( &data_.GetLogisticSupplyClasses() );

    // Logistic resource category info
    QWidget* pHolder = builder.AddFieldHolder( pGroup );
    pHolder->layout()->setMargin( 0 );

    // Name field
    builder.AddLocalizedField( data_.GetLogisticSupplyClasses(), pHolder, "name", tr( "Name" ), vInfosConnectors[ eLogisticSupplyClassName ] );

    builder.PopSubName(); //! logistic-categories

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
