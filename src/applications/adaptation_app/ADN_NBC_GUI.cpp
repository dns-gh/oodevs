// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_NBC_GUI.cpp $
// $Author: Ape $
// $Modtime: 18/04/05 11:38 $
// $Revision: 12 $
// $Workfile: ADN_NBC_GUI.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_NBC_GUI.h"

#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_NBC_Datas.h"
#include "ADN_CommonGfx.h"
#include "ADN_NBC_NbcAgentListView.h"
#include "ADN_GuiBuilder.h"

#include "ADN_Validator.h"
#include "ADN_TimeField.h"

#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qgrid.h>


// -----------------------------------------------------------------------------
// Name: ADN_NBC_GUI constructor
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_GUI::ADN_NBC_GUI( ADN_NBC_Datas& data )
: ADN_GUI_ABC( "ADN_NBC_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_NBC_GUI destructor
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_GUI::~ADN_NBC_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_NBC_GUI::Build
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
void ADN_NBC_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );

    // NBC agents list
    ADN_NBC_NbcAgentListView* pNBCAgentListView = new ADN_NBC_NbcAgentListView(pMainWidget_);
    pNBCAgentListView->GetConnector().Connect( &data_.GetNbcAgentVector() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    // NBC agent data
    QGroupBox* pGroup = new QGroupBox( 1, Qt::Horizontal, tr( "NBC Agent" ), pMainWidget_ );

    QWidget* pHolder = builder.AddFieldHolder( pGroup );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "Name" ), vInfosConnectors[eName] );
    builder.AddField<ADN_TimeField>( pHolder, tr( "Lifetime" ), vInfosConnectors[eLifetime] );
    builder.AddField<ADN_EditLine_Double>( pHolder, tr( "Spread angle" ), vInfosConnectors[eSpreadAngle], tr( "°" ) );
    builder.SetValidator( new ADN_DoubleValidator( 0.01, 360, 2, this ) );

    QGroupBox* pAttritionsGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Human attritions" ), pGroup );
    builder.AddField<ADN_EditLine_Double>( pAttritionsGroup, tr( "Unhurt" ), vInfosConnectors[eNbrOk], tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pAttritionsGroup, tr( "U1" ), vInfosConnectors[eNbrHurt1], tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pAttritionsGroup, tr( "U2" ), vInfosConnectors[eNbrHurt2], tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pAttritionsGroup, tr( "U3" ), vInfosConnectors[eNbrHurt3], tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pAttritionsGroup, tr( "UE" ), vInfosConnectors[eNbrHurt4], tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pAttritionsGroup, tr( "Killed" ), vInfosConnectors[eNbrDead], tr( "%" ), ePercentage );

    pNBCAgentListView->SetItemConnectors( vInfosConnectors );

    // Main vertical layout
    QVBoxLayout* pMainLayout = new QVBoxLayout( pMainWidget_, 10, 10 );

    // Top horizontal layout
    QBoxLayout* pListAndDataLayout = new QHBoxLayout( 0, 10, 10 );
    pListAndDataLayout->addWidget( pNBCAgentListView, 1 );
    pListAndDataLayout->addWidget( pGroup, 4 );

    pMainLayout->addLayout( pListAndDataLayout, 2 );
}
