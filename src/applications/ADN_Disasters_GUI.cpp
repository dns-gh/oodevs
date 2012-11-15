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

    Q3GroupBox* pThresholdConcentrations = new Q3VGroupBox( tr( "Threshold concentrations" ) );

    Q3GroupBox* pThresholdAttritions = new Q3VGroupBox( tr( "Threshold attritions" ) );

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
    pContentLayout->addWidget( pThresholdConcentrations );
    pContentLayout->addWidget( pThresholdAttritions );
    pContentLayout->addStretch( 1 );

    // ListView
    ADN_SearchListView< ADN_ListView_Disasters >* pSearchListView = new ADN_SearchListView< ADN_ListView_Disasters >( this, data_.GetDisastersInfos(), vConnectors );
    pListView_ = pSearchListView->GetListView();
    pListView_->setObjectName( strClassName_ + "_List" );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent, pSearchListView );
    pMainWidget_->setObjectName( strClassName_ );
}
